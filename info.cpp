//
// Created by benre on 02/11/2026.
//

#include "info.h"

info::info(int base_Health){
    //set max health value and then create with full health
    maxHealth = base_Health;
    health = maxHealth;
    //start with no additional effects
    poison = 0;
    madness = 0;
    burning = 0;
    bleeding = 0;
    block = 0;
}

void info::heal(int value){
    health = (health += value >= maxHealth)? health = maxHealth : health += value;
}

void info::damage(int value){

    // should we do death checks here??
if ((health-value) > 0) {
    health -= value;
} else {
    health = 0;
}
}

//overwrites current health value ignoring maximum
//- will get limited to maximum on heal()
void info::setHealth(int value){
    health = value;
}

//reaname to activate turn effects??
void info::activateTurnStart(){
    // perform poison calc
    activateStatus();
    //reset block
    block = 0;
}

void info::activateStatus() {
    if (poison) {
        damage(poison); // deal damage equal to poison
        poison -= 1; // decrement by 1
        if(poison < 0){
            poison = 0;
        }
    }

    if (bleeding) {
        damage(bleeding); // deal damage equal to poison
        bleeding -= 1; // decrement by 1
        if(bleeding < 0){
            bleeding = 0;
        }
    }

    if (madness) {
        damage(madness); // deal damage equal to poison
        madness -= 1; // decrement by 1
        if(madness < 0){
            madness = 0;
        }
    }

    if (burning) {
        damage(burning); // deal damage equal to poison
        burning -= 1; // decrement by 1
        if(burning < 0){
            burning = 0;
        }
    }

}



void info::addPoison(int value){
    poison += value;
}
void info::addMadness(int value){
    madness += value;
}
void info::addBurning(int value){
    burning += value;
}
void info::addBleed(int value){
    bleeding += value;
}
void info::addBlock(int value){
    block += value;
}
int info::getHealth() const {return health;}
int info::getMaxHealth() const {return maxHealth;}
int info::getPoison() const {return poison;}
int info::getMadness() const {return madness;}
int info::getBurning() const {return burning;}
int info::getBleed() const {return bleeding;}
int info::getBlock() const {return block;}
