#include "frozenPea.h"
#include "frozenPeaBullet.h"

frozenPea::frozenPea()
{
	cost = 150;
	damage = 20;
	cost = 100;
	health = 1000;
	damage = 10;

	bulletInterval = 4;
	bulletTimer = 0;
	texture.loadFromFile("Assets\\Plants\\SnowPea\\SnowPea_0.png");
	sprite.setTexture(texture);
}

void frozenPea::shoot(float deltaTime, MovingObjectArr* movingObjectArr)
{
	bulletTimer += deltaTime;
	if (bulletTimer >= bulletInterval) {
		bulletTimer = 0;
		ammo* bullet = new frozenPeaBullet();
		bullet->setPosition(position.x + 50, position.y);
		movingObjectArr->push_back(bullet);
	}
}


frozenPea ::~frozenPea()
{
}

void frozenPea::draw(sf::RenderWindow& window)
{
	string path = "Assets\\Plants\\SnowPea\\SnowPea_" + to_string((int)(bulletTimer *(15.0/4.0))) + ".png";
	texture.loadFromFile(path);
	sprite.setTexture(texture);
	sprite.setPosition(position.x , position.y);
	window.draw(sprite);

}