#ifndef character_h
#define character_h
#include "object.h"
#include <string>
using namespace std;

class character : public object
{
protected:
	int health;
	int damage;
public:
	string getName();
	void setName(string name);
	int getHealth();
	void setHealth(int health);
	int getDamage();
	void setDamage(int damage);
	bool isExist() override;
};

#endif