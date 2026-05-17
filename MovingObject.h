#ifndef MovingObject_h
#define MovingObject_h

#pragma once
#include "object.h"
#include <string>

class MovingObject :  public object
{
protected :

	float dx;
	float dy;
	std::string type;
	
public :
	virtual void update (float,float) = 0;
	virtual void move(float) = 0;
	virtual void draw(sf::RenderWindow & ) = 0;
	std::string getType();
	
	void setVelocity(float dx, float dy);
	float getdx();
	float getdy();
};

#endif // !MovingObject_h

#pragma once
#ifndef MovingObjectArr_h
#define MovingObjectArr_h

#include "MovingObject.h"
#include <iostream>

class MovingObjectArr {
private:
    MovingObject** arr;
    int capacity;
    int size;

public:
    // Constructor
    MovingObjectArr();

    // Destructor
    ~MovingObjectArr();

    // Copy constructor (implemented for deep copy of the array)
    MovingObjectArr(const MovingObjectArr& other);

    // Assignment operator (implemented for deep copy of the array)
    MovingObjectArr& operator=(const MovingObjectArr& other);

    // Push back element
    void push_back(MovingObject* obj);

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

    MovingObject* at(int index) const;

    // Insert element at index
    void insert(MovingObject* obj, int index);

    // Erase element at index
    void erase(int index);
};


#endif 
