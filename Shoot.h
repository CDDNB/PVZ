#pragma once
#include "Plant.h"
#include"Bullet.h"
class Shoot :
	public Plant
{
protected:
	int attack;
	int speed, shoot_time;
public:
	Shoot(int hloc, int vloc) :attack(20), speed(10), shoot_time(60 / speed), Plant(hloc, vloc) {
		string n = "Íã¶¹"; name = n;
		type = 0;
		cd = 10;
	}
	virtual Bullet* shoot();
};

class NormalShoot :
	public Shoot
{
public:
	//static int count;
	NormalShoot(int hloc, int vloc) :Shoot(hloc, vloc) {}
};

class DoubleShoot :
	public Shoot
{
public:
	static int count;
	DoubleShoot(int hloc, int vloc) :Shoot(hloc, vloc) { name = "Ë«·¢"; speed = 20; cd = 20; }
	Bullet* shoot();
};

class IceShoot :
	public Shoot
{
public:
	static int count;
	IceShoot(int hloc, int vloc) :Shoot(hloc, vloc) { name = "º®±ù"; value = 20; cd = 20; }
	Bullet* shoot();
};

class Sunflower :
	public Shoot
{
public:
	static int count;
	Sunflower(int hloc, int vloc) :Shoot(hloc, vloc) {
		name = "ÏòÈÕ¿û";
		value = 10;
		type = 3;
		speed = 3;
		cd = 10;
	}
	Bullet* shoot();
};