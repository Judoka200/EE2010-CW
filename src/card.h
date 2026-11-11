#pragma once
#include <string>
#include "arcaneKnowledgeRepository.h"


class card
{
public:
    card(int id,std::string name,int cost, bool exclusive = false, int attackVal, int attackRange, ATKTYPE attackType, int poisonVal, int madnessVal, int burningVal, int bleedingVal, int specialProperty, int specialPropertyVal); //id,name,cost,exclusive to enemy
    int getAttack() const; //returns attack and range as array
    int getType() const; //returns type: slash,strike,stab,def
    int getEff() const; //returns Effects as array
    bool getExclusive() const; //returns if card is exclusive
    int getCost() const; //returns energy cost
    int iD() const; //returns id
    std::string getName() const; //returns cardname
    int getProperty() const; //returns specialProperty and specialPropertyVal
private:
    /* data */
    std::string name;
    int id, eCost, attackVal, attackRange, attackType, poisonVal, madnessVal, burningVal, bleedingVal, specialProperty, specialPropertyVal; //all of these should initialise to 0
    bool isEnemyExclusive;
};



