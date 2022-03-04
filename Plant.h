#pragma once
#include<string>
#include<vector>

using namespace std;
class Head {
private:
	int health;
public:
	static int count;
	Head() :health(300) {}
	void hurt(int h) { health -= h; }
	bool dead() { return health <= 0; }
};

class Plant
{
protected:
	string name;
	int health;
	Head* _head;
	void change_position(int x, int y);
public:
	int value, cd;
	int HLoc, VLoc;
	int type;//0 shoot, 1 bomb, 2 defense, 3 func
	Plant(int hloc, int vloc) :health(100), value(10), type(0), _head(NULL), cd(10) {
		string n = "普通植物";
		name = n;
		//name = "豌豆射手";
		HLoc = hloc;
		VLoc = vloc;
	}
	//virtual void print();
	virtual void hurt(int h);//挨打
	void bombed();
	void _add();
	void _delete();
	bool death();
	void print();
	friend ostream& operator<<(ostream& os, const Plant* plant);
	friend class Groove;
};

class Groove {
private:
	vector<Plant*> allPlant;
public:
	Groove() {}
	void create();
	void print();
	void plant(int x, int y, int i);
};