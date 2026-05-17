#include "plantGen.h"


plantGen* plantGen::instance = nullptr;

plantGen* plantGen::getInstance()
{
	if (plantGen::instance == nullptr)
	{
		plantGen::instance = new plantGen();
	}
	return plantGen::instance;
}

plantGen::plantGen()
{

}

Plant* plantGen::createPlant(std::string plantName)
{
	int id = 0;

	if (plantName == "PeaShooter") id = 1;
	else if (plantName == "SunFlower") id = 2;
	else if (plantName == "FrozenPeaShooter") id = 3;
	else if (plantName == "Repeater") id = 4;
	else if (plantName == "CherryBomb") id = 5;
	else if (plantName == "WallNut") id = 6;
	else if (plantName == "Cactus") id = 7;

	switch (id)
	{
	case 1:
		return new PeaShooter();
	case 2:
		return new SunFlower();
	case 3:
		return new frozenPea();
	case 4:
		return new Repeater();
	case 5:
		return new cherry();
	case 6:
		return new Wallnut();
	case 7:
		return new Cactus();
	default:
		return nullptr;
	}
}

plantGen::~plantGen()
{
}