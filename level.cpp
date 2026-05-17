#include "Level.h"
#include "Sun.h"
#include "LawnMower.h"
#include <iostream>

using namespace sf;
using namespace std;

Level::Level(int levelId) 
    : plantShop(levelId), currentLevel(levelId), gameLawn(levelId)
{
    isLevelOver = false;
    livesRemaining = 3;
    isGamePaused = false;
    hasLost = false;
    hasWon = false;
    death = 0.0f; 

    // UI: Pause Button Setup
    texPauseBtn.loadFromFile("Assets\\Background\\pauseButton.png");
    sprPauseBtn.setTexture(texPauseBtn);
    sprPauseBtn.setPosition(1020, 10);
    sprPauseBtn.setScale(2.0f, 2.0f);

    // UI: Pause Screen Overlay Setup
    texPauseScreen.loadFromFile("Assets\\Background\\pauseScreen.png");
    sprPauseScreen.setTexture(texPauseScreen);
    sprPauseScreen.setPosition(50, 0);
    sprPauseScreen.setScale(1.0f, 1.0f);
    
    // UI: Defeat Screen Setup
    texLostScreen.loadFromFile("Assets\\Background\\lost_screen.png");
    sprLostScreen.setTexture(texLostScreen);
    sprLostScreen.setPosition(0, 0);
    sprLostScreen.setOrigin(texLostScreen.getSize().x / 2.0f, texLostScreen.getSize().y / 2.0f);

    // UI: Victory Screen Setup
    texWinScreen.loadFromFile("Assets\\Background\\win.png");
    sprWinScreen.setTexture(texWinScreen);
    sprWinScreen.setPosition(0, 0);
    sprWinScreen.setOrigin(texWinScreen.getSize().x / 2.0f, texWinScreen.getSize().y / 2.0f);

    // UI: Lives Setup
    texLives.loadFromFile("Assets\\Background\\3lives.png");
    sprLives.setTexture(texLives);
    sprLives.setPosition(160, -50);
    sprLives.setScale(0.3f, 0.3f);
}

void Level::drawPause(RenderWindow& window)
{
    window.draw(sprPauseBtn);
    window.draw(sprLives);
    
    // Draw semi-transparent dark overlay if paused or game ended
    if (isGamePaused || hasLost || hasWon)
    {
        RectangleShape darkOverlay(Vector2f(1200, 800));
        darkOverlay.setFillColor(Color(0, 0, 0, 150));
        window.draw(darkOverlay);
    }

    if (isGamePaused && !hasLost && !hasWon)
    {
        window.draw(sprPauseScreen);
    }

    // Handle end-game text
    if (hasLost || hasWon)
    {
        Font burbankFont;
        burbankFont.loadFromFile("Assets\\fonts\\burbank.otf");
        
        Text continueText;
        continueText.setFont(burbankFont);
        continueText.setCharacterSize(50);
        continueText.setFillColor(Color::White);
        continueText.setPosition(550, 450);
        continueText.setString("Press Enter to Continue");
        
        window.draw(continueText);
    }

    // Handle end-game zoom animations
    if (hasLost || hasWon)
    {
        if (death < 1.0f)
        {
            death += 0.01f;
        }

        Sprite& activeEndSprite = hasLost ? sprLostScreen : sprWinScreen;
        
        activeEndSprite.setScale(death, death);
        activeEndSprite.setPosition(550, 300);
        window.draw(activeEndSprite);
    }
}

void Level::drawBackround(RenderWindow& window)
{
    sprBackground.setPosition(0, 0);
    window.draw(sprBackground);
}

void Level::loadLawnMowers()
{
    const int numRows = 5;
    for (int i = 0; i < numRows; i++)
    {
        LawnMower* newMower = new LawnMower();
        newMower->setPosition(160, 70 + (i * 100));
        gameLawn.movingObjectsArray.push_back(newMower);
    }
}

void Level::addSuntoShop()
{
    for (int i = 0; i < gameLawn.movingObjectsArray.getSize(); i++)
    {
        auto* currentObj = gameLawn.movingObjectsArray.at(i);

        // Check if object is an existing Sun
        if (currentObj->getName() == "Sun" && currentObj->isExist())
        {
            Vector2f pos = currentObj->getPosition();
            
            // If Sun reaches the shop icon coordinates, collect it
            bool isAtShopX = (pos.x >= 110 && pos.x <= 114);
            bool isAtShopY = (pos.y >= 25 && pos.y <= 29);

            if (isAtShopX && isAtShopY)
            {
                currentObj->setisAlive(false);
                plantShop.incrementSun(25);
            }
        }
    }
}

