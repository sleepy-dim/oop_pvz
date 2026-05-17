#pragma once
#include "Zombie.h"
#include "simpleZom.h"
#include "flyingZom.h"
#include <iostream>
#include <string>

class zombieGen {
private:
	zombieGen(); 
	static zombieGen* instance; 

public:
	static zombieGen* getInstance(); 
	Zombie* createZombie(std::string zombieName);
	~zombieGen(); 
};