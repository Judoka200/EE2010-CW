//

#ifndef EE2010_CW_PLAYER_H
#define EE2010_CW_PLAYER_H
#include "info.h"
#include "playerDeck.h"

class Player: public info {
    public:
        Player(int); //health
        void removEnergy(int); //removes specified amount of energy from total
        void setEnergy(int); //sets energy to specified value
        int getEnergy(); //returns current energy value
        int getMaxEnergy();
        void activateTurnStart() override; //runs strat of tuns stuff such as energy resetting and info's activateTurnStart
        bool playCard(card* , info&);
        playerDeck& deck() {return Pdeck;}
        
        private:
        playerDeck Pdeck;
        int energy;
        int maxEnergy;

};


#endif //EE2010_CW_PLAYER_H
