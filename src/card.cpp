//
// Created by benre on 02/11/2026.
//

#include "card.h"

card::card(int id, std::string name, int cost, bool exclusive, int atkval = 0, int atkrange, ATKTYPE atktype, int poison , int madness , int burning, int bleeding, int specialP, int specialPVal) :
    name(name),
    id(id),
    eCost(cost),
    attackVal(atkval),
    attackRange(atkrange),
    attackType(atktype),
    poisonVal(poison),
    madnessVal(madness),
    burningVal(burning),
    bleedingVal(bleeding),
    specialProperty(specialP),
    specialPropertyVal(specialPVal),
    isEnemyExclusive(exclusive)
{}


/* Adds an attack to a card with a set damage and type 
 slash will be (3,slash) etc 
 poison would be (0,poison) if we handle poisoin damage seperatley 
 */

int card::getAttack() const { return attackVal; }
int card::getRange() const { return attackRange; }
int card::getAttackType() const { return attackType;}
int card::getEff(int type)  const  {  // returns effect values based on input
    if (type == 1) {
        return poisonVal;
    } else if (type == 2) {
        return madnessVal;
    } else if (type == 3) {
        return burningVal;
    } else if (type == 4) {
        return bleedingVal;
    } else
        return 0;
}
bool card::getExclusive() const { return isEnemyExclusive; }
int card::getCost()  const { return eCost; }
int card::iD() const { return id; }

std::string card::getName() const { return name; }
int card::getProperty() const { return specialProperty; }
int card::getPropertyVal() const { return specialPropertyVal; }