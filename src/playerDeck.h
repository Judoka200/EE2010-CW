//
// Created by benre on 02/11/2026.
//

#ifndef EE2010_CW_PLAYERDECK_H
#define EE2010_CW_PLAYERDECK_H
#include "card.h"

class playerDeck {
    public:
    void addCard(int);
    void removeCard(int);
    void addCardToHand(int);
    void returnCardToDeck(int);
    void drawCard();
    card* getCard(int);
    card* getHand();
    card* getDeck();
    private:
    card* drawpile[];
    card* hand[];
};


#endif //EE2010_CW_PLAYERDECK_H
