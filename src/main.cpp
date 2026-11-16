#include <iostream>
#include "card.h"
#include "playerDeck.h"


int main() {
// card testCard(1,"slashsd", 4);

playerDeck deck;

    card test(2,"stabs",3,1,1,ATKTYPE::slash,2);

    deck.addCard(&test);
    deck.show();

    deck.addCardToHand(&test);

    deck.show();
}


