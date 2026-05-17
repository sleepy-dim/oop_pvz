#pragma once
#include "Plant.h"
class cherry : public Plant
{
	bool exploded = false;
	float radius;
	int time_to_explode;
	float timeExploding = 0;
	bool exploding = false;
public:
	cherry();
	~cherry();
	void draw(RenderWindow&)override;
	void action(float, MovingObjectArr* )override;
	bool isExist() override;

private:
	static Texture animationTextures[7];
	static bool texturesLoaded;
};