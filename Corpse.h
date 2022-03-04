#pragma once
#include<string>
#include"Bullet.h"
using namespace std;

class Corpse
{
protected:
	string name;
	int health;
	int attack, attack_speed, attack_time;
	int time;
	//CHead* _head;
public:
	int speed;
	int value;//»÷É±µÃ·Ö
	int HLoc, VLoc;
	/*Corpse() :health(100), attack(50), speed(5), value(20), HLoc(-1), VLoc(-1), time(0), attack_speed(5), attack_time(30/attack_speed) {
		string n = "½©Ê¬";
		name = n;
		//name = "ÆÕÍ¨½©Ê¬";
	}*/
	Corpse(int hloc, int vloc) :health(100), attack(25), speed(5), value(20), time(0), attack_speed(5), attack_time(60/attack_speed) {
		string n = "½©Ê¬";
		name = n;
		//name = "ÆÕÍ¨½©Ê¬";
		HLoc = hloc;
		VLoc = vloc;
	}
	string print();
	virtual void move();
	bool death();
	virtual int eat();
	virtual void hurt(Bullet* bullet);
	void bomb(int h);
	void change_position(int hloc, int vloc);
	friend ostream& operator<<(ostream& os, const Corpse* corpse);
	//void hurt(int h);
};

