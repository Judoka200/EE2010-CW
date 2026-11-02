//
// Created by benre on 02/11/2026.
//

#ifndef EE2010_CW_CARD_H
#define EE2010_CW_CARD_H


class Card {
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
    std::string name;
    int eCost, attackVal, attackType, defVal, poisonVal, healVal;
    bool isEnemyExclusive;
protected:
    int id;
};


#endif //EE2010_CW_CARD_H
