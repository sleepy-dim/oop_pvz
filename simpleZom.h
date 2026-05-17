#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Zombie.h"

using namespace std;
class SimpleZombie :public Zombie{
	coordinate LostHeadPosition;
	Texture headtexture;
	Sprite headSprite;
public:
	SimpleZombie();

	void action(float) override;
	void animate(string) override ;
	void draw(RenderWindow& window);

	~SimpleZombie();

};

