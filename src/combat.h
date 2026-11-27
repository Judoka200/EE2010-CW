#pragma once
#include <string>
#include "card.h"
#include "info.h"

int parseAttack(card* a, card* b);

// does damage calc and returns string for battle log 
std::string resolveClash(card* aCard, card* bCard, info& a, info& b,
                         const std::string& aName, const std::string& bName);

std::string resolveUnopposed(card* c, info& self, info& other,
                             const std::string& selfName, const std::string& otherName);