#ifndef ANT_H
#define ANT_H

#include "AntHill.hpp"
#include "AntBehaviorABS.hpp"

class Ant
{
private:
    // Vlastnosti mravence
    int id_Ant;
    int level_Ant; // level mravence
    int strenght_Ant; // sila mravence
    int defend_Ant; // obrana sila mravence
    AntHill home; //mraveniste mravence
public:
    Ant(int id, int level, int strenght, int defend);
    void moveTo(const AntHill &target); //pohyb mravence
    void attack(const AntHill &target); // utok mravence
    void defend(); // obrana
    void specialAbility(AntBehaviorABS &ability); // specialni schopnosti(utocnik, obrance, rychlejsi mravenec)
};

#endif // ANT_H
