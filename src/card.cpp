//
// Created by benre on 02/11/2026.
//

#include "card.h"

card::card(int id, std::string name, int cost, bool exclusive)
: 
name(name),
id(id),
eCost(cost),
attackVal(0),
attackType(0),
defVal(0),
poisonVal(0),
healVal(0),
isEnemyExclusive(exclusive)
{
    
}

void card::addAttack(int damage, ATKTYPE type){
    attackType = type;
    attackVal = damage;
}

int card::getAttack() { return attackVal; }
int card::getDef()    { return defVal; }
int card::getEff()    { return poisonVal; }   
bool card::getExclusive() { return isEnemyExclusive; }
int card::getCost()   { return eCost; }
int card::iD()        { return id; }

std::string card::getName(){
    return name;
}