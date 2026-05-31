#include "ammo.h"

ammo::ammo()
{
	type = "ammo";
	damage = 0;

}

float ammo::getDamage()
{
	return damage;
}

void ammo :: draw(RenderWindow & window )
{

	sprite.setPosition(position.x, position.y);
	window.draw(sprite);
}

void ammo::move(float)
{
	// Utilizing the overloaded operator+=
	position += coordinate(dx, dy);
}

void ammo::update(float time, float speed)
{
	move(speed);
}


bool ammo::isExist()
{
	if (!(position.x <= 1100 && position.y <= 600))
		isAlive = false;

	return isAlive;
}