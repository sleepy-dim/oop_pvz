#include "frozenPeaBullet.h"


frozenPeaBullet::frozenPeaBullet()
{

	name = "FrozenPea";
	this->count++;
	this->isAlive = true;
	this->position = { 0, 0 };
	this->dx = 8;
	this->dy = 0;
	this->texture.loadFromFile("Assets\\Bullets\\PeaIce\\icepea.png");
	this->sprite.setTexture(texture);


}

void frozenPeaBullet::move(float)
{
	position.x += dx;
	position.y += dy;
	sprite.setPosition(position.x, position.y);

}