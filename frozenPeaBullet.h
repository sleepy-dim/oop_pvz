#pragma once
#include "ammo.h"
class frozenPeaBullet : public ammo
{
public:
	frozenPeaBullet();
	void move(float)override;
};