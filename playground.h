#ifndef playground_h
#define playground_h
#pragma once
#include "object.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "plantGen.h"
#include "Plant.h"
#include "MovingObject.h"
#include "Zombie.h"
#include "zombieGen.h"

using namespace sf;

class zombieGen;
class plantGen;

class playground
{
private:
	int GRID_START_X ;
	int GRID_START_Y ;
	int BoxHeight;
	int BoxWidth;
	bool available [5][9];
	
	bool isLost;

	
public:


	MovingObjectArr movingObjectsArray;
	ZombieArr zombieArray;
	zombieGen* zombieGen;
	PlantArr plantsArray;
	plantGen* plantGen;



public:
	playground(int);
	int getboxHeight();
	int getboxWidth();

	int getGRID_START_X();
	int getGRID_START_Y();
	Point getGridPosition(float x, float y);

	PlantArr& getPlantsArray();
	
	bool isBoxAvailable(int x, int y);
	void setBoxAvailable(int x, int y, bool value);
	
	void addPlant(float x, float y, string type);   //e/////////Might need to change the type to string
	void updatePlants(float, MovingObjectArr*);
	void drawPlants(RenderWindow& window);


	void addZombie(float x, float y, string type);
	void updateZombie(float);
	
	void drawZombie(RenderWindow& window);


	void drawMovingObjects(RenderWindow& window);
	void updateMovingObjects(float);

	void selectSun(float x, float y); //Select Sun from screen

	void removeDead();
	bool removePlant(float x, float y);	
	bool waterPlant(float x, float y);

	void collisionAmmo_Zombie();
	void collisionPlant_Zombie();


	bool checkWin();
	bool checkLost();
	~playground();


};


#endif