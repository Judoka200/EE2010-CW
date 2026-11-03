#include <iostream>
#include "card.h"








int main() {

  card testCard("stab", 4, ATKTYPE::slash);

  std::cout<<testCard.name;

}


