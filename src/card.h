#include <string>
#include "arcaneKnowledgeRepository.h"


class Card
{
public:
    card(ind,std::string,int, bool); //id,name,cost,exclusive
    void addEffect(int,int); //poison,heal
    void addAttack(int,int); //damage,type
    void addDef(int); //block
    int getAttack(); //returns attack and type as array
    int getDef(); //returns def
    int getEff(); //returns poison and healval as array
    bool getExclusive(); //returns if card is exclusive
    int getCost(); //returns energy cost
    std::string getname(); //returns cardname
private:
    /* data */
    std::string name;
    int eCost, attackVal, attackType, defVal, poisonVal, healVal; //all of these should initialise to 0
    bool isEnemyExclusive;
protected:
    int id; //protected so other cards can read and findthis
};

#endif //EE2010_CW_CARD_H
