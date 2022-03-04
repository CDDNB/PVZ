#pragma once
#include<string>
#include"Function.h"
//#include<iostream>
using namespace std;
class Bullet
{
protected:
	string name;
	int speed, time;
public:
	int HLoc, VLoc;
	int attack;
	int de_speed;//减速效果，也偷鸡用于判断子弹类型
	bool used;//使用后变为true
	Bullet(int hloc, int vloc, int Attack) :name("*"), speed(20), de_speed(0), used(false), time(0) { attack = Attack; HLoc = hloc; VLoc = vloc; }
	Bullet(int hloc, int vloc, int Attack, int De_speed) :name("*"), speed(20), used(false), time(0) {
		HLoc = hloc; VLoc = vloc;
		attack = Attack;
		de_speed = De_speed;
	}
	void print();
	virtual void move();
	friend ostream& operator<<(ostream& os, const Bullet* bullet);
};

class Basket :public Bullet {
public:
	Basket(int hloc, int vloc, int Attack) :Bullet(hloc, vloc, Attack) { name = "@"; de_speed = -1; }
	void move();
};