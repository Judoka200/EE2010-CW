
#include "Player.h"
#include "playerDeck.h"
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
}

void Player::drawCard(){
    // move 5 random cards from the drawpile to the hand
    for (int i=0; i<5; i++) {
        playerDeck::drawCard();
    }
}

