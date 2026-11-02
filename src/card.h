#include <string>
#include "arcaneKnowledgeRepository.h"


class Card
{
public:
    card(std::string, int, bool);
    void addEffect(int,int);
    void addAttack(int, int);
    void addDef(int);
    int getAttack();
    int getDef();
    int getEff();
    bool getExclusive();
    int getCost();
    std::string getname();
private:
    /* data */
    std::string name;
    int eCost, attackVal, attackType, defVal, poisonVal, healVal;
    bool isEnemyExclusive;
protected:
    int id;
};

card::card(std::string cname, int ccost, ATKTYPE ctype){
    name = cname;
    enCost = ccost;
    type = ctype;
}

#endif //EE2010_CW_CARD_H
