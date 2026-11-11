//
// Created by benre on 02/11/2026.
//

#ifndef EE2010_CW_INFO_H
#define EE2010_CW_INFO_H


class info {
    public:
    info(int); //initialises with fixed amount of health
    void heal(int); //heal by adding to health added amount
    void damage(int); //performs damage calc to find remaining health
    void setHealth(int); //sets health to a specific value
    void activateTurnStart(); //activates start of turn effects e.g. poison, removal of block etc
    void activateStatus(int); //activates status effects poison = 1 madness = 2 burning = 3 bleed = 4
    void addPoison(int); //adds poison to total poison
    void addMadness(int); //adds madness to total madness
    void addBurning(int); //adds burning to total burning
    void addBleed(int); //adds bleed to total bleed
    void addBlock(int); //adds block to blockTotal
    int getHealth(); //returns health total
    int getPoison(); //returns poison total
    int getMadness(); //returns madness total
    int getBurning(); //returns burning total
    int getBleed(); //returns bleed total
    int getBlock(); //returns block total
    private:
    int health, maxHealth, health, poison, madness, burning, bleeding, block; //all default to except maxhealth.
};


#endif //EE2010_CW_INFO_H
