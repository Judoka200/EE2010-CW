//
// Created by benre on 04/11/2026.
//

#ifndef EE2010_CW_ENEMY_H
#define EE2010_CW_ENEMY_H
#include <vector>
#include "info.h"
#include "playerDeck.h"

class enemy : public info, public playerDeck {
    public:
    enemy(std::string,int); //name,startingHealth
    card* enemyact(); // returns a pointer to the random card the enemy is using
    std::string getname(); //returns the name of the enemy
    void addcard(card* card); //adds a card pointer to the enemy's deck array

    private:
    playerDeck Edeck;
    std::string name;
};


#endif //EE2010_CW_ENEMY_H
