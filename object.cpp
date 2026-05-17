#include "object.h"
#include <iostream>
using namespace std;


int object::count = 0;

object::object() : maxFrame(1), isAlive(true), position{ 0,0 }, frame(0)
{
	count++;
}


bool object::isExist()
{
	
	return isAlive ;
}

void object::setAlive(bool isAlive)
{
	this->isAlive = isAlive;
}

int object::getCount()
{
	return count;
}

void object::setPosition(Point  p)
{
	position.x = p.x;
	position.y = p.y;
}

void object::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

string object ::  getName()
{
	return name ;
}

Point object::getPosition()
{
	return this -> position;

}

Texture object::getTexture()
{
	return this->texture;
}

