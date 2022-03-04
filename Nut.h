#pragma once
#include "Plant.h"
class Nut :
	public Plant
{
public:
	//Nut() :Plant() { name = "坚果"; health = 500; value = 15; type = 2; }
	Nut(int hloc, int vloc) :Plant(hloc, vloc) { name = "坚果"; health = 500; value = 15; type = 2; cd = 20; }
};

class SmallNut :
	public Nut
{
public:
	//static int count;
	SmallNut(int hloc, int vloc) :Nut(hloc, vloc) {}
};
//int SmallNut::count = 0;
class HighNut :
	public Plant
{
public:
	//static int count;
	//HighNut() : Plant() { name = "高坚果"; health = 1000; value = 25; type = 2; }
	HighNut(int hloc, int vloc) :Plant(hloc, vloc) { name = "高坚果"; health = 1000; value = 25; type = 2; cd = 40; }
};
//int HighNut::count = 0;