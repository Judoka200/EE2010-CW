//
// Created by benre on 04/11/2026.
//

#ifndef EE2010_CW_ENEMY_H
#define EE2010_CW_ENEMY_H
#include "info.h"
#include "card.h"
#include <vector>

class enemy : public info {
    public:
    enemy(std::string,int); //name,startingHealth
    Card* enemyact(); // returns a pointer to the random card the enemy is using
    std::string getname(); //returns the name of the enemy
    void addcard(Card* card); //adds a card pointer to the enemy's deck array

    private:
    vector<Card*> deck;
    std::string name;
};


#endif //EE2010_CW_ENEMY_H
