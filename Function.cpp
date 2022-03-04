#include "Function.h"
#include<iostream>
#include"Plant.h"
#include"Corpse.h"
#include"Bullet.h"
#include"Shoot.h"
#include"Nut.h"
#include"bomb.h"
#include"Func.h"
#include"NewCorpse.h"
#include"MusicPlayer.h"
#include<vector>
#include<conio.h>
#include<iomanip>
#include<Windows.h>
#include<time.h>
#include<assert.h>
#include<fstream>
using namespace std;

bool ENDD = false;
#define H 3
#define V 7
#define T 1
#define PlantNum 11
//#define random(x) (rand()%x)
int mode;
int AllSun = 10;
int Score = 0;
bool store = false, buying = false;
int chosen_plant = -1;
int I = 0, J = 0;
int start = 0;
int Lots = 400;
bool shovel = false;
bool Cars[3] = { true, true, true };
int level;
//int start = 100;
//vector<Plant>AllPlant;
//vector<Corpse>AllCorpse;
struct Yard {
	bool isPlant = false;
	bool isCorpse = false;
	bool isBullet = false;
	vector<Corpse*>corpse;
	Plant* plant = NULL;
	vector<Bullet*>bullet;
};
Yard yard[H][V];
int* Count = new int[PlantNum + 2];
Groove groove;

int random(int x) {
	return rand() % x;
}

void init() {
	for (int i = 1; i <= PlantNum; i++)
		Count[i] = 0;
}

void check_yard(int hloc, int vloc) {//查找该位置有无单位
	if (yard[hloc][vloc].plant != NULL) {
		yard[hloc][vloc].isPlant = true;
	}
	else {
		yard[hloc][vloc].isPlant = false;
	}

	if (yard[hloc][vloc].corpse.empty()) {
		yard[hloc][vloc].isCorpse = false;
	}
	else {
		yard[hloc][vloc].isCorpse = true;
	}

	if (yard[hloc][vloc].bullet.empty()) {
		yard[hloc][vloc].isBullet = false;
	}
	else
		yard[hloc][vloc].isBullet = true;
	return;
}

