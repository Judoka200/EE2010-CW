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
    void addPoison(); //adds poison to total poison
    void addBlock(); //adds block to blockTotal
    int getHealth(); //returns health total
    int getPoison(); //returns poison total
    int getBlock(); //returns block total
    private:
    int health,poison,block; //all default to 0
};


#endif //EE2010_CW_INFO_H
