#include "DancingZombie.h"
#include "playground.h"

DancingZombie::DancingZombie() {
	name = "DancingZombie";
	state = "Moving";
	dx = -1.2;
	dy = -1.2;
	setHealth(4);
	setDamage(3);
	if (!texture.loadFromFile("Assets\\Zombies\\dancer zombie\\dancer (1).png"))
	{
		std::cout << "Error loading dancer zombie.png" << std::endl;
	}
	sprite.setTexture(texture);
	sprite.setPosition(0, 0);
	sprite.setScale(0.8, 0.8);
	timer = 0;
	totalIntervals = 14;
	countSpecialAbility = 0;
	specialAbilityTimer = 0.0f;
	lawn = nullptr;
}

void DancingZombie::animate(std::string state) {
	std::string path = "Assets\\Zombies\\dancer zombie\\dancer (" + std::to_string((int)((timer * 13.0 / 14.0)) + 1) + ").png";
	texture.loadFromFile(path);
	sprite.setTexture(texture);
}

void DancingZombie::action(float x) 
{
	timer += (10.0f * x);
	if (timer > totalIntervals)
	{
		timer = 0;
	}

	if (lawn != nullptr && countSpecialAbility < 1)
	{
		specialAbilityTimer += x; // Increment timer by deltaTime
		if (specialAbilityTimer > totalSpecialAbilityTime)
		{
			if (specialAction()) {
				specialAbilityTimer = 0.0f;
				countSpecialAbility++;
			}
		}
	}
	animate(state);
	move();
}

void DancingZombie::move() {
	if (isFrozen)
	{
		this->position.x += dx / 2.0;
		this->position.y += dy / 2.0;
	}
	else
	{
		if (state == "Moving")
		{
			this->position.x += dx;
			this->position.y += dy;
		}
	}
	
	if (position.y < 0) {
		dy = 1.2;
	}
	if (position.y > 450) {
		dy = -1.2;
	}
}

void DancingZombie::draw(sf::RenderWindow& window) {
	Zombie::draw(window);
}

bool DancingZombie::specialAction() {
	if (lawn == nullptr) return false;

	bool isPlant = false;
	if (position.x < 720)
	{
		for (int i = 0; i < lawn->getPlantsArray().getSize(); i++)
		{
			if (lawn->getPlantsArray().at(i) != nullptr)
			{
				Point plantPos = lawn->getPlantsArray().at(i)->getPosition();
				if (abs(plantPos.y - position.y) < 50)
				{
					isPlant = true;
					return false; // Don't spawn backup dancers if blocked by a plant
				}
			}
		}
		std::cout << "Dancing zombie spawning backup dancer at: " << position.x << ", " << position.y << std::endl;
		lawn->addZombie(position.x - 100, position.y, "SimpleZombie");
		return true;
	}
	return false;
}

DancingZombie::~DancingZombie() {
}

void DancingZombie::getLawnRef(playground* lawn) {
	this->lawn = lawn;
}
