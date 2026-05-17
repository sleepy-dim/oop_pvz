#ifndef Card_h
#define	Card_h
#include <iostream>
#include <string>
#include "Point.h"
#include <SFML/Graphics.hpp>
using namespace sf;
using namespace std;

class Card
{
private:
	Point position;
	string name;
	int cost;
	int coolDown;
	Texture texture;
	Sprite sprite;
	static Font font;
	static bool assetsLoaded;

public:
	Card(string name, int cost, int coolDown, Point P);
	void draw(RenderWindow& window, string slected);

	int getCost();
	int getCoolDown();
	string getName();
};

#endif 