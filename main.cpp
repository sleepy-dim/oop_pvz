#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "garden.h"
#include <iostream>

#define WIN32_LEAN_AND_MEAN
#include <windows.h> // For MessageBox confirmation

using namespace sf;
using namespace std;

#include <thread>
#include <mutex>

bool g_isUnlimited = false;
std::string g_aiAdvice = "Waiting for Dave's tactical advice...";
std::mutex g_adviceMutex;

void readStdinLoop() {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.rfind("AI_ADVICE:", 0) == 0) {
            std::lock_guard<std::mutex> lock(g_adviceMutex);
            g_aiAdvice = line.substr(10); // Extract advice
        }
    }
}

int main(int argc, char** argv)
{
    // Start background thread to read AI advice from Java launcher stdin
    std::thread stdinThread(readStdinLoop);
    stdinThread.detach();

    bool loadSave = false;
    int startLevel = 1;
    for (int i = 1; i < argc; ++i) {
        if (string(argv[i]) == "--load") {
            loadSave = true;
        } else if (string(argv[i]) == "--level" && i + 1 < argc) {
            startLevel = atoi(argv[++i]);
        } else if (string(argv[i]) == "--unlimited") {
            g_isUnlimited = true;
        }
    }
    cout << "Starting main... Level: " << startLevel << " Unlimited: " << g_isUnlimited << endl;
    // Create the main game window
    RenderWindow window(VideoMode(1200, 600), "Plants vs Zombies", Style::Close);
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

    // Initialize the level
    cout << "Initializing level " << startLevel << "..." << endl;
    garden* currentLevel = new garden(startLevel);
    if (loadSave) {
        currentLevel->loadProgress(); 
    }
    cout << "Level " << startLevel << " initialized." << endl;

    // Clock for delta time
    Clock gameClock;
    bool isGameRunning = true;

    // Main game loop
    while (window.isOpen() && isGameRunning)
    {
        // Event processing
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // Pause the game and ask for confirmation
                bool wasPaused = currentLevel->getIsPaused();
                currentLevel->setIsPaused(true);
                
                int result = MessageBoxA(window.getSystemHandle(), 
                    "Do you really want to quit the game?", 
                    "Quit Confirmation", 
                    MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2 | MB_TASKMODAL);
                
                if (result == IDYES) {
                    window.close();
                    isGameRunning = false;
                } else {
                    currentLevel->setIsPaused(wasPaused);
                }
            }
            else
            {
                // Pass all other events to the current level
                currentLevel->handleEvent(event, window);
            }
        }

        if (!window.isOpen()) break;

        // Calculate delta time
        float deltaTime = gameClock.restart().asSeconds();

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
        int finalScore = currentLevel->getScore();
        cout << "GAME_OVER:" << finalScore << endl;
        cout.flush();  // Force flush output
        cerr << "DEBUG: Game ended with score " << finalScore << endl;
    }
    delete currentLevel;
    if (window.isOpen()) window.close();

    return 0;
}