bool isCorpseLine(int hloc, int vloc);
void freshen() {//刷新，更新yard中bool的状态、积分
	groove.create();
	for (int i = 1; i < PlantNum; i++) {
		if (Count[i] > 0)
			Count[i]--;
	}
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < V; j++) {
			for (int c = 0; c < yard[i][j].corpse.size();) {
				if (yard[i][j].corpse[c]->death()) {//检查僵尸死亡
					Score += yard[i][j].corpse[c]->value;
					yard[i][j].corpse[c]->value = 0;
					delete yard[i][j].corpse[c];
					yard[i][j].corpse.erase(yard[i][j].corpse.begin() + c);
					if (yard[i][j].corpse.size() == 0) {
						yard[i][j].isCorpse = false;
					}
					continue;
				}
				if (yard[i][j].corpse[c]->value == 47) {//检查小丑僵尸爆炸
					vector<Location>L;
					Clown* clown = dynamic_cast<Clown*>(yard[i][j].corpse[c]);
					L = clown->bomb();
					for (int i = 0; i < L.size(); i++) {
						if (yard[L[i].hloc][L[i].vloc].plant != NULL)
							yard[L[i].hloc][L[i].vloc].plant->bombed();
					}
				}
				if (yard[i][j].isPlant) {//僵尸攻击植物
					yard[i][j].plant->hurt(yard[i][j].corpse[c]->eat());
					if (yard[i][j].plant->type == 3 && yard[i][j].corpse[c]->value != 100) {//吃到大蒜了
						int h;
						switch (yard[i][j].corpse[c]->HLoc)
						{
						case 0:h = 1; break;
						case 2:h = 1; break;
						default: {
							h = rand() % 2;
							if (h == 1)
								h++;
						}
							break;
						}
						yard[i][j].corpse[c]->change_position(h, yard[i][j].corpse[c]->VLoc);
					}
				}
				if (yard[i][j].corpse[c]->value == 100 && isPlantLine(i, j)) {//僵尸射击植物
					Basketball* basketball = dynamic_cast<Basketball*>(yard[i][j].corpse[c]);
					Basket* basket = basketball->shoot();
					if (basket != NULL)
						yard[i][j].bullet.push_back(basket);
				}

				yard[i][j].corpse[c]->move();//僵尸移动
				if (yard[i][j].corpse[c]->HLoc != i || yard[i][j].corpse[c]->VLoc != j) {
					Corpse* corp = yard[i][j].corpse[c];
					//Corpse* corp = new Corpse;
					//*corp = *yard[i][j].corpse[c];
					yard[yard[i][j].corpse[c]->HLoc][yard[i][j].corpse[c]->VLoc].corpse.push_back(corp);
					yard[yard[i][j].corpse[c]->HLoc][yard[i][j].corpse[c]->VLoc].isCorpse = true;
					//delete yard[i][j].corpse[c];
					yard[i][j].corpse.erase(yard[i][j].corpse.begin() + c);
					if (yard[i][j].corpse.size() == 0) {
						yard[i][j].isCorpse = false;
					}
					continue;
				}
				c++;
			}

			for (int b = 0; b < yard[i][j].bullet.size();) {
				if (yard[i][j].bullet[b]->used) {//检查子弹状态
					delete yard[i][j].bullet[b];
					yard[i][j].bullet.erase(yard[i][j].bullet.begin() + b);
					if (yard[i][j].bullet.size() == 0) {
						yard[i][j].isBullet = false;
					}
					continue;
				}
				if (yard[i][j].isCorpse && yard[i][j].bullet[b]->de_speed >= 0) {//子弹击中僵尸
					yard[i][j].bullet[b]->used = true;
					//yard[i][j].corpse[0]->hurt(yard[i][j].bullet[b]->attack);
					yard[i][j].corpse[0]->hurt(yard[i][j].bullet[b]);
					if (yard[i][j].corpse[0]->death()) {
						Score += yard[i][j].corpse[0]->value;
						yard[i][j].corpse.erase(yard[i][j].corpse.begin());
						if (yard[i][j].corpse.size() == 0) {
							yard[i][j].isCorpse = false;
						}
					}
				}
				if (yard[i][j].isPlant && yard[i][j].bullet[b]->de_speed == -1) {//子弹击中植物
					yard[i][j].bullet[b]->used = true;
					yard[i][j].plant->hurt(yard[i][j].bullet[b]->attack);
					if (yard[i][j].plant->death()) {
						//delete yard[i][j].plant;
						yard[i][j].plant = NULL;
						yard[i][j].isPlant = false;
					}
				}
				yard[i][j].bullet[b]->move();//子弹移动

				if (yard[i][j].bullet[b]->HLoc != i || yard[i][j].bullet[b]->VLoc != j) {
					Bullet* bul = yard[i][j].bullet[b];
					yard[bul->HLoc][bul->VLoc].bullet.push_back(bul);
					yard[bul->HLoc][bul->VLoc].isBullet = true;
					yard[i][j].bullet.erase(yard[i][j].bullet.begin() + b);
					/*Bullet* bul = new Bullet;
					*bul = *yard[i][j].bullet[b];
					yard[yard[i][j].bullet[b]->HLoc][yard[i][j].bullet[b]->VLoc].bullet.push_back(bul);
					yard[yard[i][j].bullet[b]->HLoc][yard[i][j].bullet[b]->VLoc].isBullet = true;
					delete yard[i][j].bullet[b];
					yard[i][j].bullet.erase(yard[i][j].bullet.begin() + b);*/
					if (yard[i][j].bullet.size() == 0) {
						yard[i][j].isBullet = false;
					}
					continue;
				}

				b++;
			}

			//检查植物死亡
			if (yard[i][j].plant == NULL) {
				yard[i][j].isPlant = false;
			}
			else if (yard[i][j].plant->death()) {
				//delete yard[i][j].plant;
				yard[i][j].plant = NULL;
				yard[i][j].isPlant = false;
			}

			//植物攻击
			/*需要增加type以判断是哪种子类*/
			if (yard[i][j].plant != NULL && yard[i][j].plant->type == 0 && isCorpseLine(i, j)) {//射手型，创建子弹
				//Shoot* shootp = dynamic_cast<Shoot*>(yard[i][j].plant);
				Shoot* shootp = (Shoot*)yard[i][j].plant;
				Bullet* bullet = shootp->shoot();
				if (bullet != NULL) {
					yard[i][j].bullet.push_back(bullet);
					yard[i][j].isBullet = true;
				}
			}

			if (yard[i][j].plant != NULL && yard[i][j].plant->type == 1) {//炸弹型
				vector<Location>L;
				//Pumpkin
				Bomb* ppk = dynamic_cast<Bomb*>(yard[i][j].plant);
				L = ppk->bomb(yard[i][j].isCorpse);
				for (int i = 0; i < L.size(); i++) {
					for (int j = 0; j < yard[L[i].hloc][L[i].vloc].corpse.size(); j++) {
						yard[L[i].hloc][L[i].vloc].corpse[j]->bomb(999);
						/*if (yard[L[i].hloc][L[i].vloc].corpse[j]->death()) {
							Score += yard[L[i].hloc][L[i].vloc].corpse[j]->value;
							yard[L[i].hloc][L[i].vloc].corpse.erase(yard[i][j].corpse.begin());
							if (yard[i][j].corpse.size() == 0) {
								yard[i][j].isCorpse = false;
							}
							j--;
						}*/
					}
				}
			}

			if (yard[i][j].plant != NULL && yard[i][j].plant->type == 3) {

			}
			check_yard(i, j);
		}
	}
	return;
}

