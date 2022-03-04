#pragma once
#include "Plant.h"
#include<vector>
class Onion :
	public Plant
{
public:
	static int count;
	Onion(int hloc, int vloc) :Plant(hloc, vloc) {
		name = "¥ÛÀ‚";
		value = 15;
		type = 3;
		cd = 15;
	}
};
//int Onion::count = 0;

class Car {
private:
	int HLoc, VLoc;
public:
	Car(int hloc, int vloc) :HLoc(hloc), VLoc(vloc) {}

};
