#include <iostream>
#include<string>
#include "card.h"
using namespace std;
int main() {
    card card1(1,"new",1,20,3,def,0,5,8,6,2,4,false);
    cout << card1.getAttack() << endl;
    cout << card1.getAttackType() << endl;
    cout << card1.getCost() << endl;
    cout << card1.getEff(1) << endl;
    cout << card1.getEff(2) << endl;
    cout << card1.getEff(3) << endl;
    cout << card1.getEff(4) << endl;
    cout << card1.getEff(5) << endl;
    cout << card1.getExclusive() << endl;
    cout << card1.getName() << endl;
    cout << card1.getProperty() << endl;
    cout << card1.getPropertyVal() << endl;
    cout << card1.getRange() << endl; // tested works
    cout << card1.iD() << endl;
    return 0;
}