//
// Created by benre on 02/11/2026.
//

#include "playerDeck.h"
#include "card.h"



void playerDeck::addCard(card* cd) { drawpile.push_back(cd); }


void playerDeck::removeCard(card* cd) {
    for (auto* vec : {&drawpile, &hand}) {
        for (int c = 0; c < vec->size(); c++) {
            if ((*vec)[c]->iD() == cd->iD()) {
                vec->erase(vec->begin() + c);
                return;
            }
        }
    }
    std::cout << "removeCard: card not in deck\n";
}

void playerDeck::addCardToHand(card* cd){
    if(!moveCard(drawpile,hand,cd)){
        std::cout << "addCardToHand: card not in drawpile\n";
    }
}

void playerDeck::returnCardToDeck(card* cd){
    if(!moveCard(hand,drawpile,cd)){
        std::cout<< "returnCardToDeck: card not in hand";
    }
}

void playerDeck::clearHand(){
    for(card* c : hand){
        returnCardToDeck(c);
    }
}

std::vector<card*> playerDeck::getHand(){
        return hand;
    }

std::vector<card*> playerDeck::getDeck(){
    std::vector<card*> deck = drawpile;
    deck.insert(deck.end(),hand.begin(),hand.end());
    return deck;
}



void playerDeck::show(){
    std::cout<<"drawpile:\n";
    for(card* c : drawpile){
        std::cout<< c-> getName()<<std::endl;
    }
    std::cout<< "hand\n";
    for (card* c : hand){
        std::cout << c-> getName()<<std::endl;
    }
}

bool playerDeck::moveCard(std::vector<card*>& from, std::vector<card*>& to, card* cd) {
    for (int c = 0; c < from.size(); c++) {
        if (from[c]->iD() == cd->iD()) {
            to.push_back(from[c]);
            from.erase(from.begin() + c);
            return true;
        }
    }
    return false;
}