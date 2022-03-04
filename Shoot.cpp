#include"Shoot.h"
//int NormalShoot::count = 0, DoubleShoot::count = 0, IceShoot::count = 0, Sunflower::count = 0;
Bullet* Shoot::shoot() {
	if (shoot_time < 60 / speed) {
		shoot_time++;
		return NULL;
	}
	shoot_time = 0;
	Bullet* bullet = new Bullet(HLoc, VLoc, attack, 0);
	return bullet;
}

Bullet* DoubleShoot::shoot() {
	if (shoot_time < 60 / speed) {
		shoot_time++;
		return NULL;
	}
	shoot_time = 0;
	Bullet* bullet = new Bullet(HLoc, VLoc, attack, 0);
	return bullet;
}

Bullet* IceShoot::shoot() {
	if (shoot_time < 60 / speed) {
		shoot_time++;
		return NULL;
	}
	shoot_time = 0;
	Bullet* bullet = new Bullet(HLoc, VLoc, attack, 2);
	return bullet;
}

Bullet* Sunflower::shoot() {
	if (shoot_time < 60 / speed) {
		shoot_time++;
	}
	else {
		shoot_time = 0;
		extern int AllSun;
		AllSun += 5;
	}
	return NULL;
}