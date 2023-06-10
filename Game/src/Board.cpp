#include "Board.hpp"
#include <sstream>

Board::Board() {}

Board::Board(vector<AntHill> antHills, vector<Obstacles> obstacles)
    : AntsHill_onBoard(antHills), Obstacles_onBoard(obstacles) {}

void Board::placeAntHill() // polozi anthill na mapu
{
    // implementace umístění mraveništ na desku
    for (const AntHill &anthill : AntsHill_onBoard)
    {
        int x = anthill.getPosition().getX();
        int y = anthill.getPosition().getY();
        board_for_print[y][x] = make_unique<AntHill>(anthill);
    }
}

void Board::placeObstacles(int x, int y) // polozi obstacles na mapu
{
    // Implementace umístění překážek na desku
    board_for_print[y][x] = make_unique<Obstacles>();
}

bool Board::checkplace(vector<AntHill> &AntsHill_onBoard, vector<Obstacles> &Obstacles_onBoard, int x, int y)
{
    for (const AntHill &anthill : AntsHill_onBoard)
    {
        if (!checkAroundPlace(anthill.getPosition().getX(), anthill.getPosition().getY(), x, y))
        {
            return false; // Duplicitní mraveniště
        }
    }
    return true; // Žádné duplicitní mraveniště
}

bool Board::checkAroundPlace(int x, int y, int new_x, int new_y) // kontroluje 3x3 bloky okolo mraveniste
{
    if (abs(new_x - x) <= 1 && abs(new_y - y) <= 1)
    {
        return false; // New coordinates are touching or overlapping the old coordinates
    }
    return true;
}

void Board::MakeMove(int id_from, int id_to, int choice) // choice 0 -> attack, 1 -> support, 2 -> bonus for id_from
{
    system("clear");
    if (choice == 0)
    {
        MakeMoveAttack(id_from, id_to);
    }
    else if (choice == 1)
    {
        MakeMoveSupport(id_from, id_to);
    }
    else if (choice == 2)
    {
        MakeMoveBonus(id_from);
    }

    cout << "vykonán tah" << endl;
}