void Level::takeInput(RenderWindow& window)
{
    Event event;

    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            window.close();
        }

        // --- MOUSE INPUT ---
        if (event.type == Event::MouseButtonPressed)
        {
            Vector2i mousePos = Mouse::getPosition(window);
            Point gridPos = gameLawn.getGridPosition(mousePos.x, mousePos.y);

            if (event.mouseButton.button == Mouse::Left)
            {
                if (!isGamePaused)
                {
                    bool isWithinGrid = (gridPos.x >= 0 && gridPos.x < 9 && gridPos.y >= 0 && gridPos.y < 5);

                    if (isWithinGrid && plantShop.isPurchasable())
                    {
                        string selectedItem = plantShop.getSelectedPlant();

                        // Handle Tools (Shovel / Water)
                        if (selectedItem == "Shovel")
                        {
                            if (gameLawn.removePlant(gridPos.x, gridPos.y))
                            {
                                plantShop.purchase();
                                plantShop.selectPlant("none");
                            }
                            gameLawn.setBoxAvailable(gridPos.x, gridPos.y, true);
                        }
                        else if (selectedItem == "Water")
                        {
                            if (gameLawn.waterPlant(gridPos.x, gridPos.y))
                            {
                                plantShop.purchase();
                                plantShop.selectPlant("none");
                            }
                        }
                        // Handle Planting
                        else if (gameLawn.isBoxAvailable(gridPos.x, gridPos.y))
                        {
                            float spawnX = gameLawn.getGRID_START_X() + (gridPos.x * gameLawn.getboxWidth());
                            float spawnY = gameLawn.getGRID_START_Y() + (gridPos.y * gameLawn.getboxHeight());
                            
                            gameLawn.addPlant(spawnX, spawnY, selectedItem);
                            gameLawn.setBoxAvailable(gridPos.x, gridPos.y, false);
                            
                            plantShop.purchase();
                            plantShop.selectPlant("none");
                        }
                    }

                    // Update selections based on clicks
                    plantShop.selectPlant(mousePos.x, mousePos.y);
                    gameLawn.selectSun(mousePos.x, mousePos.y);
                }

                // Check Pause Button Click
                if (mousePos.x >= 1035 && mousePos.x <= 1085 && mousePos.y >= 25 && mousePos.y <= 75)
                {
                    isGamePaused = true;
                }
                
                // Check Resume Button Click (Inside Pause Menu)
                if (isGamePaused && mousePos.x >= 400 && mousePos.x <= 680 && mousePos.y >= 435 && mousePos.y <= 475)
                {
                    isGamePaused = false;
                }
            }
            else if (event.mouseButton.button == Mouse::Right)
            {
                cout << "PIXEL : " << mousePos.x << " " << mousePos.y << endl;
                cout << "GRID : " << gridPos.x << " " << gridPos.y << endl;
            }
        }

        // --- KEYBOARD INPUT ---
        if (event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
        {
            if (hasLost || hasWon)
            {
                isLevelOver = true;
            }
        }
    }
}

void Level::checkLoose()
{
    if (gameLawn.checkLost())
    {
        if (livesRemaining <= 1)
        {
            isGamePaused = true;
            hasLost = true;
        }
        else
        {
            livesRemaining--;
            texLives.loadFromFile("Assets\\Background\\" + to_string(livesRemaining) + "lives.png");
            sprLives.setTexture(texLives);
        }
    }
}

void Level::checkWin()
{
    if (gameLawn.checkWin())
    {
        isGamePaused = true;
        hasWon = true;
    }
}

void Level::generateSun(float deltaTime)
{
    // Note: srand(time(0)) was removed from here. 
    // It should only be called once in your main() function.

    sunTimer += deltaTime;
    if (sunTimer >= sunIntervals)
    {
        // Randomize the next interval between 15 and 44 seconds
        sunIntervals = 15 + (rand() % 30);
        sunTimer = 0;

        int randomGridX = rand() % 9;
        int randomGridY = rand() % 5;

        float dropX = gameLawn.getGRID_START_X() + (randomGridX * gameLawn.getboxWidth()) + 10;
        float dropY = gameLawn.getGRID_START_Y() + (randomGridY * gameLawn.getboxHeight()) + 40;

        Sun* fallingSun = new Sun();
        fallingSun->setFallingPosition(dropX, dropY);
        
        gameLawn.movingObjectsArray.push_back(fallingSun);
    }
}

bool Level::getIsEnd()
{
    return isLevelOver;
}

bool Level::getIsWon()
{
    return hasWon;
}

bool Level::getIsLost()
{
    return hasLost;
}