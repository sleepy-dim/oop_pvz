#include "character.h"



String character::getName() {
	return name;
}

void character::setName(String n) {
	name = n;
}


bool character::isExist()
{
	return (health > 0) && object::isExist();
}

int character::getHealth()
{
	return health;
}

void character::setHealth(int health)
{
	this->health = health;
}

int character::getDamage()
{
	return damage;
}

void character::setDamage(int damage)
{
	this->damage = damage;
}

