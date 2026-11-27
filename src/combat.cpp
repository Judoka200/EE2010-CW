#include "combat.h"

int parseAttack(card* a, card* b) {
    bool aDef = a->getAttackType() == ATKTYPE::def;
    bool bDef = b->getAttackType() == ATKTYPE::def;
    if (!aDef && !bDef) return 1;   // ATTK/ATTK
    if (aDef && bDef)   return 2;   // DEF/DEF 
    return 3;                       // ATTK/DEF 
}

std::string resolveClash(card* aCard, card* bCard, info& a, info& b, const std::string& aName, const std::string& bName)
{
    int aVal = aCard->getAttack();
    int bVal = bCard->getAttack();

    // add names for battlelog 
    std::string log = aName + " (" + aCard->getName() + " " + std::to_string(aVal) + ") vs "
                    + bName + " (" + bCard->getName() + " " + std::to_string(bVal) + "): ";

    switch (parseAttack(aCard, bCard)) {
    case 1: // lower value takes the diffrence in attacks damage
        if (aVal > bVal)      {
             b.damage(aVal - bVal); log += bName + " takes " + std::to_string(aVal - bVal); 
        } else if (bVal > aVal){
            a.damage(bVal - aVal); log += aName + " takes " + std::to_string(bVal - aVal); 
        } else { log += "attacks cancel out";}
        break;

    case 2: // both gain block equal to the played card
        a.addBlock(aVal);
        b.addBlock(bVal);
        log += "both gain block";
        break;

    case 3: { 
        // i feel very smart for this one 
        // check which card is defending 
    
        bool aDefends = (aCard->getAttackType() == ATKTYPE::def); // true if Card a is def 
        info& defender = aDefends ? a : b;  // defender is a if a defends is tru
        const std::string& defName = aDefends ? aName : bName;                  // i love ternary operators :3 
        int diff = aDefends ? (bVal - aVal) : (aVal - bVal);   // attack minus defence

        if (diff > 0){
            defender.damage(diff);   log += defName + " takes " + std::to_string(diff);
        } else { 
            defender.addBlock(-diff); log += defName + " blocks, gains " + std::to_string(-diff) + " block";
        }
        break;
    }
    }
    return log;
}
// does straigh damage or block 
std::string resolveUnopposed(card* c, info& self, info& other,
                             const std::string& selfName, const std::string& otherName)
{
    int v = c->getAttack();
    if (c->getAttackType() == ATKTYPE::def) {
        self.addBlock(v);
        return selfName + " (" + c->getName() + "): gains " + std::to_string(v) + " block";
    }
    other.damage(v);
    return selfName + " (" + c->getName() + ") unopposed: " + otherName + " takes " + std::to_string(v);
}