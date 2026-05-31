#include "playground.h"
#include "LawnMower.h"
#include "DancingZombie.h"
#include <cmath>
playground::playground(int level)
{
	m_plantGen = plantGen::getInstance();
	m_zombieGen = zombieGen::getInstance();
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			available[i][j] = true;
		}
	}

	GRID_START_X = 255;
	GRID_START_Y = 75;
	BoxWidth = 80 ;
	BoxHeight = 100 ;

}



void playground::selectSun(float x, float y)
{
	for (int i = 0; i < movingObjectsArray.getSize(); i++)
	{

		if (movingObjectsArray.at(i) != nullptr)
		{
			if (movingObjectsArray.at(i)->getName() == "Sun")
				movingObjectsArray.at(i)->update(x, y);
		}

	}
}


void playground::updateZombie(float deltaTime) {

	for (int i = 0; i < zombieArray.getSize(); i++) 
	{
		if (zombieArray.at(i) != nullptr) 
		{
			zombieArray.at(i)->action(deltaTime);
			
		}
	}
}

void playground::drawMovingObjects(RenderWindow& window)
{
	for (int i = 0; i < movingObjectsArray.getSize(); i++)
	{
		
		if (movingObjectsArray.at(i) != nullptr)
		{
			movingObjectsArray.at(i)->draw(window);
		}
	}
}


void playground::addZombie(float x, float y, string type) {

	Zombie* zombie = m_zombieGen->createZombie(type);

	if(zombie != nullptr)
	{
		if (type == "DancingZombie")
		{
			((DancingZombie*)zombie)->getLawnRef(this);
		}
		zombie->setPosition(x, y);
		zombieArray.push_back(zombie);
	}
}


void playground::drawZombie(RenderWindow& window)
{
	for (int i = 0; i < zombieArray.getSize(); i++)
	{
		if (zombieArray.at(i) != nullptr)
		zombieArray.at(i)->draw(window);
	}
}
void playground::addPlant(float x, float y, string type )   ///////////Might need to change the type to string
{
	Plant* plant = 	m_plantGen->createPlant(type);
	if(plant != nullptr)
	{
		plant->setName(type);
		plant->setPosition(x, y);
		plantsArray.push_back(plant);
		cout << "[DEBUG] Added plant: " << type << " at (" << x << ", " << y << ")\n";
	}
}

void playground::drawPlants(RenderWindow& window)
{
	for (int i = 0; i < plantsArray.getSize(); i++)
	{
		if (plantsArray.at(i) != nullptr)
		plantsArray.at(i)->draw(window);
	}
}

coordinate playground::getGridPosition(float x, float y) {
	coordinate p;
	float relativeX = x - GRID_START_X;
	float relativeY = y - GRID_START_Y;

	p.x = (int)((relativeX / BoxWidth));
	p.y = (int)((relativeY / BoxHeight));
	if (relativeX < 0 || relativeY < 0)
	{
		p.x = -1;
		p.y = -1;
		return p;
	}
	p.x = (int)((relativeX / BoxWidth));
	p.y = (int)((relativeY / BoxHeight));

	return p;
}

int playground::getboxHeight()
{
	return BoxHeight;
}

int playground::getboxWidth()
{
	return BoxWidth;
}



int playground::getGRID_START_Y()
{
	return GRID_START_Y;
}

int playground::getGRID_START_X()
{
	return GRID_START_X;
}

void playground ::setBoxAvailable(int x, int y, bool value)
{
	available[y][x] = value;
}

PlantArr& playground::getPlantsArray()
{
	return plantsArray;
}


bool playground::isBoxAvailable(int x, int y)
{
	return available[y][x];
}


void playground::updateMovingObjects(float deltaTime)
{
	for (int i = 0; i < movingObjectsArray.getSize(); i++)
		if(movingObjectsArray.at(i) != nullptr)
		movingObjectsArray.at(i)->move(deltaTime);
}

void playground::updatePlants (float time, MovingObjectArr* movingObjectsArray)
{
	for (int i = 0; i < plantsArray.getSize(); i++)
	{
		if(plantsArray.at(i) != nullptr)
		{
			bool shouldAction = true;
			string name = plantsArray.at(i)->getName();
			if (name == "PeaShooter" || name == "FrozenPeaShooter" || name == "Repeater")
			{
				shouldAction = false;
				float plantY = plantsArray.at(i)->getPosition().y;
				float plantX = plantsArray.at(i)->getPosition().x;
				
				for (int j = 0; j < zombieArray.getSize(); j++)
				{
					if (zombieArray.at(j) != nullptr)
					{
						float zombieY = zombieArray.at(j)->getPosition().y;
						float zombieX = zombieArray.at(j)->getPosition().x;
						
						// More robust lane detection using a slightly larger window (75px)
						// and checking if the zombie is to the right of the plant.
						if (abs(zombieY - plantY) < 75 && zombieX >= plantX - 50)
						{
							shouldAction = true;
							break;
						}
					}
				}
			}
			
			if (shouldAction)
			{
				plantsArray.at(i)->action(time, movingObjectsArray);
			}
		}
	}
}


