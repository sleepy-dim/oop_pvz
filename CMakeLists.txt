cmake_minimum_required(VERSION 3.10)
project(PlantsVsZombies)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add SFML
set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SFML/lib/cmake/SFML")
find_package(SFML 2.5 COMPONENTS graphics window system audio REQUIRED)

# Add all source files
set(SOURCES
    main.cpp
    ammo.cpp
    card.cpp
    character.cpp
    cherry.cpp
    explosion.cpp
    cactus.cpp
    flyingZom.cpp
    frozenPea.cpp
    frozenPeaBullet.cpp
    garden.cpp
    level.cpp
    LawnMower.cpp
    MovingObject.cpp
    object.cpp
    Pea.cpp
    PeaShooter.cpp
    Plant.cpp
    plantGen.cpp
    playground.cpp
    Repeater.cpp
    ShootingPlant.cpp
    Shop.cpp
    SimpleZom.cpp
    Sun.cpp
    SunFlower.cpp
    Wallnut.cpp
    Zombie.cpp
    zombieGen.cpp
)

# Add all header files
set(HEADERS
    ammo.h
    card.h
    character.h
    cherry.h
    explosion.h
    cactus.h
    flyingZom.h
    frozenPea.h
    frozenPeaBullet.h
    garden.h
    level.h
    LawnMower.h
    MovingObject.h
    object.h
    Pea.h
    PeaShooter.h
    Plant.h
    plantGen.h
    playground.h
    Point.h
    Repeater.h
    ShootingPlant.h
    Shop.h
    simpleZom.h
    Sun.h
    SunFlower.h
    Wallnut.h
    Zombie.h
    zombieGen.h
)

# Create executable
add_executable(PlantsVsZombies ${SOURCES} ${HEADERS})

# Link SFML libraries
target_link_libraries(PlantsVsZombies PRIVATE sfml-graphics sfml-window sfml-system sfml-audio)

# Include directories
target_include_directories(PlantsVsZombies PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(PlantsVsZombies PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/SFML/include")
