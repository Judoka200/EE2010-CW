//
// Created by benre on 02/11/2026.
//
#pragma once
#ifndef EE2010_CW_PLAYERDECK_H
#define EE2010_CW_PLAYERDECK_H
#include "card.h"
#include <vector>
#include <iostream>

class playerDeck {

    public:
    // playerDeck(); //constructor initialises drawpile and hand
    void addCard(std::vector<card*> vec,card* cd); //takes card id and adds it to drawpile. throws error message if card does not exist
    void removeCard(std::vector<card*> vec, card* cd); //removes card reference from drawpile. if not in drawpile check hand. if not in either throw error
    void addCardToHand(int); //adds card to hand from drawpile. throw error message if card is not in drawpile
    void returnCardToDeck(int); //add card to drawpile from hand. throw error message if card is not in hand
    void clearhand(); //puts entire hand in drawpile for easier referencing.
    void drawCard(); //draws one random card from drawpile to hand. does noting if drawpile is empty
    card* getCard(int); //returns pointer to card.
    std::vector<card*> getHand(); //returns array/vector of pointers to hand
    std::vector<card*> getDeck(); //returns array/vector of pointers to entire deck

    void show();

    private:
    std::vector<card*> drawpile;
    std::vector<card*> hand; //vector of class pointers
};


#endif