//
//void Lawn::drawMovingObjects(RenderWindow& window)
//{
//	for (int i = 0; i < movingObjectsArray.getSize(); i++)
//	{
//		movingObjectsArray.at(i)->draw(window);
//	}
//}
//




void playground::removeDead()
{
	for (int i = 0; i < plantsArray.getSize(); i++)
	{
		if(plantsArray.at(i) != nullptr)
		if (plantsArray.at(i)->getHealth() <= 0 || plantsArray.at(i)->isExist() == false)
		{
			coordinate p = plantsArray.at(i)->getPosition();
			coordinate P = getGridPosition(p.x, p.y);
			setBoxAvailable(P.x, P.y, true);

			plantsArray.erase(i);
			i--;
		}
	}
	for (int i = 0; i < zombieArray.getSize(); i++)
	{
		if(zombieArray.at(i) != nullptr)
		if (zombieArray.at(i)->getHealth() <= 0 || zombieArray.at(i)->isExist() == false)
		{
			if (zombieArray.at(i)->getHealth() <= 0 )
			{
				Zombie :: killCount++;
			}
			zombieArray.erase(i);
			i--;
		}
	}
	for (int i = 0; i < movingObjectsArray.getSize(); i++)
	{
		if(movingObjectsArray.at(i) != nullptr)
		if (movingObjectsArray.at(i)->isExist() == false)
		{
			movingObjectsArray.erase(i);
			i--;
		}
	}
}





bool playground::checkLost()
{
	for (int i = 0; i < zombieArray.getSize(); i++)
	{
		if (zombieArray.at(i) != nullptr)
		{
			if (zombieArray.at(i)->getPosition().x  <= 80)
			{
				zombieArray.erase(i);
				return true;
			}
		}
	}
	return false;
}

bool playground::checkWin()
{

	for (int i = 0; i < zombieArray.getSize(); i++)
	{
		if (zombieArray.at(i) != nullptr )
		{
			return false;
		}
	}
	
	return true ;
}




void playground::collisionAmmo_Zombie()
{
	for (int i = 0; i < movingObjectsArray.getSize(); i++)
	{
		for (int j = 0; j < zombieArray.getSize(); j++)
		{
			if (movingObjectsArray.at(i) != nullptr && zombieArray.at(j) != nullptr)
			{
				coordinate M = movingObjectsArray.at(i)->getPosition();
				coordinate Z = zombieArray.at(j)->getPosition();

				bool isFlying = (zombieArray.at(j)->getName() == "FlyingZombie");

				if (movingObjectsArray.at(i)->getType() == "ammo")
				{
					bool hit = false;
					if (isFlying)
					{
						// Flying zombies are higher, so we check a much higher and broader collision box
						// Balloon zombies fly high, so we check from Z.y - 100 up to Z.y + 100
						if (M.x >= Z.x + 30 && M.x <= Z.x + 170 && M.y >= Z.y - 100 && M.y <= Z.y + 120)
						{
							hit = true;
						}
					}
					else if (M.x >= Z.x + 50 && M.x <= Z.x + 150 && M.y >= Z.y && M.y <= Z.y + 110)
					{
						hit = true;
					}

					if (hit)
					{
						if (movingObjectsArray.at(i)->getName() == "Pea")
							zombieArray.at(j)->setHealth(zombieArray.at(j)->getHealth() - ((ammo*)movingObjectsArray.at(i))->getDamage());
						else if (movingObjectsArray.at(i)->getName() == "FrozenPea" || movingObjectsArray.at(i)->getName() == "frozenPea")
						{
							zombieArray.at(j)->setHealth(zombieArray.at(j)->getHealth() - ((ammo*)movingObjectsArray.at(i))->getDamage());
							zombieArray.at(j)->setIsFrozen(true);
						}
						movingObjectsArray.erase(i);
						i--;
						break; // Break inner loop as ammo is destroyed
					}
				}
				else if (movingObjectsArray.at(i)->getType() == "LawnMower")
				{
					if (M.x >= Z.x - 20 && M.x <= Z.x + 20 && M.y >= Z.y + 25 && M.y <= Z.y + 100)
					{
						cout << "LawnMower" << endl;
						if (!((LawnMower*)movingObjectsArray.at(i))->getIsMoving())
						{
							((LawnMower*)movingObjectsArray.at(i))->setIsMoving(true);
						}
						zombieArray.at(j)->setHealth(0);
					}
				}
				else if (movingObjectsArray.at(i)->getType() == "Explosion")
				{
					//coordinate Z(zombieArray.at(j)->getPosition().x + ( zombieArray.at(j)->getTexture().getSize().x /2 ), zombieArray.at(j)->getPosition().y + (zombieArray.at(j)->getTexture().getSize().y / 2));
					//coordinate M(movingObjectsArray.at(i)->getPosition().x + ((movingObjectsArray.at(i)->getTexture().getSize().x) / 2.0), movingObjectsArray.at(i)->getPosition().y + ((movingObjectsArray.at(i)->getTexture().getSize().y) / 2.0 ));

					float radius  = sqrt(pow(M.x - Z.x, 2) + pow(M.y - Z.y, 2));
					if (radius < 200)
					{
						zombieArray.at(j)->setBurnt();
					}	
				}
			}
		}
	}
}