void print_yard() {
	cout << "#####################################################################################################################" << endl;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < V; j++) {
			cout << "|";
			if (yard[i][j].isBullet) {
				for (vector<Bullet*>::iterator it = yard[i][j].bullet.begin(); it != yard[i][j].bullet.end(); it++) {
					//(*it)->print(); //break;
					cout << *it;
				}
			}
			cout << "\t\t";
		}
		cout << "|" << endl;
		for (int j = 0; j < V; j++) {
			cout << "|";
			if (yard[i][j].isPlant) {
				//yard[i][j].plant->print();
				cout << setw(8) << yard[i][j].plant << "\t";
			}
			else
				cout << "\t\t";
		}
		cout << "|" << endl;
		for (int j = 0; j < V; j++) {
			cout << "|";
			if (j == 0 && Cars[i]) {
				cout << "-\\。";
			}
			if (yard[i][j].isCorpse) {
				for (vector<Corpse*>::iterator it = yard[i][j].corpse.begin(); it != yard[i][j].corpse.end(); it++) {
					cout << setw(15) << (*it)->print();
					//cout << setw(8) << (*it) << "\t";
					//cout << (*it);
				}
			}
			else
				cout << "\t\t";
		}
		cout << "|" << endl;
		for (int j = 0; j < V; j++) {
			cout << "|";
			if (shovel && i == I && j == J) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << "--》";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			if (i == I && j == J && ((mode == 1 && chosen_plant > 0 && !buying) || (mode == 2 && chosen_plant != -1 && !buying))) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
				cout << "Plant";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			cout << "\t\t";
		}
		cout << "|" << endl;
		cout << "---------------------------------------------------------------------------------------------------------------------" << endl;
	}
	if (mode == 1) {
		cout << endl << "=====================================================================================================================" << endl;
		if (store) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "[商店]  请用方向键选择需购买的植物  按[y]确认 [x]取消";
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else {
			cout << "[商店](press b)  [铲子](press c)";
		}
		cout << "        阳光总数：" << AllSun << " | " << "得分: " << Score << endl;
		cout << "---------------------------------------------------------------------------------------------------------------------" << endl;
		if (store) {
			if (chosen_plant != 1)
				cout << setw(16) << "向日葵5  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "向日葵5  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "向日葵5  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

			if (chosen_plant != 2)
				cout << setw(16) << "豌豆射手10  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "豌豆射手10  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "豌豆射手10  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

			if (chosen_plant != 3)
				cout << setw(16) << "双发射手20  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "双发射手20  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "双发射手20  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

			if (chosen_plant != 4)
				cout << setw(16) << "寒冰射手20  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "寒冰射手20  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "寒冰射手20  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

			if (chosen_plant != 5)
				cout << setw(16) << "窝瓜25  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "窝瓜25  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "窝瓜25  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			cout << endl;
			if (chosen_plant != 6)
				cout << setw(16) << "樱桃炸弹30  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "樱桃炸弹30  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "樱桃炸弹30  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

			if (chosen_plant != 7)
				cout << setw(16) << "火爆辣椒25  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "火爆辣椒25  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "火爆辣椒25  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			//cout << endl;
			if (chosen_plant != 8)
				cout << setw(16) << "南瓜头15  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "南瓜头15  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "南瓜头15  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

			if (chosen_plant != 9)
				cout << setw(16) << "洋葱15  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "洋葱15  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "洋葱15  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

			if (chosen_plant != 10)
				cout << setw(16) << "坚果15  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "坚果15  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "坚果15  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			cout << endl;
			if (chosen_plant != 11)
				cout << setw(16) << "高坚果25  ";
			else if (Count[chosen_plant] != 0) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
				cout << setw(16) << "高坚果25  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
				cout << setw(16) << "高坚果25  ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}

			cout << endl;
		}
	}
	else {
		if (store) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << "[商店]" << endl;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else
			cout << "[商店](press b)  [铲子](press c)" << endl;
		groove.print();
		cout << "得分：" << Score << endl;
	}
	cout << "=====================================================================================================================" << endl << endl;
	cout << "#####################################################################################################################" << endl;
	
	if (Lots < 151) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
		cout << "一大波僵尸正在靠近！碾碎他们！" << endl;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	return;
}

