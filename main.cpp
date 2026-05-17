#include <SFML/Graphics.hpp>
#include "garden.h"
#include <iostream>

using namespace sf;
using namespace std;

int main()
{
    // Create the main game window
    RenderWindow window(VideoMode(1200, 800), "Plants vs Zombies", Style::Close);
    window.setFramerateLimit(60);

    // Initialize the first level
    garden* currentLevel = new garden(1);

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
    delete currentLevel;
    window.close();

    return 0;
}
