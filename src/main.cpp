#include <iostream>
#include "card.h"
#include "playerDeck.h"
#include "card_generation.h"

int main() {
card testCard(1,"slashsd", 4);

playerDeck deck;

deck.loadDeck("cards.txt");
deck .show();

deck.addCardToHand(deck.getDeck()[0]);
deck.show();
deck.clearHand();
deck.addCardToHand(deck.getDeck()[0]);
deck.addCardToHand("Fireball");
deck.show();
}
