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
		lawn.addZombie(1000 + i * 300, rand() % 5 * 100 + 20, "SimpleZombie");
	}
}




