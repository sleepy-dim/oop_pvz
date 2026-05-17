#include "zombieGen.h"
#include "DancingZombie.h"

zombieGen* zombieGen::instance = nullptr;

zombieGen* zombieGen::getInstance() {
    if (zombieGen::instance == nullptr) {
        zombieGen::instance = new zombieGen();
    }
    return zombieGen::instance;
}

zombieGen::zombieGen() { cout << "Zombie Factory\n";
}

Zombie* zombieGen::createZombie(std::string zombieName) {
    int id = 0;

    if (zombieName == "SimpleZombie") id = 1;
    else if (zombieName == "FootballZombie") id = 2;
    else if (zombieName == "FlyingZombie") id = 3;
    else if (zombieName == "DancingZombie") id = 4;

    switch (id) {
    case 1:
        return new SimpleZombie();
    case 2:
        return nullptr; // FootballZombie missing
    case 3:
        return new flyingZom();
    case 4:
        return new DancingZombie(); // DancingZombie integrated!
    default:
        return nullptr;
    }
}
zombieGen::~zombieGen() {
    // Destructor
}