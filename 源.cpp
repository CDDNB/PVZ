#include<iostream>
#include<vector>
#include<windows.h>
#include"Function.h"
using namespace std;

int main() {
	srand(time(NULL));
	startscreen();
	extern bool ENDD;
	int suntime = 0;//Ì«Ñô¼ÆÊý
	init();
	
	while (!end_game() && !ENDD) {
		makesun(suntime, 5);
		if (buy_plant()) {
			//system("cls");
			return 0;
			break;
		}
		create_corpse();
		freshen();
		print_yard();
		Sleep(250);
		system("cls");
	}
	end();
	writescore();
	return 0;
}