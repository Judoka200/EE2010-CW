#pragma once
#include <string>
#include "arcaneKnowledgeRepository.h"


class card
{
public:
    card(int id,std::string name,int cost, bool exclusive = false, int atkval, int atkrange, ATKTYPE atktype, int poison, int madness, int burning, int bleeding, int specialP, int specialPVal); //id,name,cost,exclusive to enemy
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
    ATKTYPE attackType;
    int id, eCost, attackVal, attackRange, poisonVal, madnessVal, burningVal, bleedingVal, specialProperty, specialPropertyVal; //all of these should initialise to 0
    bool isEnemyExclusive;
};