bool end_game() {
	bool end = false;
	for (int i = 0; i < H; i++) {
		if (!yard[i][0].isPlant && yard[i][0].isCorpse) {
			if (Cars[i]) {
				for (int j = 0; j < V; j++) {
					for (vector<Corpse*>::iterator it = yard[i][j].corpse.begin(); it != yard[i][j].corpse.end(); it++) {
						delete* it;
					}
					yard[i][j].isCorpse = false;
					yard[i][j].corpse.clear();
				}
				Cars[i] = false;
				return false;
			}
			else {
				end = true;
				break;
			}
		}
	}
	return end;
}

void makesun(int& i, int time) {
	i++;
	if (i == time) {
		AllSun += 5;
		i = 0;
	}
}

bool buy_plant() {
	if (_kbhit()) {
		char c = _getch();
		//重开
		if (c == 'r') {
			restart();
			system("cls");
			cout << "The game gonna restart..." << 3;
			Sleep(500);
			system("cls");
			cout << "The game gonna restart..." << 2;
			Sleep(500);
			system("cls");
			cout << "The game gonna restart..." << 1;
			Sleep(500);
			return false;
		}
		//暂停
		if (c == 'p') {
			cout << "Paused. Input anything to continue." << endl;
			int t;
			cin >> t;
			return false;
		}
		//退出
		if (c == 'q') {
			writescore();
			restart();
			cout << "Bye. Have a nice day!" << endl;
			return true;
		}
		//铲子
		if (c == 'c') {
			shovel = true;
			store = false;
			buying = false;
			chosen_plant = 0;
			I = 0; J = 0;
		}
		if (shovel) {
			int n = int(c);
			switch (c)
			{
			case 72: {
				if (I != 0)
					I--;
				break;
			}
			case 77: {
				if (J != V - 1)
					J++;
				break;
			}
			case 80: {
				if (I != H - 1) {
					I++;
				}
				break;
			}
			case 75: {
				if (J != 0) {
					J--;
				}
				break;
			}
			case 13: {
				if (yard[I][J].isPlant) {
					MusicPlayer mp("plant.wav", false, "mPLANT");
					mp.play();
					delete yard[I][J].plant;
					yard[I][J].plant = NULL;
					yard[I][J].isPlant = false;
				}
				shovel = false;
				I = 0; J = 0;
			}
			default:
				break;
			}
		}
		//取消
		if (c == 'x') {//取消购买
			shovel = false;
			store = false;
			buying = false;
			chosen_plant = 0;
			I = 0; J = 0;
		}
		if (mode == 1) {
			if (c == 'b') {//进入商店
				store = true; buying = true;
				chosen_plant = 1;
				I = 0; J = 0;
			}
			if (store && buying) {//选购植物
				int n = int(c);
				switch (c) {
				case 77: {
					if (chosen_plant < PlantNum)
						chosen_plant++;
					break;
				}
				case 75: {
					if (chosen_plant > 1)
						chosen_plant--;
					break;
				}
				case 72: {
					if (chosen_plant > 5)
						chosen_plant -= 5;
					break;
				}
				case 80: {
					if (chosen_plant < 7)
						chosen_plant += 5;
					break;
				}
				case 121: {
					if (Count[chosen_plant] != 0) {
						store = false;
						chosen_plant = 0;
						I = 0; J = 0;
					}
					buying = false;
					break;
				}
				default:break;
				}
			}
			if (store && chosen_plant != 0 && !buying) {
				int n = int(c);
				switch (n)
				{
				case 72: {
					if (I != 0)
						I--;
					break;
				}
				case 77: {
					if (J != V - 1)
						J++;
					break;
				}
				case 80: {
					if (I != H - 1) {
						I++;
					}
					break;
				}
				case 75: {
					if (J != 0) {
						J--;
					}
					break;
				}
				case 13: {
					if (yard[I][J].isPlant) {//该地块有植物
						if (chosen_plant == 8 && AllSun >= 10 && yard[I][J].plant->type != 1) {
							MusicPlayer mp("plant.wav", false, "mPLANT");
							mp.play();
							yard[I][J].plant->_add();
							Count[chosen_plant] = 20;
							AllSun -= 15;
						}
						store = false;
						chosen_plant = 0;
						I = 0; J = 0;
						break;
					}
					else {
						MusicPlayer mp("plant.wav", false, "mPLANT");
						mp.play();
						switch (chosen_plant)
						{
						case 1: {
							if (AllSun >= 10) {
								yard[I][J].plant = new Sunflower(I, J);
								AllSun -= 10;
								//Count[chosen_plant] = 10;
							}
							break;
						}
						case 2: {
							if (AllSun >= 10) {
								yard[I][J].plant = new NormalShoot(I, J);
								AllSun -= yard[I][J].plant->value;
								//Count[chosen_plant] = 10;
							}
							//Plant* plant = new Plant(I, J); 
							break;
						}
						case 3: {
							if (AllSun >= 20) {
								yard[I][J].plant = new DoubleShoot(I, J);
								AllSun -= yard[I][J].plant->value;
								//Count[chosen_plant] = 10;;
							}
							break;
						}
						case 4: {
							if (AllSun >= 20) {
								yard[I][J].plant = new IceShoot(I, J);
								AllSun -= yard[I][J].plant->value;
								//IceShoot::count = 20;
							}
							break;
						}
						case 5: {
							if (AllSun >= 25) {
								yard[I][J].plant = new Pumpkin(I, J);
								AllSun -= yard[I][J].plant->value;
								//Pumpkin::count = 40;
							}
							break;
						}
						case 6: {
							if (AllSun >= 30) {
								yard[I][J].plant = new Cherry(I, J);
								AllSun -= 30;
								//Cherry::count = 50;
							}
							break;
						}
						case 7: {
							if (AllSun >= 25) {
								yard[I][J].plant = new Chili(I, J);
								AllSun -= 25;
								//Chili::count = 50;
							}
							break;
						}
						case 9: {
							if (AllSun >= 15) {
								yard[I][J].plant = new Onion(I, J);
								AllSun -= 15;
								//Onion::count = 30;
							}
							break;
						}
						case 10: {
							if (AllSun >= 15) {
								yard[I][J].plant = new SmallNut(I, J);
								AllSun -= 15;
								//SmallNut::count = 30;
							}
							break;
						}
						case 11: {
							if (AllSun >= 25) {
								yard[I][J].plant = new HighNut(I, J);
								AllSun -= 25;
								//HighNut::count = 50;
							}
							break;
						}
						default: {
							mciSendString(TEXT("CLOSE mPLANT"), NULL, 0, NULL);
							break;
						}
						}
						if (chosen_plant != 8 && yard[I][J].plant != NULL)
							Count[chosen_plant] = yard[I][J].plant->cd;
						else if (chosen_plant == 8)
							Count[chosen_plant] = 15;
						//yard[I][J].plant = plant;
						store = false;
						chosen_plant = 0;
						I = 0; J = 0;
						break;
					}
				}
				default: break;
				}
			}
		}
		else {
			if (c == 'b') {
				store = true; buying = true;
				chosen_plant = -1;
			}
			if (buying) {
				int n = int(c);
				switch (n)
				{
				case '0':chosen_plant = 0; break;
				case '1':chosen_plant = 1; break;
				case '2':chosen_plant = 2; break;
				case '3':chosen_plant = 3; break;
				case '4':chosen_plant = 4; break;
				case '5':chosen_plant = 5; break;
				case '6':chosen_plant = 6; break;
				case '7':chosen_plant = 7; break;
				case '8':chosen_plant = 8; break;
				case '9':chosen_plant = 9; break;
				default://chosen_plant = -1;
					break;
				}
				if(n < 10)
					buying = false;
			}
			if (store && !buying && chosen_plant != -1) {
				int n = int(c);
				switch (n)
				{
				case 72: {
					if (I != 0)
						I--;
					break;
				}
				case 77: {
					if (J != V - 1)
						J++;
					break;
				}
				case 80: {
					if (I != H - 1) {
						I++;
					}
					break;
				}
				case 75: {
					if (J != 0) {
						J--;
					}
					break;
				}
				case 13: {
					if (!yard[I][J].isPlant) {
						groove.plant(I, J, chosen_plant);
					}
					store = false; buying = false;
					chosen_plant = -1;
					I = 0; J = 0;
					break;
				}
				}
			}
		}
		return false;
	}
}

