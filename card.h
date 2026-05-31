#ifndef Card_h
#define	Card_h
#include <iostream>
#include <string>
#include "coordinate.h"
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

class Card
{
private:
	coordinate position;
	string name;
	int cost;
	int coolDown;
	Texture texture;
	Sprite sprite;
	static Font* font;
	static bool assetsLoaded;
	bool isUnlocked;

public:
	Card(string name, int cost, int coolDown, coordinate P, bool isUnlocked = true);
	void draw(RenderWindow& window, string slected);

	// Return true if the given window coordinates fall inside this card's sprite
	bool contains(float x, float y);

	int getCost();
	int getCoolDown();
	string getName();
	bool getIsUnlocked();
};

#endif 