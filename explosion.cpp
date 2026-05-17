#include "explosion.h"
#include <iostream>
#include <string>

using namespace std;
using namespace sf;


explosion::explosion(float radius) : explosionRadius(radius)
{
	type = "explosion";
	name = "explosion";
	texture.loadFromFile("Assets\\explosion\\explosion_1\\explosion_1.png");

	sprite.setTexture(texture);
	sprite.setScale(0.5, 0.5);
	sprite.setOrigin(170, 170);
	isAlive = true;
	damage = 0;
	explosionTimer = 0;
	explosionTime = 10;

}

explosion :: ~explosion()
{
}

float explosion::getExplosionRadius()
{
	return explosionRadius;
}


void explosion::move(float dt)
{
	explosionTimer += (dt*5);
	if (explosionTimer > explosionTime)
	{
		isAlive = false;
	}

	
}


void explosion::update(float deltaTime, float time)
{
	
}


void explosion::draw(RenderWindow& window)
{
	texture.loadFromFile("Assets\\explosion\\explosion_1\\explosion_" + to_string(((int)(explosionTimer * 10.0 / totalexplosionTime ))+1) + ".png");
	sprite.setTexture(texture);
	sprite.setPosition(position.x - 20, position.y);
	window.draw(sprite);
}