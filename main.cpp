#include <iostream>
#include "card.h"
// #include "playerDeck.h"
#include "cardGeneration.h"
#include "Player.h"

using namespace std;

int main() {
    // card testCard(1,"slashsd", 4);

    Player p(10);

    std::vector<card> allCards = loadCards("src/cards.txt");
    for (card& c : allCards) {
        p.addCard(&c);
    }

    p.show();
}


