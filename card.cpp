#include "Card.h"
#include <iostream>
#include <algorithm>
#include <cctype>

Font* Card::font = nullptr;
bool Card::assetsLoaded = false;

Card::Card(string name, int cost, int coolDown, coordinate P, bool isUnlocked)
	: position(P), name(name), cost(cost), coolDown(coolDown), isUnlocked(isUnlocked)
{
	if (!assetsLoaded)
	{
		font = new Font();
		if (!font->loadFromFile("Assets/fonts/burbank.otf"))
			cout << "Error loading font" << endl;
		assetsLoaded = true;
	}

	// Convert name to lowercase for file lookup
	string nameLower = name;
	for (char& c : nameLower) {
		c = tolower(c);
	}

	if (!texture.loadFromFile("Assets/Inventory-GameScreen/Cards/card_" + nameLower + "_move.png"))
		cout << "Error loading card texture for: " << name << endl;
	
	sprite.setTexture(texture);
	sprite.setScale(1, 0.8);
	if (name == "Shovel" || name == "Water")
	{
		sprite.setScale(0.7, 0.7);
		position.x += 10;
		position.y += 4;
	}
    // Ensure sprite position is set so global bounds are correct
    sprite.setPosition(position.x, position.y);
}


void Card::draw(RenderWindow& window, string slcted)
{
	if (name == slcted && isUnlocked)
	{
		RectangleShape rectangle(Vector2f(64, 72));
		rectangle.setFillColor(Color::Transparent);
		rectangle.setOutlineThickness(3);
		rectangle.setOutlineColor(Color::Yellow);
		rectangle.setPosition(position.x, position.y);
		window.draw(rectangle);
	}

	Text text;
	text.setFont(*font);
	text.setString(to_string(cost));
	text.setCharacterSize(16);
	
	if (isUnlocked)
	{
		text.setFillColor(Color::Black);
		sprite.setColor(Color(255, 255, 255));
	}
	else
	{
		text.setFillColor(Color(50, 50, 50)); // Dimmed text
		sprite.setColor(Color(80, 80, 80));   // Darker dimmed sprite
	}

	sprite.setPosition(position.x, position.y);
	window.draw(sprite);
	window.draw(text);

	if (!isUnlocked)
	{
		// Draw semi-transparent black overlay for locked cards
		RectangleShape overlay(Vector2f(64, 72));
		overlay.setFillColor(Color(0, 0, 0, 180)); // More opaque black
		overlay.setPosition(position.x, position.y);
		window.draw(overlay);
	}
}

bool Card::contains(float x, float y)
{
    // Ensure sprite position is consistent
    sprite.setPosition(position.x, position.y);
    FloatRect bounds = sprite.getGlobalBounds();
    return bounds.contains(x, y);
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

bool Card::getIsUnlocked()
{
	return isUnlocked;
}