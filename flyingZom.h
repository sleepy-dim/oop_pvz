#pragma once
#include <iostream>
using namespace std;
#include "Zombie.h"
class flyingZom :public Zombie
{
public:
	flyingZom();
	void action(float) override;
	void move();
	void draw(RenderWindow& window);
	void animate(string) override;
	void setPosition(float x, float y) override;
	~flyingZom();
};
