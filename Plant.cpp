#include "Plant.h"
#include<iostream>
using namespace std;

int Head::count = 0;
/*void Plant::print() {
	cout << name;
	if (health < 0)
		cout << '0';
	else
		cout << health;
}*/

ostream& operator<<(ostream& os, const Plant* plant) {
	os << plant->name;
	if (plant->health < 0)
		os << '0';
	else
		os << plant->health;
	return os;
}

void Plant::print() {
	cout << name << ' ';
}

void Plant::hurt(int h) {
	if (_head != NULL) {
		_head->hurt(h);
		if (_head->dead())
			_delete();
	}
	else
		health -= h;
}

bool Plant::death() {
	if (health <= 0) {
		return true;
	}
	else
		return false;
}

void Plant::_add() {
	if (type == 1)
		return;
	if (health > 0) {
		_head = new Head;
	}
	name.insert(0, "[");
	name += ']';
}

void Plant::_delete() {
	delete _head;
	_head = NULL;
	name.erase(name.begin());
	name.erase(name.end() - 1);
}

void Plant::bombed() {
	health = 0;
}

void Plant::change_position(int x, int y) {
	HLoc = x; VLoc = y;
}