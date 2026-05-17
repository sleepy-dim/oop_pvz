#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "garden.h"
#include <iostream>

using namespace sf;
using namespace std;

int main(int argc, char** argv)
{
    bool loadSave = false;
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "--load") {
            loadSave = true;
        }
    }
    cout << "Starting main..." << endl;
    // Create the main game window
    RenderWindow window(VideoMode(1200, 800), "Plants vs Zombies", Style::Close);
    window.setFramerateLimit(60);
    cout << "Window created." << endl;

    // Load and play background music
    sf::Music bgMusic;
    if (!bgMusic.openFromFile("Assets/Sounds/OOP music assets/06. Main Music 00.mp3"))
    {
        cout << "Error loading background music Assets/Sounds/OOP music assets/06. Main Music 00.mp3!" << endl;
    }
    else
    {
        bgMusic.setLoop(true);
        bgMusic.setVolume(40.f); // 40% volume is usually comfortable
        bgMusic.play();
        cout << "Background music playing successfully." << endl;
    }

    // Initialize the first level
    cout << "Initializing level 1..." << endl;
    garden* currentLevel = new garden(1);
    if (loadSave) {
        currentLevel->loadProgress();
    }
    cout << "Level 1 initialized." << endl;

    // Clock for delta time
    Clock gameClock;
    bool isGameRunning = true;

    // Main game loop
    while (window.isOpen() && isGameRunning)
    {
        // Calculate delta time
        float deltaTime = gameClock.restart().asSeconds();

        // Handle input
        currentLevel->takeInput(window);

        // Update game state
        currentLevel->update(deltaTime);

        // Check if game has ended
        if (currentLevel->getIsEnd())
        {
            isGameRunning = false;
        }

        // Clear and draw
        window.clear();
        currentLevel->draw(window);
        window.display();
    }

    // Cleanup
    if (currentLevel->getIsEnd()) {
        cout << "GAME_OVER:" << currentLevel->getScore() << endl;
    }
    delete currentLevel;
    window.close();

    return 0;
}
