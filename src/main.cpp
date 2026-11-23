#include <iostream>
#include "card.h"
// #include "playerDeck.h"
#include "cardGeneration.h"
#include "Player.h"
#include "enemy.h"

using namespace std;

int main() {
// card testCard(1,"slashsd", 4);

    Player p(10);
    enemy e("grunt",4);



    // std::vector<card> allCards = loadCards("src/cards.txt");  
    // for (card& c : allCards) {
        // p.addCard(&c);
    // }

     p.load("src/cards.txt");
    e.load("src/enemyCards.txt");

    



    while(e.getHealth()){

        p.turnStart();
        p.show();
        e.showInfo();
        p.playCard(p.getHand()[0],e);
        p.playCard(p.getHand()[0],e);
        e.showInfo();
    }
}




