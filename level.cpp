#include "Level.h"
#include "Sun.h"
#include "LawnMower.h"
#include "Zombie.h"
#include <iostream>
#include <fstream>
#include <mutex>
#include <string>

using namespace sf;
using namespace std;

Level::Level(int levelId) 
    : shop(levelId), levelNumber(levelId), lawn(levelId)
{
    isEnd = false;
    livesRemaining = 3;
    isPaused = false;
    hasLost = false;
    hasWon = false;
    last_screen = 0.0f; 

    // UI: Pause Button Setup
    pauseTexture.loadFromFile("Assets\\Background\\pauseButton.png");
    pauseSprite.setTexture(pauseTexture);
    pauseSprite.setPosition(1020, 10);
    pauseSprite.setScale(2.0f, 2.0f);

    // UI: Pause Screen Overlay Setup
    pauseScreenTexture.loadFromFile("Assets\\Background\\pauseScreen.png");
    pauseScreenSprite.setTexture(pauseScreenTexture);
    pauseScreenSprite.setPosition(50, 0);
    pauseScreenSprite.setScale(1.0f, 1.0f);
    
    // UI: Defeat Screen Setup
    lostTexture.loadFromFile("Assets\\Background\\lost_screen.png");
    lostSprite.setTexture(lostTexture);
    lostSprite.setPosition(0, 0);
    lostSprite.setOrigin(lostTexture.getSize().x / 2.0f, lostTexture.getSize().y / 2.0f);

    // UI: Victory Screen Setup
    winTexture.loadFromFile("Assets\\Background\\win.png");
    winSprite.setTexture(winTexture);
    winSprite.setPosition(0, 0);
    winSprite.setOrigin(winTexture.getSize().x / 2.0f, winTexture.getSize().y / 2.0f);

    // UI: Lives Setup
    livesTexture.loadFromFile("Assets\\Background\\3lives.png");
    livesSprite.setTexture(livesTexture);
    livesSprite.setPosition(160, -50);
    livesSprite.setScale(0.3f, 0.3f);
}

void Level::drawPause(RenderWindow& window)
{
    window.draw(pauseSprite);
    window.draw(livesSprite);

    // Draw Crazy Dave AI Advice Box on the Top Right of the Screen!
    if (!isPaused && !hasLost && !hasWon)
    {
        extern std::string g_aiAdvice;
        extern std::mutex g_adviceMutex;

        std::string adviceText;
        {
            std::lock_guard<std::mutex> lock(g_adviceMutex);
            adviceText = g_aiAdvice;
        }

        // 1. Draw Background Wood Card
        RectangleShape bgBox(Vector2f(380.f, 130.f));
        bgBox.setPosition(800.f, 15.f);
        bgBox.setFillColor(Color(35, 25, 20, 220)); // Wooden brown
        bgBox.setOutlineThickness(3.f);
        bgBox.setOutlineColor(Color(218, 165, 32)); // Golden border
        window.draw(bgBox);

        // 2. Draw Crazy Dave Portrait
        Texture daveTex;
        if (daveTex.loadFromFile("Assets/Images/crazydave.png"))
        {
            Sprite daveSprite(daveTex);
            daveSprite.setPosition(810.f, 25.f);
            
            // The user's uploaded image has different dimensions, let's scale it to fit nicely inside the 110x110 box
            FloatRect bounds = daveSprite.getLocalBounds();
            float scaleX = 100.f / bounds.width;
            float scaleY = 110.f / bounds.height;
            daveSprite.setScale(scaleX, scaleY);
            
            window.draw(daveSprite);
        }

        // 3. Draw Burbank Font Texts
        Font font;
        if (font.loadFromFile("Assets/fonts/burbank.otf"))
        {
            Text title;
            title.setFont(font);
            title.setCharacterSize(16);
            title.setFillColor(Color(255, 69, 0)); // Orange-Red
            title.setString("CRAZY DAVE'S ADVICE:");
            title.setPosition(925.f, 22.f);
            window.draw(title);

            Text advice;
            advice.setFont(font);
            advice.setCharacterSize(14);
            advice.setFillColor(Color::Yellow);

            // Dynamic word wrapping for the advice text (fits approx 28 characters per line)
            std::string wrappedText = "";
            size_t lineLength = 0;
            for (size_t i = 0; i < adviceText.length(); ++i) {
                wrappedText += adviceText[i];
                lineLength++;
                if (lineLength > 24 && adviceText[i] == ' ') {
                    wrappedText += "\n";
                    lineLength = 0;
                }
            }
            advice.setString(wrappedText);
            advice.setPosition(925.f, 45.f);
            window.draw(advice);
        }
    }
    
    // Draw semi-transparent dark overlay if paused or game ended
    if (isPaused || hasLost || hasWon)
    {
        RectangleShape darkOverlay(Vector2f(1200, 600));
        darkOverlay.setFillColor(Color(0, 0, 0, 150));
        window.draw(darkOverlay);
    }

    if (isPaused && !hasLost && !hasWon)
    {
        window.draw(pauseScreenSprite);
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
        if (last_screen < 1.0f)
        {
            last_screen += 0.01f;
        }

        Sprite& activeEndSprite = hasLost ? lostSprite : winSprite;
        
        activeEndSprite.setScale(last_screen, last_screen);
        activeEndSprite.setPosition(550, 300);
        window.draw(activeEndSprite);
    }
}

