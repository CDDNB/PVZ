#pragma once
#include "Plant.h"
#include"Function.h"
#include<vector>

class Bomb :
	public Plant
{
protected:
	int time, cnt;
public:
	Bomb(int hloc, int vloc) :Plant(hloc, vloc), time(5), cnt(0) { name = "Œ—πœ"; value = 25; type = 1; cd = 30; }
	virtual vector<Location> bomb(bool b);
};

class Pumpkin :
	public Bomb
{
public:
	//static int count;
	Pumpkin(int hloc, int vloc) :Bomb(hloc, vloc) {}
};

class Cherry :
	public Bomb
{
private:
	int round;
public:
	//static int count;
	Cherry(int hloc, int vloc) :Bomb(hloc, vloc) { name = "”£Ã“"; value = 30; type = 1; round = 3; cd = 50; }
	vector<Location> bomb(bool b);
};

class Chili :
	public Bomb
{
public:
	//static int count;
	Chili(int hloc, int vloc) :Bomb(hloc, vloc) { name = "¿±Ω∑"; value = 25; type = 1; cd = 50; }
	vector<Location>bomb(bool b);
};