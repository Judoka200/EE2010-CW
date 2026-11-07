#include <iostream>
#include "card.h"
#include "playerDeck.h"


int main() {
card testCard(1,"slashsd", 4);

playerDeck deck;

    card test2(2,"stabs",3);
    deck.addCard(&testCard);
    // deck.show();
    deck.addCard(&test2);
    deck.show();

    deck.addCardToHand(&test2);

}