void create_corpse() {
	//时间随机
	if (start > 0) {
		start--;
		return;
	}
	int p;
	if (Lots == 150) {
		MusicPlayer mp("Zbgm.wav", false, "ZBGM");
		mp.play();
	}
	if (Lots > 150) {
		p = random(5000/level);//种类随机
		Lots--;
	}
	else if(Lots > 0) {
		p = random(1000/level);
		Lots--;
	}
	else {
		p = random(5000/level);
		Lots = 1000;
	}
	int h = random(H);//横坐标，位置随机
	if (p < 70) {//普通僵尸概率
		Corpse* corpse = new Corpse(h, V - 1);
		yard[h][V - 1].corpse.push_back(corpse);
	}
	else if (p < 100) {//路障僵尸
		Corpse* corpse = new Block(h, V - 1);
		yard[h][V - 1].corpse.push_back(corpse);
	}
	else if (p < 130) {//读报僵尸
		Corpse* corpse = new Newspaper(h, V - 1);
		yard[h][V - 1].corpse.push_back(corpse);
	}
	else if (p < 150) {//撑杆僵尸
		Corpse* corpse = new Jump(h, V - 1);
		yard[h][V - 1].corpse.push_back(corpse);
	}
	else if (p < 170) {//小丑僵尸
		Corpse* corpse = new Clown(h, V - 1);
		yard[h][V - 1].corpse.push_back(corpse);
	}
	else if (p < 175) {
		Corpse* corpse = new Basketball(h, V - 1);
		yard[h][V - 1].corpse.push_back(corpse);
	}
	
}

