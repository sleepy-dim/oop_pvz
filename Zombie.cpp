#include "Zombie.h"
#include <iostream>

// --- ZombieArr Implementation ---

ZombieArr::ZombieArr() {
	capacity = 45;
	size = 0;
	arr = new Zombie * [capacity];
}

ZombieArr::~ZombieArr() {
	for (int i = 0; i < size; i++) {
		if (arr[i] != nullptr) {
			delete arr[i];
		}
	}
	delete[] arr;
}


void ZombieArr::push_back(Zombie* obj) {
	if (size == capacity) {
		resize();
	}
	arr[size] = obj;
	size++;
}

void ZombieArr::pop_back() {
	if (size > 0) {
		delete arr[size - 1];
		size--;
	}
}

int ZombieArr::getSize() const {
	return size;
}

bool ZombieArr::empty() const {
	return size == 0;
}

void ZombieArr::clear() {
	for (int i = 0; i < size; i++) {
		delete arr[i];
	}
	size = 0;
}

void ZombieArr::resize() {
	capacity *= 2;
	Zombie** temp = new Zombie * [capacity];
	for (int i = 0; i < size; i++) {
		temp[i] = arr[i];
	}
	
	// FIX: Do NOT delete the objects at arr[i]
	delete[] arr;
	arr = temp;
}

Zombie* ZombieArr::at(int index) const {
	if (index >= 0 && index < size) {
		return arr[index];
	}
	return nullptr;
}

void ZombieArr::insert(Zombie* obj, int index) {
	if (index >= 0 && index <= size) {
		if (size == capacity) {
			resize();
		}
		for (int i = size; i > index; i--) {
			arr[i] = arr[i - 1];
		}
		arr[index] = obj;
		size++;
	}
}

void ZombieArr::erase(int index) {
	if (index >= 0 && index < size) {
		delete arr[index];
		for (int i = index; i < size - 1; i++) {
			arr[i] = arr[i + 1];
		}
		size--;
	}
}

// --- Zombie Implementation ---

int Zombie::killCount = 0;

Zombie::Zombie() : totalFrozenTime(15)  {
	isMoving = true;
	isFrozenTimer = 0;
	isFrozen = false;
	totalIntervals = 0;
	timer = 0;
	state = "Moving";
	dx = 0;
	dy = 0;
	isWithoutHead = false;
	headTimer = 0;
	isDying = false;
	isBurnt = false;
	burntTimer = 0;
	attackTimer = 1.0f;
}

Zombie::~Zombie()
{
}

float Zombie::getdx()
{
	return dx;
}

float Zombie::getdy()
{
	return dy;
}

float Zombie::getAttackTimer()
{
	return attackTimer;
}

void Zombie::setAttackTimer(float t)
{
	this->attackTimer = t;
}

void Zombie::setSpeed(float dx, float dy)
{
	this->dx = dx;
	this->dy = dy;
}

void Zombie::draw(sf::RenderWindow& window)
{		
	if (isExist())
	{
		sprite.setTexture(texture);
		sprite.setPosition(position.x, position.y);
		if (isFrozen)
		{
			sprite.setColor(sf::Color(0, 0, 255, 255 ));
		}
		else
		{
			sprite.setColor(sf::Color(255, 255, 255, 255));
		}
		window.draw(sprite);
	}
}

void Zombie::setState(std::string s)
{
	state = s;
}

std::string Zombie::getState()
{
	return state;
}

void Zombie::setIsMoving(bool b)
{
	isMoving = b;
}

bool Zombie::getIsMoving()
{
	return isMoving;
}

bool Zombie::setIsFrozen(bool isFrozen)
{
	this->isFrozen = isFrozen;
	return isFrozen;
}

void Zombie :: action(float x)
{
	timer += (x*5);
	if (timer > totalIntervals) {
		timer = 0;
	}
	attackTimer += x;
	if (isWithoutHead)
	{
		headTimer += (x * 15);
		if (headTimer > 20)
		{
			headTimer = 0;
			isWithoutHead = false;
		}
	}

	if (!isDying)
	{
		if (health <= 0)
		{
			isDying = true;
			headTimer = 0;
		}
	}

	if (isBurnt)
	{
		burntTimer += x;
		if (burntTimer > 10)
		{
			isAlive = false;
			isBurnt = false;
		}
	}

	if (isDying)
	{
		headTimer += x;	
		if (headTimer >= 10 )
		{
			isDying = false  ;
		}
	}
	isFrozenTimer += x;
	if (isFrozenTimer >= totalFrozenTime)
	{
		isFrozen = false;
		isFrozenTimer = 0;
	}
}

bool Zombie::isExist()
{
	if (health >= 0)
	{
		return true;
	}
	if (health < 0)
	{
		return isBurnt ;
	}
	return false;
}

void  Zombie::setBurnt()
{
	health = 0;
	isBurnt = true ;
	setState("Burning");
}

bool Zombie::isCollidable()
{
	return true; // Simple implementation as per user's logic
}