void Level::drawBackround(RenderWindow& window)
{
    backgroundSprite.setPosition(0, 0);
    window.draw(backgroundSprite);
}

void Level::loadLawnMowers()
{
    const int numRows = 5;
    for (int i = 0; i < numRows; i++)
    {
        LawnMower* newMower = new LawnMower();
        newMower->setPosition(160, 70 + (i * 100));
        lawn.movingObjectsArray.push_back(newMower);
    }
}

void Level::addSuntoShop()
{
    for (int i = 0; i < lawn.movingObjectsArray.getSize(); i++)
    {
        auto* currentObj = lawn.movingObjectsArray.at(i);

        // Check if object is an existing Sun
        if (currentObj->getName() == "Sun" && currentObj->isExist())
        {
            coordinate pos = currentObj->getPosition();
            
            // If Sun reaches the shop icon coordinates, collect it
            bool isAtShopX = (pos.x >= 110 && pos.x <= 114);
            bool isAtShopY = (pos.y >= 25 && pos.y <= 29);

            if (isAtShopX && isAtShopY)
            {
                currentObj->setAlive(false);
                shop.incrementSun(25);
            }
        }
    }
}

void Level::takeInput(RenderWindow& window)
{
    Event event;
    while (window.pollEvent(event))
    {
        handleEvent(event, window);
    }
}

void Level::handleEvent(Event& event, RenderWindow& window)
{
    if (event.type == Event::Closed)
    {
        window.close();
    }

    // --- MOUSE INPUT ---
    if (event.type == Event::MouseButtonPressed)
    {
        int mouseX = event.mouseButton.x;
        int mouseY = event.mouseButton.y;
        coordinate gridPos = lawn.getGridPosition(mouseX, mouseY);

        if (event.mouseButton.button == Mouse::Left)
        {
            cout << "[DEBUG] Mouse Left Press at: " << mouseX << ", " << mouseY << endl;
            cout << "[DEBUG] GridPos: " << gridPos.x << ", " << gridPos.y << endl;
            cout << "[DEBUG] Shop selected before click: " << shop.getSelectedPlant() << " sun=" << shop.getSun() << endl;
            if (!isPaused)
            {
                bool isWithinGrid = (gridPos.x >= 0 && gridPos.x < 9 && gridPos.y >= 0 && gridPos.y < 5);

                if (isWithinGrid && shop.isPurchasable())
                {
                    string selectedItem = shop.getSelectedPlant();

                    cout << "[DEBUG] Attempting to plant: " << selectedItem << " at " << gridPos.x << "," << gridPos.y << "\n";

                    // Handle Tools (Shovel / Water)
                    if (selectedItem == "Shovel")
                    {
                        if (lawn.removePlant(gridPos.x, gridPos.y))
                        {
                            shop.purchase();
                            shop.selectPlant("none");
                        }
                        lawn.setBoxAvailable(gridPos.x, gridPos.y, true);
                    }
                    else if (selectedItem == "Water")
                    {
                        if (lawn.waterPlant(gridPos.x, gridPos.y))
                        {
                            shop.purchase();
                            shop.selectPlant("none");
                        }
                    }
                    // Handle Planting
                    else if (lawn.isBoxAvailable(gridPos.x, gridPos.y))
                    {
                        float spawnX = lawn.getGRID_START_X() + (gridPos.x * lawn.getboxWidth());
                        float spawnY = lawn.getGRID_START_Y() + (gridPos.y * lawn.getboxHeight());
                        
                        lawn.addPlant(spawnX, spawnY, selectedItem);
                        lawn.setBoxAvailable(gridPos.x, gridPos.y, false);
                        
                        shop.purchase();
                        shop.selectPlant("none");
                    }
                }

                // Update selections based on clicks
                shop.selectPlant(mouseX, mouseY);
                lawn.selectSun(mouseX, mouseY);
            }

            // Check Pause Button Click
            if (mouseX >= 1035 && mouseX <= 1085 && mouseY >= 25 && mouseY <= 75)
            {
                isPaused = true;
            }
            
            // Check Resume Button Click (Inside Pause Menu)
            if (isPaused && mouseX >= 400 && mouseX <= 680 && mouseY >= 435 && mouseY <= 475)
            {
                isPaused = false;
            }
        }
        else if (event.mouseButton.button == Mouse::Right)
        {
            cout << "PIXEL : " << mouseX << " " << mouseY << endl;
            cout << "GRID : " << gridPos.x << " " << gridPos.y << endl;
        }
    }

    // --- KEYBOARD INPUT ---
    if (event.type == Event::KeyPressed)
    {
        if (event.key.code == Keyboard::Enter)
        {
            if (hasLost || hasWon)
            {
                isEnd = true;
            }
        }
        else if (event.key.code == Keyboard::S)
        {
            saveProgress();
        }
    }
}

