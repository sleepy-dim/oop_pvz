#ifndef object_h
#define object_h

#pragma once


#include <SFML/Graphics.hpp>
using namespace sf;
#include <iostream>
#include <string>
using namespace std;
#include "coordinate.h"

class object
{
	friend class playground;

protected:
	sf::Texture texture;
	sf::Sprite sprite;
	static int count;
	coordinate position;
	bool isAlive;
	int frame;
	int maxFrame;
	string name; 
	


public:
	
	object();
	virtual bool isExist();
	void setAlive(bool isAlive);
	void setPosition(coordinate);
	coordinate getPosition();
	virtual void setPosition(float x, float y);
	static int getCount();
	string getName();
	Texture getTexture();

};




#endif