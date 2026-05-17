#pragma once
#include "ammo.h"

class explosion : public ammo
{
	
	float explosionTimer;
	int  explosionTime;
	float explosionRadius;
public:
	explosion(float);
	
	~explosion();
	void update(float , float )override ;
	void move(float)override;
	void draw(RenderWindow &) override;
	float getExplosionRadius();


};