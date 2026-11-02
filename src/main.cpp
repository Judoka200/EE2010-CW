#include <iostream>
#include "card.h"



class uiBlock
{
private:
  /* data */
public:
  uiBlock(/* args */);
  ~uiBlock();
};

uiBlock::uiBlock(/* args */)
{
}

uiBlock::~uiBlock()
{
}




int main() {

  card testCard("stab", 4, ATKTYPE::slash);

  std::cout<<testCard.name;

}


