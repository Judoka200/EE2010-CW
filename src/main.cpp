#include <iostream>
#include "card.h"
#include "playerDeck.h"


int main() {
card testCard(1,"slashsd", 4, false);
std::cout<<(testCard.getName())<<std::endl;

playerDeck deck;

    deck.addCard(deck.getHand(),&testCard);
    deck.show();
}


