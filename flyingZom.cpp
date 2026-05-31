#include "flyingZom.h"

flyingZom::flyingZom() {
	name = "FlyingZombie";
	// Set balanced speed
	dx = -0.06;
	dy = 0;
	setHealth(10);
	setDamage(2);
	if (!texture.loadFromFile("Assets\\Zombies\\Ballon Zombie\\ballon zombie (1).png"))
	{
		cout << "Error loading Zombie.png" << endl;

	}
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);
	timer = 0;
	totalIntervals = 42;
}

void flyingZom::setPosition (float x, float y) {
	
	position.x = x ;
	position.y = y - 80;
}

void flyingZom::action(float x) {
	timer += (10*x);
	if (timer > totalIntervals)
	{
		timer = 0;
	}

	move();
	animate(state);
}

void flyingZom::animate(string state ) {
	string path = "Assets\\Zombies\\Ballon Zombie\\ballon zombie (" + to_string((int)((timer * 41.0 / totalIntervals )) + 1) + ").png";
	texture.loadFromFile(path);
	sprite.setTexture(texture);
}

void flyingZom::move() {
	this->position.x += dx;
	this->position.y += dy;

	/*if (position.x < 0) {
		dx = 0.2;
	}
	if (position.x > 1000) {
		dx = -0.2;
	}*/

	if (position.y < 0) {
		dy = 0.1;
	}
	if (position.y > 600) {
		dy = -0.1;
	}
}

void flyingZom::draw(RenderWindow& window) {
	sprite.setPosition(position.x, position.y);
	window.draw(sprite);
}



flyingZom:: ~flyingZom() {

}