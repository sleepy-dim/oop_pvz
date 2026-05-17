#pragma once
#include "ShootingPlant.h"
#include "MovingObject.h"

class Repeater : public ShootingPlant
{
	float bulletinterval_2;
	float bulletTimer_2;
public : 
	Repeater();
	void shoot(float, MovingObjectArr *)override;
	
	void draw(RenderWindow & )override;
	
};
