

#include "enemy.h"


#include <cstdlib> //library for randomisation

// this constructer sets the enemy's name and health
enemy::enemy(std::string enemyName, int health) : info(health) {
    name = enemyName;
}

// randomly selects a card from the enemy's deck
card* enemy::enemyact() {
    int randomIndex = rand() % Edeck.getDeck().size();
    return Edeck.getDeck()[randomIndex];
}

// returns the enemy's name
std::string enemy::getname() {
    return name;
}

// adds a card to the enemy's deck
void enemy::addcard(card* newCard) {
    Edeck.getDeck().push_back(newCard);
}