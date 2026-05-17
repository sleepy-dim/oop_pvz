#ifndef ammo_h
#define ammo_h

#pragma once
#include "MovingObject.h"


class ammo : public MovingObject 
{
protected:
	float damage;
public:
	ammo();		
	float getDamage();
	void draw(RenderWindow& ) override ;
	void move(float) override;
	void update(float time, float speed)override;
	bool isExist() override;
};
#endif