bool isPlant(int hloc, int vloc) {
	return yard[hloc][vloc].isPlant;
}

bool isCorpse(int hloc, int vloc) {
	return yard[hloc][vloc].isCorpse;
}

bool isCorpseLine(int hloc, int vloc) {//hloc往后一整行是否有僵尸
	for (int i = vloc; i < V; i++) {
		if (yard[hloc][i].isCorpse)
			return true;
	}
	return false;
}

void end() {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < V; j++) {
			for (vector<Corpse*>::iterator it = yard[i][j].corpse.begin(); it != yard[i][j].corpse.end(); it++) {
				delete* it;
			}
			yard[i][j].corpse.clear();

			//if(yard[i][j].isPlant)
				//delete yard[i][j].plant;

			for (vector<Bullet*>::iterator it = yard[i][j].bullet.begin(); it != yard[i][j].bullet.end(); it++) {
				delete* it;
			}
			yard[i][j].bullet.clear();
		}
	}
	cout << "Brain Eaten. Game Over. Wait..." << endl;
	MusicPlayer mp("laugh.wav", false, "LAUGH");
	mp.play();
	Sleep(5000);
	mciSendString(TEXT("CLOSE MUSIC1"), NULL, 0, NULL);
}

bool isHighNut(int hloc, int vloc) {
	return yard[hloc][vloc].plant->type == 2 && yard[hloc][vloc].plant->value == 25;
}

bool isPlantLine(int hloc, int vloc) {
	for (int i = vloc; i >= 0; i--) {
		if (yard[hloc][i].isPlant)
			return true;
	}
	return false;
}

