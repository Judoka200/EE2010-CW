//
// Created by benre on 04/11/2026.
//

#ifndef EE2010_CW_ENEMY_H
#define EE2010_CW_ENEMY_H
#include <vector>
#include "info.h"
#include "playerDeck.h"

class enemy: public info, public playerDeck {
public:
    enemy(std::string,int,std::string); //name,startingHealth, cardsource
    std::string getname(); //returns the name of the enemy
    virtual void drawCard() override;
private:
    std::string name;
};


#endif //EE2010_CW_ENEMY_H