//
// Created by benre on 02/11/2026.
//

#include "playerDeck.h"
#include "card.h"
#include "cardGeneration.h"
#include <cstdlib>


void playerDeck::load(const std::string& filename) {
    for (card& c : loadCards(filename)) {              // :: = the global function, not this method
        owned.push_back(std::make_unique<card>(c));      // deck now owns a copy
        drawpile.push_back(owned.back().get());          // drawpile just points at it
    }
}

// append a card to the end of the drawpile vector
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

//return all cards from the hand to the drawpile
void playerDeck::clearHand(){
    drawpile.insert(drawpile.end(), hand.begin(), hand.end());
    hand.clear();
}

void playerDeck::drawCard(){
    if (drawpile.empty()){      // check to see if drawpile empty first to stop accessing a card that doesnt exist
        std::cout<<"drawpile empty\n";
        return;
    }

    // move a random card from the drawpile to the hand 
    int randomIndex = rand() % drawpile.size();

    moveCard(drawpile, hand, drawpile[randomIndex]);
}


std::vector<card*> playerDeck::getHand(){
        return hand;
    }


    
//returns vector<card*> of the drawpile and hand
std::vector<card*> playerDeck::getDeck(){
    std::vector<card*> all = drawpile;
    all.insert(all.end(), hand.begin(), hand.end());
    return all;
}


// show all current cards within the drawpile and hand 
void playerDeck::show(){
    std::cout<<"\e[1;33m drawpile:\n\e[22m";
    for(card* c : drawpile){
        std::cout<< c-> getName()<<std::endl;
    }
    std::cout<< "\e[1;32m hand\n\e[22m";
    for (card* c : hand){
        std::cout << c-> getName()<<std::endl;
    }
    std::cout<<"\e[0m";
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