void playground::collisionPlant_Zombie()
{
	for (int i = 0; i < plantsArray.getSize(); i++)
	{
		for (int j = 0; j < zombieArray.getSize(); j++)
		{
			if(plantsArray.at(i) != nullptr && zombieArray.at(j) != nullptr)
			{
				if (plantsArray.at(i)->isExist() && zombieArray.at(j)->isExist())
				{
					if (zombieArray.at(j)->getName() == "FlyingZombie" && plantsArray.at(i)->getName() != "Cactus")
					{
						continue;
					}

					coordinate P = plantsArray.at(i)->getPosition();
					coordinate gridPos = getGridPosition(P.x, P.y);
					if (gridPos.x == 0)
					{
						// Zombies bypass plants on the first tile (column 0)
						continue;
					}

					coordinate Z = zombieArray.at(j)->getPosition();
					
					// Calculate approximate center of the zombie for a more accurate radius check
					coordinate zombieCenter(Z.x + 85, Z.y + 70);

					// Using the 'distance' friend function from coordinate.h to detect collision!
					if (distance(P, zombieCenter) < 80)
					{
						if (zombieArray.at(j)->getName() == "FlyingZombie" && plantsArray.at(i)->getName() == "Cactus")
						{
							zombieArray.at(j)->setHealth(0);
							continue;
						}
						zombieArray.at(j)->setState("Attacking");
						
						// Implement bite rate: damage is only applied once every 1.0 second!
						if (zombieArray.at(j)->getAttackTimer() >= 1.0f)
						{
							// Scale the bite damage by 15x to balance with the discrete bite rate
							plantsArray.at(i)->setHealth(plantsArray.at(i)->getHealth() - (zombieArray.at(j)->getDamage() * 15));
							zombieArray.at(j)->setAttackTimer(0.0f); // Reset bite timer
						}

						if (plantsArray.at(i)->getHealth() <= 0)
						{
							plantsArray.erase(i);
							i--;
							zombieArray.at(j)->setState("Moving");
							zombieArray.at(j)->setAttackTimer(1.0f); // Ready to bite next target immediately!
						}
					}
					else
					{
						zombieArray.at(j)->setState("Moving");
						// Cap attack timer to 1.0f so it is ready to bite immediately on next collision
						if (zombieArray.at(j)->getAttackTimer() > 1.0f)
						{
							zombieArray.at(j)->setAttackTimer(1.0f);
						}
					}

					
				}
			}
		}
	}
}

playground :: ~playground()
{
	
}	






bool playground::removePlant(float x, float y)
{
	for (int i = 0; i < plantsArray.getSize(); i++)
	{
		int  x_ = getGridPosition(plantsArray.at(i)->getPosition().x, plantsArray.at(i)->getPosition().y).x;
		int  y_ = getGridPosition(plantsArray.at(i)->getPosition().x, plantsArray.at(i)->getPosition().y).y;
		

		if (x_ == x && y_ == y)
		{
			cout << "Removing Plant" << endl;
			plantsArray.erase(i);
			setBoxAvailable(x, y, true);
			return 1;
		
		}
	}
	return 0;
}


bool playground::waterPlant(float x, float y)
{
	for (int i = 0; i < plantsArray.getSize(); i++)
	{
		int  ax = getGridPosition(plantsArray.at(i)->getPosition().x, plantsArray.at(i)->getPosition().y).x;
		int  ay = getGridPosition(plantsArray.at(i)->getPosition().x, plantsArray.at(i)->getPosition().y).y;


		if (ax == x && ay == y)
		{
			cout << "Watering Plant" << endl;
			plantsArray.at(i)->setHealth(plantsArray.at(i)->getHealth() + 1000);
			return 1;
		}
	}
	return 0;
}