void restart() {
	AllSun = 0;
	Score = 0;
	store = false;
	buying = false;
	chosen_plant = 0;
	I = 0; J = 0;
	start = 101;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < V; j++) {
			yard[i][j].isBullet = yard[i][j].isCorpse = yard[i][j].isPlant = false;
			for (int k = 0; k < yard[i][j].bullet.size(); k++)
				delete yard[i][j].bullet[k];
			yard[i][j].bullet.clear();
			for (int k = 0; k < yard[i][j].corpse.size(); k++)
				delete yard[i][j].corpse[k];
			yard[i][j].corpse.clear();
			if(yard[i][j].plant != NULL)
				delete yard[i][j].plant;
			yard[i][j].plant == NULL;
		}
	}
}

void writescore() {
	cout << "Your final score is: " << Score << endl;
	ifstream history("history.txt", ios::in);
	if (history.fail()) {
		ofstream his("history.txt", ios::app);
		his << Score << endl << Score << endl;
		his.close();
		return;
	}
	int max;
	history >> max;
	if (Score > max) {
		cout << "You've broken the record!" << endl;
		//Sleep(3000);
		ofstream his("history.txt", ios::in | ios::out);
		his << Score;
		his.close();
	}
	ofstream his("history.txt", ios::app);
	his << Score << endl;
	his.close();
}

void startscreen() {
	MusicPlayer mp1("1.wav", true);
	//mp1.play();
	assert(mp1.play());
	//Sleep(99999);
	cout << "Welcome to my PVZ world!";
	L1: cout << "start NOW? Choose the game mode here:\n1.Normal  2.Entertainment  3.history" << endl;
	cin >> mode;

	while (mode != 1 && mode != 2 && mode != 3) {
		cout << "wtf?\n1.Normal  2.Entertainment  3.history" << endl;
		cin >> mode;
	}
	//mp1.stop();
	if (mode == 3) {
		ifstream in_file("history.txt", ios::in);
		if (in_file.fail()) {
			cout << "NO RECORD" << endl;
		}
		else {
			int score, count = 0;
			in_file >> score;
			cout << "您的最高得分为：" << score << endl;
			in_file >> score;
			cout << "您的历史得分为：" << endl;
			while (count < 100 && !in_file.eof()) {
				cout << score << endl;
				count++;
				in_file >> score;
			}
			in_file.close();
		}
		goto L1;
	}

	cout << "Choose difficulty level: 1.Easy  2.Normal  3.Hard" << endl;
	cin >> level;
	while (level != 1 && level != 2 && level != 3) {
		cout << "wtf?\n1.Easy  2.Normal  3.Hard" << endl;
		cin >> level;
	}

	mp1.close();
	system("cls");
	cout << "Don't be afraid.\nI stored SUN.\nNot only for PLANT." << endl;
	MusicPlayer mp2("2.wav", true);
	assert(mp2.play());
	Sleep(3000);
}

void Groove::create() {
	if (allPlant.size() == 10)
		return;
	int x = random(200 * level);
	if (x > 9)
		return;
	Plant* plant;
	switch (x)
	{
	case 0:plant = new Sunflower(-1, -1); break;
	case 1:plant = new NormalShoot(-1, -1); break;
	case 2:plant = new DoubleShoot(-1, -1); break;
	case 3:plant = new IceShoot(-1, -1); break;
	case 4:plant = new Pumpkin(-1, -1); break;
	case 5:plant = new Chili(-1, -1); break;
	case 6:plant = new Cherry(-1, -1); break;
	case 7:plant = new Onion(-1, -1); break;
	case 8:plant = new SmallNut(-1, -1); break;
	case 9:plant = new HighNut(-1, -1); break;
	default:
		break;
	}
	allPlant.push_back(plant);
}

void Groove::print() {
	cout << "|| [ ";
	for (int i = 0; i < allPlant.size(); i++) {
		if (store && i == chosen_plant) {
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
			cout << i << '.';
			allPlant[i]->print();
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		}
		else {
			cout << i << '.';
			allPlant[i]->print();
		}
	}
	cout << "] ||" << endl;
}

void Groove::plant(int x, int y, int i) {
	if (i < allPlant.size() && !yard[x][y].isPlant) {
		yard[x][y].plant = allPlant[i];
		yard[x][y].plant->change_position(x, y);
		allPlant.erase(allPlant.begin() + i);
		yard[x][y].isPlant = true;
	}
}