void Level::checkLoose()
{
    if (lawn.checkLost())
    {
        if (livesRemaining <= 1)
        {
            isPaused = true;
            hasLost = true;
        }
        else
        {
            livesRemaining--;
            livesTexture.loadFromFile("Assets\\Background\\" + to_string(livesRemaining) + "lives.png");
            livesSprite.setTexture(livesTexture);
        }
    }
}

void Level::checkWin()
{
    extern bool g_isUnlimited;
    if (g_isUnlimited)
    {
        return; // Infinite survival mode!
    }

    int requiredKills = 0;
    if (levelNumber == 1) requiredKills = 10;
    else if (levelNumber == 2) requiredKills = 15;
    else if (levelNumber == 3) requiredKills = 18;

    if (requiredKills > 0 && Zombie::killCount >= requiredKills)
    {
        if (!hasWon) {
            hasWon = true;
            isPaused = true;
            cout << "GAME_OVER: " << getScore() << endl;
            cout.flush();
        }
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

        float dropX = lawn.getGRID_START_X() + (randomGridX * lawn.getboxWidth()) + 10;
        float dropY = lawn.getGRID_START_Y() + (randomGridY * lawn.getboxHeight()) + 40;

        Sun* fallingSun = new Sun();
        fallingSun->setFallingPosition(dropX, dropY);
        
        lawn.movingObjectsArray.push_back(fallingSun);
    }
}

bool Level::getIsEnd()
{
    return isEnd;
}

bool Level::getIsWon()
{
    return hasWon;
}

bool Level::getIsLost()
{
    return hasLost;
}

void Level::saveProgress()
{
    std::ofstream out("savegame.txt");
    if (out.is_open())
    {
        out << levelNumber << "\n" << timeSurvived << "\n" << Zombie::killCount << "\n" << shop.getSun() << "\n";
        out.close();
        cout << "Game Saved to savegame.txt" << endl;
    }
}

void Level::loadProgress()
{
    std::ifstream in("savegame.txt");
    if (in.is_open())
    {
        int lvl; float t; int k; int s;
        if (in >> lvl >> t >> k >> s)
        {
            timeSurvived = t;
            Zombie::killCount = k;
            shop.setSun(s);
            cout << "Game Loaded from savegame.txt" << endl;
        }
        in.close();
    }
}

int Level::getScore()
{
    return (int)timeSurvived + Zombie::killCount;
}

bool Level::getIsPaused()
{
    return isPaused;
}

void Level::setIsPaused(bool p)
{
    isPaused = p;
}