#include "DancingZombie.h"
#include "playground.h"

DancingZombie::DancingZombie() {//dancong 
	name = "DancingZombie";
	state = "Moving";
	dx = -1.2;
	dy = 0;
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
	if (state == "Moving")
	{
		if (isFrozen)
		{
			this->position.x += dx / 2.0;
		}
		else
		{
			this->position.x += dx;
		}
	}
}

void DancingZombie::draw(sf::RenderWindow& window) {
	Zombie::draw(window);
}

bool DancingZombie::specialAction() {
	if (lawn == nullptr) return false;

	if (position.x < 850)
	{
		std::cout << "Dancing zombie spawning backup dancers crew!" << std::endl;
		
		// Spawn the 4 backup dancers around the Disco Zombie!
		// 1. Front
		lawn->addZombie(position.x - 100, position.y, "SimpleZombie");
		// 2. Back
		lawn->addZombie(position.x + 100, position.y, "SimpleZombie");
		// 3. Above (if there's a valid lane above)
		if (position.y >= 100) {
			lawn->addZombie(position.x, position.y - 100, "SimpleZombie");
		}
		// 4. Below (if there's a valid lane below)
		if (position.y <= 350) {
			lawn->addZombie(position.x, position.y + 100, "SimpleZombie");
		}
		return true;
	}
	return false;
}

DancingZombie::~DancingZombie() {
}

void DancingZombie::getLawnRef(playground* lawn) {
	this->lawn = lawn;
}
