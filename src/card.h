#pragma once
#include <string>
#include "arcaneKnowledgeRepository.h"


class card
{
public:
    card(int id,std::string name,int cost, int atkval, int atkrange, ATKTYPE atktype, int poison=0, int madness=0, int burning=0, int bleeding=0, int specialP=0, int specialPVal=0,bool exclusive = false); //id,name,cost,exclusive to enemy
    int getAttack() const; //returns attack
    int getRange() const; //returns range
    ATKTYPE getAttackType() const; //returns type: slash,strike,stab,def
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
    ATKTYPE attackType;
    int id, eCost, attackVal, attackRange, poisonVal, madnessVal, burningVal, bleedingVal, specialProperty, specialPropertyVal; //all of these should initialise to 0
    bool isEnemyExclusive;
};



