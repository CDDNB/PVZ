#include"bomb.h"
#include"MusicPlayer.h"

vector<Location> Bomb::bomb(bool b) {
	vector<Location>L;
	if (b) {
		MusicPlayer mp("boom.wav", false, "BOOM");
		mp.play();
		health = 0;
		Location loc;
		loc.hloc = HLoc;
		loc.vloc = VLoc;
		L.push_back(loc);
	}
	return L;
}

vector<Location> Cherry::bomb(bool b) {
	//health = 0;
	cnt++;
	vector<Location>L;
	if (cnt == time) {
		MusicPlayer mp("boom.wav", false, "BOOM");
		mp.play();
		health = 0;
		for (int i = HLoc - round / 2; i <= HLoc + round / 2; i++) {
			for (int j = VLoc - round / 2; j <= VLoc + round / 2; j++) {
				if (i >= 0 && i < 3 && j >= 0 && j < 7) {
					Location l;
					l.hloc = i; l.vloc = j;
					L.push_back(l);
				}
			}
		}
	}
	return L;
}

vector<Location>Chili::bomb(bool b) {
	//health = 0;
	cnt++;
	vector<Location>L;
	if (cnt == time) {
		MusicPlayer mp("burn.wav", false, "BURN");
		mp.play();
		health = 0;
		for (int i = 0; i < 7; i++) {
			Location l;
			l.hloc = HLoc;
			l.vloc = i;
			L.push_back(l);
		}
	}
	return L;
}