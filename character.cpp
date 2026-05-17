#include "character.h"



string character::getName() {
	return name;
}

void character::setName(string n) {
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

