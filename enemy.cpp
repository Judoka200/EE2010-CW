

#include "enemy.h"


#include <cstdlib> //library for randomisation

// this constructer sets the enemy's name, health and cards
enemy::enemy(std::string enemyName, int health, std::string cardfile) : info(health) {
    name = enemyName;
    load(cardfile);
}

// returns the enemy's name
std::string enemy::getname() {
    return name;
}
