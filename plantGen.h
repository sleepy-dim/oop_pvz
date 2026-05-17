#pragma once
#include "Plant.h"
#include "PeaShooter.h"
#include "SunFlower.h"
#include "frozenPea.h"
#include "Repeater.h"
#include "cherry.h"
#include "Wallnut.h"
#include "cactus.h"
#include <iostream>
#include <string>
using namespace std;


class plantGen {
private:
	plantGen();
	static plantGen* instance;

public:
	static plantGen* getInstance();
	Plant* createPlant(string plantName);
	~plantGen();
};
