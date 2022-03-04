#include "Bullet.h"
#include<iostream>

/*void Bullet::print() {
	cout << name;
}*/

ostream& operator<<(ostream& os, const Bullet* bullet) {
	os << bullet->name;
	return os;
}

void Bullet::move() {
	if (used) {
		return;
	}
	if (time < 60 / speed) {
		time++;
	}
	else if (!isCorpse(HLoc, VLoc)) {
		if (VLoc == 6) {
			used = true;
			return;
		}
		VLoc++;
		time = 0;
	}
}

void Basket::move() {
	if (used)
		return;
	if (time < 60 / speed)
		time++;
	else if (!isPlant(HLoc, VLoc)) {
		if (VLoc == 0) {
			used = true;
			return;
		}
		VLoc--;
		time = 0;
	}
}