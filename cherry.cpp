#include "cherry.h"
#include "Explosion.h"
#include <iostream>

Texture cherry::animationTextures[7];
bool cherry::texturesLoaded = false;


cherry::cherry(): time_to_explode(2), cost(150), health(1000), radius(100)
{
	this->name = "cherry";
	this->isAlive = true;
	this->exploded = false;
	this->exploding = false;

	if (!texturesLoaded)
	{
		for (int i = 0; i < 7; i++)
		{
			string path = "Assets/Plants/cherry/cherry_" + to_string(i) + ".png";
			if (!animationTextures[i].loadFromFile(path))
			{
				cout << "Error loading texture: " << path << endl;
			}
		}
		texturesLoaded = true;
	}

	this->sprite.setTexture(animationTextures[0]);
}

 
void cherry::draw(RenderWindow& window)
{
	if (isAlive)
	{
		int frame = (int)(this->timeExploding * 6 / 3);
		if (frame >= 0 && frame < 7)
		{
			this->sprite.setTexture(animationTextures[frame]);
		}

		sprite.setPosition(position.x - 20, position.y);
		window.draw(this->sprite);
	}
}


cherry :: ~cherry()
{
}

bool cherry::isExist()
{
	return isAlive;
}


void cherry::action(float deltaTime, MovingObjectArr* movingObjectArr)
{
	if(!exploded)
	if (exploding == false)
	{
		timeExploding += deltaTime;
		if (this->timeExploding > this->time_to_explode)
		{
			exploding = true;

			timeExploding = 0;

		}
	}

	if (exploding)
	{
		Explosion* explosion = new Explosion(radius);
		explosion->setPosition(position.x,position.y);
		movingObjectArr->push_back(explosion);
			

		exploding = false;
		exploded = true;
		isAlive = false;
	}
	

}