vector<Position> Board::FindWay(int id_from, int id_to)
{
    Position start = AntsHill_onBoard[id_from].getPosition();
    Position end = AntsHill_onBoard[id_to].getPosition();

    board_for_print[start.getY()][start.getX()]->setPassable(true);
    board_for_print[end.getY()][end.getX()]->setPassable(true);

    int numRows = board_for_print.size();
    int numCols = board_for_print[0].size();

    vector<vector<bool>> visited(numRows, vector<bool>(numCols, false));
    vector<vector<int>> distance(numRows, vector<int>(numCols, INT_MAX));
    vector<vector<Position>> predecessors(numRows, vector<Position>(numCols));

    queue<Position> q;

    vector<int> dr = {-1, 1, 0, 0};
    vector<int> dc = {0, 0, -1, 1};

    q.push(start);
    visited[start.getY()][start.getX()] = true;
    distance[start.getY()][start.getX()] = 0;

    while (!q.empty())
    {
        Position current = q.front();
        q.pop();

        if (current == end){
            // cout << "konec" << endl;
            break;
        }
        // cout << current.getX() << "||" << end.getX() <<"--" << current.getY() << "||" << end.getY() << endl;
        for (int i = 0; i < 4; i++)
        {
            int newRow = current.getY() + dr[i];
            int newCol = current.getX() + dc[i];

            if (isValidPosition(newRow, newCol) && !visited[newRow][newCol])
            {
                Position neighbor(newCol, newRow);
                q.push(neighbor);
                visited[newRow][newCol] = true;
               distance[newRow][newCol] = distance[current.getY()][current.getX()] + 1; 
                predecessors[newRow][newCol] = current;
            }
            // cout << newRow <<  "||" << newCol << endl;
        }
    }

    vector<Position> path;

    if (distance[end.getY()][end.getX()] != INT_MAX)
    {
        Position current = end;
        while (current != start)
        {
            path.push_back(current);
            current = predecessors[current.getY()][current.getX()];
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
    }

     board_for_print[start.getY()][start.getX()]->setPassable(false);
    board_for_print[end.getY()][end.getX()]->setPassable(false);
    return path;
}

bool Board::isValidPosition(int row, int col)
{
    int numRows = board_for_print.size();
    int numCols = board_for_print[0].size();
    return (row >= 0 && row < numRows && col >= 0 && col < numCols && board_for_print[row][col]->IsPassable());
}

void Board::MakeMoveBonus(int id_from)
{
}

void Board::MakeMoveSupport(int id_from, int id_to)
{
    vector<Position> way = FindWay(id_from, id_to);
}

void Board::MakeMoveAttack(int id_from, int id_to)
{
    vector<Position> way;
    way = FindWay(id_from, id_to);
    for (auto test : way)
    {
        cout << test.getX() << "," << test.getY() << endl;
    }
}

void Board::BoardForPrintMake(int y_board, int x_board) // vytvoří 2d pole pro tisk mapy
{
    board_for_print.resize(x_board);
    for (int row = 0; row < x_board; row++)
    {
        board_for_print[row].resize(y_board);
        for (int col = 0; col < y_board; col++)
        {
            if (row == 0 || row == x_board - 1 || col == 0 || col == y_board - 1)
            {
                board_for_print[row][col] = std::make_unique<Obstacles>();
            }
            else
            {
                board_for_print[row][col] = std::make_unique<EmptySpace>();
            }
        }
    }
}

void Board::printAnthillOwner(int owner) // tiskne mraveniste podle majitele
{                                        // tiskne id mravenist dle ownera
    for (const AntHill &anthill : AntsHill_onBoard)
    {
        if (anthill.getOwner() == owner)
        {
            cout << anthill.getId() << " -> " << anthill.getNumberOfAnts() << " || ";
        }
    }
}

char Board::printChoiceOfMove() // tiskne možnosti co udelat utok -> obrana
{
    char choice = 0;
    cout << "1: Útok" << endl;
    cout << "2: Podpora" << endl;
    cout << "3: Bonus" << endl;
    cout << "4: Nic" << endl;
    cout << "5: Uložit" << endl;
    cin >> choice;
    system("clear");
    printBoard();
    return choice;
}

int Board::printChoiceAnthillFrom() // z jakeho mraveniste se utoci
{
    int id;
    for (const AntHill &anthill : AntsHill_onBoard)
    {
        if (anthill.getOwner() == 1)
        {
            cout << anthill.getId() << " -> " << anthill.getNumberOfAnts() << " || ";
        }
    }
    cin >> id;
    if (std::cin.eof())
    {
        return 0;
    }
    if (cin.fail())
    {
        cin.clear();                                         // Vynulovat příznaky chyby u cin
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignorovat zbylé znaky ve vstupním proudu až do konce řádku
    }
    else
    {
        for (const AntHill &anthill : AntsHill_onBoard)
        {
            if (anthill.getOwner() == 1)
            {
                if (anthill.getId() == id)
                {
                    return id;
                }
            }
        }
    }
    system("clear");
    printBoard();
    cout << "Zadej svoje mraveniště odkud se vykoná akce" << endl;
    return printChoiceAnthillFrom();
}

// public
void Board::loadMap(string name_of_map) // nacteni mapy -> vybrani velikost hriste a pozice barier + pozice mravenistw
{
    x_board = 0;
    y_board = 0;
    string Maps = "../examples/";
    string File_Name = Maps + name_of_map; // cesta ke složce + nazev vybrane mapy
    ifstream file(File_Name);              // otevreni souboru
    if (file.is_open())
    {
        string line;
        getline(file, line);
        line.find("x: "); // najde hodnotu x
        Board::x_board = stoi(line.substr(3));
        getline(file, line);
        line.find("y: "); // najde hodnotu y
        Board::y_board = stoi(line.substr(3));
        BoardForPrintMake(x_board, y_board);

        while (getline(file, line)) // prochazi vsechny radky
        {
            if (line.find("anthills:") != string::npos) // našlo to anthills
            {

                int id = 0; // Inkrementace ID před cyklem while
                int x = 0;
                int y = 0;
                while (getline(file, line) && line.find("-") != string::npos) // najde to pomlcku -> další mraveniste
                {

                    getline(file, line); // Načtení řádku s "x:"
                    try
                    {
                        x = stoi(line.substr(line.find(":") + 1));
                    }
                    catch (const invalid_argument &e)
                    {
                        continue;
                    }

                    getline(file, line); // Načtení řádku s "y:"
                    try
                    {
                        y = stoi(line.substr(line.find(":") + 1));
                    }
                    catch (const invalid_argument &e)
                    {
                        continue;
                    }

                    if (((x < x_board - 1 && x > 1) && (y < y_board - 1 && y > 1)) && checkplace(AntsHill_onBoard, Obstacles_onBoard, x, y))
                    {
                        AntHill new_Anthill(id, x, y);
                        AntsHill_onBoard.push_back(new_Anthill); // dám mraveniste do vektoru
                        id++;                                    // Inkrementace ID pro další mraveniště
                    }
                }
            }
            else if (line.find("obstacles:") != string::npos && line.substr(0, 10) == "obstacles:")
            {
                while (getline(file, line) && !line.empty())
                {

                    // Rozdělení řádku podle čárky na souřadnice x a y
                    size_t commaPos = line.find(",");
                    if (commaPos != string::npos)
                    {
                        int x = stoi(line.substr(0, commaPos));
                        int y = stoi(line.substr(commaPos + 1));

                        // Přidání překážky do vektoru
                        if (((x < x_board - 1 && x > 1) && (y < y_board - 1 && y > 1)) && checkplace(AntsHill_onBoard, Obstacles_onBoard, x, y))
                        {
                            placeObstacles(x, y);
                        }
                    }
                }
            }
        }
        file.close();
        system("clear");
    }
    else
    {
        cout << "Nepodařilo se otevřít soubor." << endl;
    }

    if (x_board == 0 || y_board == 0) // kontrola že je zadane x a y
    {
        cout << "Spatný konfiguracni soubor nektere hodnoty byly dodany automaticky\n"
             << endl;
        x_board = 100;
        y_board = 25;
    }

    placeAntHill();
}

void Board::moveAnt(Ant *ant, const Position &newPosition)
{
    // implementace pohybu mravence na desce
}

void Board::removeAnt(Ant *ant)
{
    // implementace odstranění mravence z desky
}

bool Board::checkWin() // kontroluje jestli někdo nevyhral
{
    test_counter++;
    if (test_counter == 5)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Board::printAnthills() // tiskne jak na tom jsou mraveniste
{                           // tisk aktuálních mravenist
    cout << "Mraveniště hráče: ";
    printAnthillOwner(1);
    cout << endl;

    cout << "Mraveniště neobsazené: ";
    printAnthillOwner(0);
    cout << endl;

    cout << "Mraveniště počítače: ";
    printAnthillOwner(2);
    cout << "\n"
         << endl;
}

void Board::printBoard() // tiskne mapu
{
    for (long unsigned int row = 0; row < board_for_print.size(); row++)
    {
        for (long unsigned int col = 0; col < board_for_print[row].size(); col++)
        {
            board_for_print[row][col]->print();
        }
        cout << endl;
    }
}

bool Board::printMove() // zpracovava tisk a vyber ukonu
{                       // tiskne možné tahy
    char choice = printChoiceOfMove();
    int id_from = -1;
    int id_to = -1;
    switch (choice)
    {
    case '1': // Kód pro provedení akce Attack
        cout << "Útok z mraveniště" << endl;
        id_from = printChoiceAnthillFrom(); // otazka odkud se zautoci
        if (std::cin.eof())
            return false;
        system("clear");
        printBoard();
        cout << "Kam chceš zaútočit" << endl;
        id_to = AntsHill_onBoard[id_from].printAttackTo(AntsHill_onBoard); // kam se zautoci
        if (std::cin.eof())
            return false;
        MakeMove(id_from, id_to, 0);
        break;
    case '2': // Kód pro provedení akce Support
        cout << "Podpora z mraveniště" << endl;
        id_from = printChoiceAnthillFrom(); // otazka odkud se podpori
        system("clear");
        printBoard();
        cout << "Kam chceš Podporit" << endl;
        id_to = AntsHill_onBoard[id_from].printSupportTo(AntsHill_onBoard); // kam se podpori
        MakeMove(id_from, id_to, 1);
        break;
    case '3': // Kód pro provedení akce Bonus
        cout << "Kam chces umistit bonus" << endl;
        id_from = printChoiceAnthillFrom();
        MakeMove(id_from, -1, 2);
        break;
    case '4': // Kód pro provedení akce Nic
        system("clear");

        break;
    case '5': // Kód pro provedení akce Uložit
        system("clear");
        break;
    default:
        if (std::cin.eof())
        {
            return false;
        }
        cout << "Neplatná volba -> zvol něco jiného" << endl;

        printMove();
    }
    return true;
}