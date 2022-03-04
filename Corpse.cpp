#include "Corpse.h"
#include<iostream>
#include"Function.h"
#include"Bullet.h"
#include"MusicPlayer.h"
using namespace std;

/*void Corpse::print() {
	cout << name;
	if (health < 0)
		cout << '0';
	else
		cout << health;
}*/

ostream& operator<<(ostream& os, const Corpse* corpse) {
	os << corpse->name;
	if (corpse->health < 0)
		os << '0';
	else
		os << corpse->health;
	return os;
}

string Corpse::print() {
	string a = "";
	a += name;
	if (health < 0)
		a += '0';
	else
		a += to_string(health);
	return a;
}

void Corpse::move() {
	if (time < 60 / speed) {
		time++;
	}
	else if(!isPlant(HLoc, VLoc)) {
		VLoc--;
		time = 0;
	}
}

bool Corpse::death() {
	if (health <= 0) {
		return true;
	}
	else
		return false;
}

int Corpse::eat() {
	if (attack_time < 30 / attack_speed) {
		attack_time++;
		return 0;
	}
	attack_time = 0;
	MusicPlayer mp("bite.wav", false, "BITE");
	mp.play();
	return attack;
}

//void Corpse::hurt(int h) {
	//health -= h;
//}

void Corpse::hurt(Bullet* bullet) {
	MusicPlayer mp("hit.wav", false, "HIT");
	mp.play();
	health -= bullet->attack;
	if (speed > 3)
		speed -= bullet->de_speed;
}

void Corpse::bomb(int h) {
	health -= h;
}

void Corpse::change_position(int hloc, int vloc) {
	HLoc = hloc;
	VLoc = vloc;
}