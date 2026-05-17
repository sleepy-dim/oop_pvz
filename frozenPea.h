#pragma once
#include "ShootingPlant.h"
#include "MovingObject.h"

class frozenPea : public ShootingPlant
{
public:
	frozenPea();
	void shoot(float, MovingObjectArr*) override;
	void draw(sf::RenderWindow&) override;
	~frozenPea();
};
