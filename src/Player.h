//
// Created by benre on 04/11/2026.
//

#ifndef EE2010_CW_PLAYER_H
#define EE2010_CW_PLAYER_H
#include "info.h"
#include "playerDeck.h"

class Player: public info, public playerDeck {
    public:
        Player(int); //health
        void removEnergy(int); //removes specified amount of energy from total
        void setEnergy(int); //sets energy to specified value
        int getEnergy(); //returns current energy value
        void turnStart(); //runs strat of tuns stuff such as energy resetting and info's activateTurnStart
    private:
        int energy;
};


#endif //EE2010_CW_PLAYER_H
