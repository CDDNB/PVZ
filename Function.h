#pragma once
struct Location {
	int hloc, vloc;
};

int random(int x);

void init();

void check_yard(int hloc, int vloc);

void freshen();

void print_yard();

bool end_game();

void makesun(int &i, int n);//Ì«Ñô

bool buy_plant();

void create_corpse();

bool isPlant(int hloc, int vloc);

bool isCorpse(int hloc, int vloc);

bool isCorpseLine(int hloc, int vloc);

void end();

bool isHighNut(int hloc, int vloc);

bool isPlantLine(int hloc, int vloc);

void restart();

void writescore();

void startscreen();