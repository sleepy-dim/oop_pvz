#ifndef ZOMBIE_H
#define ZOMBIE_H

#pragma once
#include "character.h"
#include <SFML/Graphics.hpp>
#include <string>

class Zombie : public character
{
protected:
	float dx;
	float dy;
	bool isMoving;
	bool isFrozen;
	float isFrozenTimer;
	float totalFrozenTime;
	float totalIntervals;
	float timer;
	std::string state;

	bool isWithoutHead;
	float headTimer;
	bool isDying;
	bool isBurnt;
	float burntTimer;

public:
	static int killCount;

	Zombie();
	virtual ~Zombie();

	float getdx();
	float getdy();
	void setSpeed(float dx, float dy);

	void draw(sf::RenderWindow& window);
	virtual void action(float x);

	virtual void animate(std::string) {};

	void setState(std::string s);
	std::string getState();

	void setIsMoving(bool b);
	bool getIsMoving();

	bool setIsFrozen(bool isFrozen);

	bool isExist() override;
	void setBurnt();
	bool isCollidable();
};

#endif

#ifndef ZombieArr_h
#define ZombieArr_h

#pragma once
#include "Zombie.h"

class ZombieArr {
private:
    Zombie** arr;
    int capacity;
    int size;

public:
    // Constructor
    ZombieArr();

    // Destructor
    ~ZombieArr();

    // Copy constructor (deleted for safety as it's an owning pointer array)
    ZombieArr(const ZombieArr& other) = delete;

    // Assignment operator (deleted for safety as it's an owning pointer array)
    ZombieArr& operator=(const ZombieArr& other) = delete;

    // Push back element
    void push_back(Zombie* obj);

    // Pop back element
    void pop_back();

    // Get size
    int getSize() const;

    // Check if empty
    bool empty() const;

    // Clear all elements
    void clear();

    // Resize the array
    void resize();

    // Access element at index
    Zombie* at(int index) const;

    // Insert element at index
    void insert(Zombie* obj, int index);

    // Erase element at index
    void erase(int index);
};

#endif 
