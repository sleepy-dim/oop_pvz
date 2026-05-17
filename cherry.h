#pragma once
#include "Plant.h"
class cherry : public Plant
{
	bool has_exploded = false;
	float exploding_radius;
	int time_to_explode;
	float time_exploding = 0;
	bool is_exploding = false;
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