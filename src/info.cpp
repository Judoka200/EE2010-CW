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

    health = (health -= value < 0)? health = 0 : health -= value;
}

//overwrites current health value ignoring maximum
//- will get limited to maximum on heal()
void info::setHealth(int value){
    health = value;
}

//reaname to activate turn effects??
void info::activateTurnStart(){
    // perform poison calc

    //reset block
    block = 0; 
}
void info::activateStatus(int type) {
    if (type == 1) {
        damage(poison); // deal damage equal to poison
        poison -= 1; // decrement by 1
    } else if (type == 2) {
        damage(madness);
        madness = 0;
    } else if (type == 3) {
        damage(burning);
        burning -= 1;
    } else if (type == 4) {
        damage(bleeding);
        bleeding -= 1;
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