#include "Card.h"
#include <iostream>

Font Card::font;
bool Card::assetsLoaded = false;

Card::Card(string name, int cost, int coolDown, Point P, bool isUnlocked)
	: position(P), name(name), cost(cost), coolDown(coolDown), isUnlocked(isUnlocked)
{
	if (!assetsLoaded)
	{
		if (!font.loadFromFile("Assets/fonts/burbank.otf"))
			cout << "Error loading font" << endl;
		assetsLoaded = true;
	}

	if (!texture.loadFromFile("Assets/Inventory-GameScreen/Cards/card_" + name + "_move.png"))
		cout << "Error loading card texture for: " << name << endl;
	
	sprite.setTexture(texture);
	sprite.setScale(1, 0.8);
	if (name == "Shovel" || name == "Water")
	{
		sprite.setScale(0.7, 0.7);
		position.x += 10;
		position.y += 4;
	}
}


void Card::draw(RenderWindow& window, string slcted)
{
	if (name == slcted)
	{
		RectangleShape rectangle(Vector2f(64, 72));
		rectangle.setFillColor(Color::Transparent);
		rectangle.setOutlineThickness(3);
		rectangle.setOutlineColor(Color::Yellow);
		rectangle.setPosition(position.x, position.y);
		window.draw(rectangle);
	}

	Text text;
	text.setFont(font);
	text.setString(to_string(cost));
	text.setCharacterSize(16);
	text.setFillColor(Color::Black);
	text.setPosition(position.x + 20, position.y + 52);

	sprite.setColor(Color(255, 255, 255));
	sprite.setPosition(position.x, position.y);
	window.draw(sprite);
	window.draw(text);
}

int Card::getCost()
{
	return cost;
}

int Card::getCoolDown()
{
	return coolDown;
}

string Card::getName()
{
	return name;
}