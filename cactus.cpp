#include "Cactus.h"
#include "Pea.h"

Cactus::Cactus()
	: ShootingPlant()
{
	name = "Cactus";
	cost = 125;
	health = 1000;
	texture.loadFromFile("Assets/Plants/Cactus.png");
	sprite.setTexture(texture);
	sprite.setScale(0.2,0.2);
	sprite.setOrigin(0,180);
	bulletInterval = 1.5f;
	bulletTimer = 0;
}


void Cactus::shoot(float deltaTime, MovingObjectArr* movingObjectArr)
{
	bulletTimer += deltaTime;
	if (bulletTimer >= bulletInterval)
	{
		Pea* p = new Pea();
		p->setPosition(position.x + 30, position.y + 10);
		movingObjectArr->push_back(p);
		bulletTimer = 0;
	}
}

void Cactus::draw(RenderWindow& window)
{
	sprite.setPosition(position.x, position.y);
	window.draw(sprite);
}


void Cactus::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}


Cactus::~Cactus()
{
}
