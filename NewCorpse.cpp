#include "NewCorpse.h"
#include"Function.h"
#include"MusicPlayer.h"
#include<Windows.h>
#include<iostream>
using namespace std;
void Newspaper::hurt(Bullet* bullet) {
	MusicPlayer mp("hit.wav", false, "HIT");
	mp.play();
	if (_head != NULL) {
		_head->hurt(bullet->attack);
		if (_head->dead()) {
			delete _head;
			_head = NULL;
			name = "报纸";
			on = true;
		}
	}
	else {
		health -= bullet->attack;
	}
	if (speed > 3) {
		speed -= bullet->de_speed;
	}
}

void Newspaper::move() {
	int T = 60;
	if (on)
		T = 30;
	if (time < T / speed) {
		time++;
	}
	else if (!isPlant(HLoc, VLoc)) {
		VLoc--;
		time = 0;
	}
}

void Block::hurt(Bullet* bullet) {
	MusicPlayer mp("hit.wav", false, "HIT");
	mp.play();
	if (_head != NULL) {
		_head->hurt(bullet->attack);
		if (_head->dead()) {
			delete _head;
			_head = NULL;
			name = "路障";
		}
	}
	else
		health -= bullet->attack;
	if (speed > 3) {
		speed -= bullet->de_speed;
	}
}

void Jump::move() {
	if (time < 60 / speed) {
		time++;
	}
	else if (!isPlant(HLoc, VLoc)) {
		VLoc--;
		time = 0;
	}
	else if (on) {
		name = "撑杆";
		on = false;
		if (VLoc == 0) {//添加推土机
			extern bool ENDD;
			ENDD = true;
		}
		else if (!isHighNut(HLoc, VLoc))
			change_position(HLoc, VLoc - 1);
	}
}

vector<Location> Clown::bomb() {
	luck = rand() % 10;
	vector<Location> L;
	if (luck == 7) {
		MusicPlayer mp("boom.wav", false, "BOOM");
		mp.play();
		for (int i = HLoc - round / 2; i <= HLoc + round / 2; i++) {
			for (int j = VLoc - round / 2; j <= VLoc + round / 2; j++) {
				if (i >= 0 && i < 3 && j >= 0 && j < 7) {
					Location l;
					l.hloc = i; l.vloc = j;
					L.push_back(l);
				}
			}
		}
		health = 0;
	}
	return L;
}

Basket* Basketball::shoot() {
	if (shoot_time < 60 / speed || basket_num <= 0) {
		shoot_time++;
		return NULL;
	}
	shoot_time = 0;
	basket_num--;
	Basket* basket = new Basket(HLoc, VLoc, attack);
	return basket;
}

void Basketball::move() {
	if (basket_num <= 0) {
		if (time < 60 / speed) {
			time++;
		}
		else if (!isPlant(HLoc, VLoc)) {
			VLoc--;
			time = 0;
		}
	}
}

int Basketball::eat() {
	return 999;
}