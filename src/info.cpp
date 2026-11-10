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
    block = 0;
}

void info::heal(int value){
    health = (health += value >= maxHealth)? health = maxHealth : health += value;
}

void info::damage(int value){

    // should we do death checks here??

    health = (health -= value < 0)? health = 0 : health -= value;
}

//overwrites current health value ifnoring maximum
//- will get limited to maximum on heal()
void info::setHealth(int value){
    health = value;
}

//reaname to activate turn effects??
void info::activateTurnStart(){
    // perform poison calc
    health -= poison;

    //reset block
    block = 0; 
}

void info::addPoison(int value){
    poison += value;
}

void info::addBlock(int value){
    block += value;
}