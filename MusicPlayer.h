#pragma once
#include<iostream>
using namespace std;
class MusicPlayer
{
private:
	bool loop;
	string bgm, alias;
public:
	MusicPlayer(string a, bool t) :bgm(a), loop(t), alias("MUSIC1") {}
	MusicPlayer(string a, bool t, string _alias) :bgm(a), loop(t), alias(_alias) {}
	bool play();
	void stop();
	void close();
};

