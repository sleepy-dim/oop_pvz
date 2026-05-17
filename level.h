#ifndef Level_h
#define Level_h

#pragma once
#include "playground.h"
#include "Shop.h"
#include <string>

#include <SFML/Graphics.hpp>
using namespace sf;
class Level
{
protected:
	
	Shop shop;
	playground lawn;
	int levelNumber;
	float last_screen;
	Texture backgroundTexture;
	Sprite backgroundSprite;
	//int livesRemaining;
	//bool is_animation;
	void drawBackround(RenderWindow& window);
	//void animation(RenderWindow &)  ;
	//virtual void loadZombies() ;
	int sunIntervals = 10;
	float sunTimer = 0;
	Texture lostTexture;
	Sprite lostSprite;
	Texture winTexture;
	Sprite winSprite;

	Texture pauseTexture;
	Sprite pauseSprite;
	Texture pauseScreenTexture;
	Sprite pauseScreenSprite;
	int livesRemaining;


	Texture livesTexture;
	Sprite livesSprite;

	bool isPaused = false;
	bool hasLost = false;
	bool hasWon = false;
	bool isEnd = false;

public:
	
	Level(int levelNumber);

	virtual void loadLawnMowers() ;

	virtual void draw(RenderWindow& window) = 0;
	virtual void update(float) = 0;
	virtual void takeInput(RenderWindow& window) ;
	virtual void zombieSpawn() = 0;
	void addSuntoShop(); 
	void generateSun(float);
	void drawPause (RenderWindow& window);
	void checkLoose();
	void checkWin();
	bool getIsEnd();	
	bool getIsWon();
	bool getIsLost();

	float timeSurvived = 0;
	void saveProgress();
	void loadProgress();
	int getScore();
	//virtual void checkCollisions() = 0 ;
	//virtual void checkWin() = 0 ;
	//virtual void checkLose() = 0;
	//virtual void takeInput(RenderWindow& window) = 0 ;
};

#endif 