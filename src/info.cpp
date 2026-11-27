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
    if (health + value > maxHealth) {
        health = maxHealth;
    } else {
        health += value;
    }
    if (value < 0) { // if you heal negative value
        damage(-value); // run through damage for its checks
    }
}

void info::damage(int value) {

    // Block damage 
    if (block >= value) {
        block -= value;
    }
    else {
        // damage after block
        int remainingDamage = value - block;

        // Remove all remaining block
        block = 0;

        // do damage to health 
        if (health - remainingDamage > 0) {
            health -= remainingDamage;
        }
        else {
            health = 0;
        }
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
    activateStatus(1);
    //reset block
    
    block = 0; 
}

void info::activateStatus(int type) {
    switch (type) {
        case 1:
            damage(poison); // deal damage equal to poison
            poison -= 1; // decrement by 1
            if(poison < 0){
                poison = 0;
            }
            break;
        case 2:
            damage(madness * 2); // deal damage equal to madness*2
            madness = 0;
            break;
        case 3:
            damage(bleeding); // deal damage equal to bleeding
            bleeding -= 1; // decrement by 1
            if(bleeding < 0){
                bleeding = 0;
            }
            break;
        case 4:
            damage(burning); // deal damage equal to burning
            burning -= 1; // decrement by 1
            if(burning < 0){
                burning = 0;
            }
            break;
        default: // do nothing if outside
            break;
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

int info::getMaxHealth(){
    return maxHealth;
}

void info::showInfo(){
    std::cout << "health: "<< getHealth() << std::endl;
}