#pragma once
#include "ShootingPlant.h"
class Cactus : public ShootingPlant
{
public:
	Cactus();
	void shoot(float deltaTime, MovingObjectArr*) override;
	void draw(RenderWindow&) override;
	void setPosition(float x, float y)override;
	~Cactus();
};
