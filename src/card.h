#include <string>
#include "arcaneKnowledgeRepository.h"

class card
{
private:
    /* data */
public:
    card(std::string name, int cost, ATKTYPE type);
    ~card();

    std::string name;
    int enCost;
    ATKTYPE type;

};

card::card(std::string cname, int ccost, ATKTYPE ctype){ 
    name = cname;
    enCost = ccost;
    type = ctype;
}

card::~card()
{
}
