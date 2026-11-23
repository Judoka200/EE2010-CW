

#include "enemy.h"
#include "playerDeck.h"
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
void enemy::drawCard(){
    // move 3 random cards from the drawpile to the hand
    for (int i=0; i<3; i++) {
        playerDeck::drawCard();
    }
}
