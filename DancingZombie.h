#pragma once
#ifndef DANCINGZOMBIE_H
#define DANCINGZOMBIE_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Zombie.h"
#include "Plant.h"

class playground; 

class DancingZombie : public Zombie {
	int countSpecialAbility;
	float specialAbilityTimer;
	int totalSpecialAbilityTime = 10;
	playground* lawn;
public:
	DancingZombie();

	void getLawnRef(playground* lawn);
	void action(float) override;
	void move();
	void draw(sf::RenderWindow& window);
	void animate(std::string);
	bool specialAction();

	~DancingZombie();
};

#endif
