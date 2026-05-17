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
	void SpawnZombies() override;
	void input(RenderWindow& windowzz);
	void loadLawnMowers() override;
};

#endif 