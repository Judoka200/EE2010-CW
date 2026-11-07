//
// Created by benre on 02/11/2026.
//

#include "playerDeck.h"
#include "card.h"

std::vector<card*> playerDeck::getHand(){
    return hand;
}

void playerDeck::addCard(std::vector<card*> vec, card* cd){
    vec.push_back(cd);
}

void playerDeck::removeCard(std::vector<card*> vec, card* cd){
    for (int i = 0; i<vec.size(); i++){
        if (vec[i]->iD() == cd->iD()){
            vec.erase(vec.begin()+i);
            break;
        }
    }

}


// void playerDeck::removeCard(std::vector<card*>& vec, card* cd) {
//     for (int i = 0; i < vec.size(); i++) {
//         if (vec[i]->iD() == cd->iD()) {
//             vec.erase(vec.begin() + i);
//             break;
//         }
//     }
// }



void playerDeck::show(){
    for(int i = 0; i < drawpile.size(); i++){
        std::cout << drawpile[i]->getName() << std::endl;
    }
}
