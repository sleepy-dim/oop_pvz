#include "garden.h"
#include "playground.h"
#include "Pea.h"


garden::garden(int level) : Level(level)
{
	srand(time(0));
	this->levelNumber = level;

	string path = "Assets/Level/" + to_string(level) + ".png";
	if (!backgroundTexture.loadFromFile(path))
	{
		cout << "Error loading background image for Level " << level << endl;
	}
	backgroundSprite.setTexture(backgroundTexture);

	zombieTimer = 0.0f;
	zombieSpawnInterval = 10.0f;
	this->zombieSpawn();
	this->loadLawnMowers();
}

void garden::draw(RenderWindow& window)
{
	drawBackround(window);
	shop.draw(window);
	lawn.drawPlants(window);
	lawn.drawZombie(window);
	lawn.drawMovingObjects(window);
	drawPause(window);
	

}

void garden::update(float deltaTime)
{
	
		if (!isPaused)
		{
			timeSurvived += deltaTime;

			// Handle periodic, escalating zombie spawning
			zombieTimer += deltaTime;
			if (zombieTimer >= zombieSpawnInterval)
			{
				zombieTimer = 0.0f;
				// Escalate: decrease interval slightly every spawn (min 4 seconds)
				if (zombieSpawnInterval > 4.0f)
				{
					zombieSpawnInterval -= 0.5f;
				}

				// Spawn a random zombie: SimpleZombie or FlyingZombie
				string zombieType = (rand() % 2 == 0) ? "SimpleZombie" : "FlyingZombie";
				lawn.addZombie(1100, rand() % 5 * 100 + 20, zombieType);
			}

			lawn.updateMovingObjects(deltaTime);
			lawn.updatePlants(deltaTime, &(lawn.movingObjectsArray));
			lawn.updateZombie(deltaTime);
			generateSun(deltaTime);
			addSuntoShop();
			lawn.removeDead();
			lawn.collisionAmmo_Zombie();
			lawn.collisionPlant_Zombie();
			checkLoose();
			checkWin();
		}
}
	
void garden::loadLawnMowers()
{
	Level::loadLawnMowers();
}



void garden::input(RenderWindow& window)
{
	Level :: takeInput(window);
}


void garden::zombieSpawn() {
	for (int i = 0; i < 10; i++)
	{
		string zombieType = (rand() % 2 == 0) ? "SimpleZombie" : "FlyingZombie";
		lawn.addZombie(1000 + i * 300, rand() % 5 * 100 + 20, zombieType);
	}
}




