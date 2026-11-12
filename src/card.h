#pragma once
#include <string>
#include "arcaneKnowledgeRepository.h"


class card
{
public:
    card(int id,std::string name,int cost, bool exclusive = false, int attackVal, int attackRange, ATKTYPE attackType, int poisonVal, int madnessVal, int burningVal, int bleedingVal, int specialProperty, int specialPropertyVal); //id,name,cost,exclusive to enemy
    int getAttack() const; //returns attack
    int getRange() const; //returns range
    int getAttackType() const; //returns type: slash,strike,stab,def
    int getEff(int) const; //returns corresponding effect 1 = poison 2 = madness 3 = burning 4 = bleeding
    bool getExclusive() const; //returns if card is exclusive
    int getCost() const; //returns energy cost
    int iD() const; //returns id
    std::string getName() const; //returns cardname
    int getProperty() const; //returns specialProperty
    int getPropertyVal() const; //returns specialPropertyVal
private:
    /* data */
    std::string name;
    int id, eCost, attackVal, attackRange, attackType, poisonVal, madnessVal, burningVal, bleedingVal, specialProperty, specialPropertyVal; //all of these should initialise to 0
    bool isEnemyExclusive;
};



