//
// Created by benre on 02/11/2026.
//

#ifndef EE2010_CW_PLAYERDECK_H
#define EE2010_CW_PLAYERDECK_H
#include "card.h"

class playerDeck {
    public:
    void addCard(int); //takes card id and adds it to drawpile. throws error message if card does not exist
    void removeCard(int); //removes card reference from drawpile. if not in drawpile check hand. if not in either throw error
    void addCardToHand(int); //adds card to hand from drawpile. throw error message if card is not in drawpile
    void returnCardToDeck(int); //add card to drawpile from hand. throw error message if card is not in hand
    void clearhand(); //puts entire hand in drawpile for easier referencing.
    void drawCard(); //draws one random card from drawpile to hand. does noting if drawpile is empty
    card* getCard(int); //returns pointer to card.
    card* getHand(); //returns array of pointers to hand
    card* getDeck(); //returns array of pointers to entire deck
    private:
    card* drawpile[]; //drawpile array definition
    card* hand[]; //hand array definition
};


#endif //EE2010_CW_PLAYERDECK_H
