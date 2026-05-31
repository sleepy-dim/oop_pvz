#include "Shop.h"




Shop::Shop(int level)
{
	this->level = level;
	extern bool g_isUnlimited;
	sun = (level == 1 && !g_isUnlimited) ? 200 : 1000;

	// In Level 1, only Shooter and basic cards are unlocked.
	// In Level 2 and 3, everything is unlocked.
	bool level1Unlocks = (level == 1 && !g_isUnlimited);
	bool level2Unlocks = (level >= 2);
	bool level3Unlocks = (level >= 3);

	listOfCards[0] = new Card("SunFlower", 50, 5, coordinate(25, 25), true);
	listOfCards[1] = new Card("PeaShooter", 100, 5, coordinate(25, 105), true);
	listOfCards[2] = new Card("WallNut", 50, 5, coordinate(25, 185), !level1Unlocks); 
	listOfCards[3] = new Card("CherryBomb", 150, 5, coordinate(25, 265), !level1Unlocks);
	listOfCards[4] = new Card("Cactus", 100, 5, coordinate(25, 345), !level1Unlocks);
	listOfCards[5] = new Card("Repeater", 200, 5, coordinate(25, 425), !level1Unlocks);
	listOfCards[6] = new Card("FrozenPeaShooter", 175, 5, coordinate(25, 505), !level1Unlocks);
	listOfCards[7] = new Card("Shovel", 0, 0, coordinate(90 , 115), true);
	listOfCards[8] = new Card("Water", 50, 5, coordinate(90 , 190), true);
	selectedPlant = nullptr;

    if (!sunBankTexture.loadFromFile("Assets/Inventory-GameScreen/SunBank.png"))
        cout << "Error loading SunBank.png" << endl;
    if (!shovelBankTexture.loadFromFile("Assets/Inventory-GameScreen/ShovelBank.png"))
        cout << "Error loading ShovelBank.png" << endl;
    if (!font.loadFromFile("Assets/fonts/burbank.otf"))
        cout << "Error loading font" << endl;
}

void Shop::draw(RenderWindow & window)
{
	Sprite sunBankSprite;
	sunBankSprite.setTexture(sunBankTexture);
	sunBankSprite.setPosition(97, 22);
	window.draw(sunBankSprite);

	Sprite shovelBankSprite;
	shovelBankSprite.setTexture(shovelBankTexture);	
	shovelBankSprite.setPosition(97, 120);
	shovelBankSprite.setScale(1,1);
	window.draw(shovelBankSprite);

	shovelBankSprite.setPosition(97 , 195 );
	window.draw(shovelBankSprite);

	Text text;
	text.setFont(font);
	text.setString( to_string(sun));
	text.setCharacterSize(24);
	text.setFillColor(Color::Black);
	text.setPosition(115,80);
	window.draw(text);

	for (int i = 0; i < 9 ; i++)
	{
		string name = "";
		if (selectedPlant != nullptr)
			name = selectedPlant->getName();
			
		 listOfCards[i]->draw(window, name );
	}
}

//void Shop::update()
//{
//	for (int i = 0; i < noOfPlants; i++)
//	{
//		if (coolDown[i].getElapsedTime().asSeconds() > 5)
//		{
//			coolDown[i].restart();
//		}
//	}
//}
void Shop::selectPlant(string name)
{
	if (name == "none")
	{
		selectedPlant = nullptr;
		return;
	}
	for (int i = 0; i < 9; i++)
	{
		if (listOfCards[i] != nullptr && listOfCards[i]->getName() == name)
		{
			if (listOfCards[i]->getIsUnlocked())
			{
				selectedPlant = listOfCards[i];
				return;
			}
			else
			{
				cout << "[DEBUG] selectPlant(string): Card " << name << " is locked!\n";
				selectedPlant = nullptr;
				return;
			}
		}
	}
}

void Shop::selectPlant(float x, float y)
{
	Card* clicked = nullptr;
	for (int i = 0; i < 9; ++i)
	{
		if (listOfCards[i] != nullptr && listOfCards[i]->contains(x, y))
		{
			clicked = listOfCards[i];
			break;
		}
	}

	if (clicked != nullptr)
	{
		if (clicked == this->selectedPlant)
		{
			this->selectedPlant = nullptr;
			cout << "[DEBUG] Deselected: " << clicked->getName() << "\n";
		}
		else if (clicked->getIsUnlocked())
		{
			this->selectedPlant = clicked;
			cout << "[DEBUG] Selected: " << this->selectedPlant->getName() << "\n";
		}
		else
		{
			cout << "[DEBUG] Cannot select: " << clicked->getName() << " (Locked)\n";
			this->selectedPlant = nullptr; // Deselect current if clicking a locked card
		}
	}
	// If clicking outside cards, don't change selection (allows planting)
}

void Shop::setSun(int s)
{
	sun = s;
}

int Shop::getSun()
{
	return sun;
}

bool Shop::isPurchasable()
{
	if (selectedPlant != nullptr)
		return sun >= selectedPlant->getCost();
	return false;
}

void Shop::purchase()
{
	if (selectedPlant != nullptr)
	{
		sun -= selectedPlant->getCost();
	}
}

string Shop::getSelectedPlant()
{
	if (selectedPlant != nullptr)
		return selectedPlant->getName();
	return "";
}

void Shop::incrementSun(int amount)
{
	sun += amount;
}