#pragma once 
#ifndef PLANT_H
#define PLANT_H

#include "character.h"
#include "MovingObject.h"

class Plant : public character
{
protected:
	int cost;
public:
	int getCost();
	void setCost(int cost);
	virtual void draw(sf::RenderWindow&);
	virtual void action(float, MovingObjectArr*) = 0;
	virtual ~Plant();
};

#endif

#ifndef PlantArr_h
#define PlantArr_h

#pragma once
#include "Plant.h"
#include <iostream>

class PlantArr {
private:
    Plant** arr;
    int capacity;
    int size;

public:
    // Constructor
    PlantArr();

    // Destructor
    ~PlantArr();

    // Copy constructor (deleted for safety as it's an owning pointer array)
    PlantArr(const PlantArr& other) = delete;

    // Assignment operator (deleted for safety as it's an owning pointer array)
    PlantArr& operator=(const PlantArr& other) = delete;

    // Push back element
    void push_back(Plant* obj);

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
    Plant* at(int index) const;

    // Insert element at index
    void insert(Plant* obj, int index);

    // Erase element at index
    void erase(int index);
};

#endif 
