# 0 "C:/Users/benre/Downloads/EE2010-CW-clion-ver/EE2010-CW-clion-ver/info.cpp"
# 1 "C:\\Users\\benre\\Downloads\\EE2010-CW-clion-ver\\EE2010-CW-clion-ver\\cmake-build-debug//"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "C:/Users/benre/Downloads/EE2010-CW-clion-ver/EE2010-CW-clion-ver/info.cpp"




# 1 "C:/Users/benre/Downloads/EE2010-CW-clion-ver/EE2010-CW-clion-ver/info.h" 1
# 9 "C:/Users/benre/Downloads/EE2010-CW-clion-ver/EE2010-CW-clion-ver/info.h"
class info {
public:
    info(int);
    void heal(int);
    void damage(int);
    void setHealth(int);
    void activateTurnStart();
    void activateStatus(int);
    void addPoison(int);
    void addMadness(int);
    void addBurning(int);
    void addBleed(int);
    void addBlock(int);
    int getHealth() const;
    int getMaxHealth() const;
    int getPoison() const;
    int getMadness() const;
    int getBurning() const;
    int getBleed() const;
    int getBlock() const;
private:
    int health, maxHealth, poison, madness, burning, bleeding, block;
};
# 6 "C:/Users/benre/Downloads/EE2010-CW-clion-ver/EE2010-CW-clion-ver/info.cpp" 2

info::info(int base_Health){

    maxHealth = base_Health;
    health = maxHealth;

    poison = 0;
    madness = 0;
    burning = 0;
    bleeding = 0;
    block = 0;
}

void info::heal(int value){
    health = (health += value >= maxHealth)? health = maxHealth : health += value;
}

void info::damage(int value){


if ((health-value) > 0) {
    health -= value;
} else {
    health = 0;
}
}



void info::setHealth(int value){
    health = value;
}


void info::activateTurnStart(){

    activateStatus(0);

    block = 0;
}

void info::activateStatus(int type) {
    if (type == 0) {
        damage(poison);
        poison -= 1;
        if(poison < 0){
            poison = 0;
        }
    }

    if (type == 1) {
        damage(bleeding);
        bleeding -= 1;
        if(bleeding < 0){
            bleeding = 0;
        }
    }

    if (type == 2) {
        damage((madness * 2));
        madness = 0;
    }

    if (type == 3) {
        damage(burning);
        burning -= 1;
        if(burning < 0){
            burning = 0;
        }
    }

}



void info::addPoison(int value){
    poison += value;
}
void info::addMadness(int value){
    madness += value;
}
void info::addBurning(int value){
    burning += value;
}
void info::addBleed(int value){
    bleeding += value;
}
void info::addBlock(int value){
    block += value;
}
int info::getHealth() const {return health;}
int info::getMaxHealth() const {return maxHealth;}
int info::getPoison() const {return poison;}
int info::getMadness() const {return madness;}
int info::getBurning() const {return burning;}
int info::getBleed() const {return bleeding;}
int info::getBlock() const {return block;}
