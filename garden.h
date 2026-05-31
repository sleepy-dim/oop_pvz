#ifndef garden_h
#define garden_h
#include "Level.h"
#include "LawnMower.h"
class garden : public Level
{
public:
	garden(int level);
	void draw(RenderWindow& window) override;
    void update(float deltaTime) override;
	void zombieSpawn() override;
	void input(RenderWindow& windowzz);
	void loadLawnMowers() override;
private:
	float zombieTimer = 0.0f;
	float zombieSpawnInterval = 10.0f; // Spawn a new zombie every 10 seconds!
	float aiTimer = 0.0f;
};

#endif 