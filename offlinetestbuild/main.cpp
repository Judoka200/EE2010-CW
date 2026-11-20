#include <iostream>
#include<string>
#include "card.h"
#include "Player.h"
#include <vector>
#include <fstream> //library to read from file system
using namespace std;
vector<card*> list;
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

    Player player1(30);
    player1.setEnergy(6);
    cout << "energy = " << player1.getEnergy() << endl;
    player1.removEnergy(4);
    cout << "energy = " << player1.getEnergy() << endl;
    player1.turnStart();
    cout << "energy = " << player1.getEnergy() << endl;
    player1.damage(3);
    return 0;
}