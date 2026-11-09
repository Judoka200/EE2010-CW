#pragma once
#include <string>
#include "arcaneKnowledgeRepository.h"


class card
{
public:
    card(int id,std::string name,int cost, bool exclusive = false); //id,name,cost,exclusive to enemy
    void addEffect(int,int); //poison,heal
    void addAttack(int); //damage,type
    void addDef(int); //block
    int getAttack(); //returns attack and type as array
    int getDef(); //returns def
    int getEff(); //returns poison and healval as array
    bool getExclusive(); //returns if card is exclusive
    int getCost(); //returns energy cost
    int iD(); //returns id
    std::string getName(); //returns cardname
private:
    /* data */
    std::string name;
    int id, eCost, attackVal,  defVal, poisonVal, healVal; //all of these should initialise to 0
    bool isEnemyExclusive;
};



