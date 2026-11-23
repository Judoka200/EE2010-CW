
#include "Player.h"

// constructor - sets player health and starting energy
Player::Player(int health) : info(health) {
    energy = 3;
}

// removes energy from the player
void Player::removEnergy(int amount) {
    energy -= amount;
    if (energy < 0) energy = 0;
}

// sets the player's energy
void Player::setEnergy(int amount) {
    energy = amount;
}

// returns current energy
int Player::getEnergy() {
    return energy;
}

// runs at the start of each turn
void Player::turnStart() {
    energy = 3;
    activateTurnStart();
    drawCard();
}

bool Player::playCard(card* c, info& target){
    //check if card is in hand first
    bool inHand = false;
    for (card* check : getHand()){
        if(check == c){inHand = true;
        break;}

    }
    if (!inHand){
        std::cout << "playCard: card not in hand\n";
        return false;
    }
    //check energy cost to see if playable 
    if (getEnergy() < c->getCost()){
        std::cout<< "Not enough energy\n";
    }
 
    // adds attk value as block instead of damage if defence card
    if (c->getAttackType() == ATKTYPE::def){
        addBlock(c->getAttack());
    }
    else{

    //quick debug stuffs    
    std::cout << "Target health: " << target.getHealth() << std::endl;
    std::cout << "Card pointer: " << c << std::endl;
    std::cout << "Card attack: " << c->getAttack() << std::endl;
    std::cout << "atk type: " << c->getAttackType() <<std::endl;
         target.damage(c->getAttack());     // damage targt based on card attk 
    std::cout << "Damage successful" << std::endl;
    }

 return true;


}