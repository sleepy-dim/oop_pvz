#include "garden.h"
#include "playground.h"
#include "Pea.h"


garden::garden(int level) : Level(level)
{
	srand(time(0));
	this->levelNumber = level;

	string path = "";
	if (level == 3)
	{
		path = "Assets/Background/bgnight.jpg";
	}
	else
	{
		path = "Assets/Level/" + to_string(level) + ".png";
	}

	if (!backgroundTexture.loadFromFile(path))
	{
		cout << "Error loading background image: " << path << endl;
	}
	backgroundSprite.setTexture(backgroundTexture);

	zombieTimer = 0.0f;
	// Start with a reasonably long interval so zombies come slowly at first
	zombieSpawnInterval = 10.0f;
	// Do not spawn a big wave at start — spawn will occur periodically one-by-one
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
				// Limit how many zombies can be active at once to avoid big waves
				int maxConcurrent = 6;
				if (lawn.zombieArray.getSize() < maxConcurrent) {
					// Slow, gradual escalation: decrease interval slowly (min 5 seconds)
					if (zombieSpawnInterval > 5.0f)
					{
						zombieSpawnInterval -= 0.2f;
					}

					// Spawn a single random zombie: SimpleZombie, DancingZombie, or FlyingZombie
					int roll = rand() % 3;
					string zombieType = "SimpleZombie";
					if (roll == 1) zombieType = "DancingZombie";
					else if (roll == 2 && levelNumber >= 2) zombieType = "FlyingZombie";
					else if (roll == 2 && levelNumber < 2) zombieType = "SimpleZombie"; // Fallback for level 1
					
					lawn.addZombie(1100, rand() % 5 * 100 + 20, zombieType);
				} else {
					// Too many zombies; wait a bit before trying again
					// Keep interval unchanged so spawning paces out
				}
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

			// AI State Reporting
			aiTimer += deltaTime;
			if (aiTimer >= 10.0f) {
				cout << "GAME_STATE: Sun=" << shop.getSun() 
					 << ", ZombieCount=" << lawn.zombieArray.getSize() << endl;
				cout.flush(); // Ensure output is sent
				aiTimer = 0.0f;
			}
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
	// Spawn a single starter zombie (one-by-one behavior)
	int roll = rand() % 2;
	string zombieType = "SimpleZombie";
	if (roll == 1) zombieType = "DancingZombie";
	lawn.addZombie(1000, rand() % 5 * 100 + 20, zombieType);
}




