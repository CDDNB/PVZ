#pragma once
#include "Corpse.h"
#include<vector>
class CHead {
private:
	int health;
public:
	CHead() :health(200) {}
	void hurt(int h) { health -= h; }
	bool dead() { return health <= 0; }
};

class Newspaper :
	public Corpse
{
private:
	CHead* _head;
	bool on;
public:
	Newspaper(int hloc, int vloc) :Corpse(hloc, vloc) { 
		name = "[报纸]";
		_head = new CHead;
		on = false;
		value = 35;
	}
	~Newspaper() {
		if (_head != NULL)
			delete _head;
	}
	void hurt(Bullet* bullet);
	void move();
};

class Block :
	public Corpse
{
private:
	CHead* _head;
public:
	Block(int hloc, int vloc) :Corpse(hloc, vloc) {
		name = "[路障]";
		_head = new CHead;
		value = 30;
	}
	~Block() {
		if (_head != NULL)
			delete _head;
	}
	void hurt(Bullet* bullet);
};

class Jump :
	public Corpse
{
private:
	bool on = true;
public:
	Jump(int hloc, int vloc) :Corpse(hloc, vloc) {
		name = "|撑杆";
		value = 30;
		speed = 6;
		health = 400;
	}
	void move();
};

class Clown :
	public Corpse
{
private:
	int luck;
	int round;
public:
	Clown(int hloc, int vloc) :Corpse(hloc, vloc) {
		name = "小丑";
		value = 47;
		luck = 0; round = 3;
	}
	vector<Location> bomb();
};

class Basketball :
	public Corpse
{
private:
	int attack, speed, shoot_time, basket_num;
public:
	Basketball(int hloc, int vloc) :Corpse(hloc, vloc), attack(40), speed(5), shoot_time(60 / speed) {
		name = "投石";
		value = 100;
		basket_num = 10;
	}
	Basket* shoot();
	void move();
	int eat();
};