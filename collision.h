#pragma once
#include "player.h"
#include "enemy.h"

class Collision {
public:
	static bool isCollidingAABB(Player* a, Enemy* b);
	static bool isCollidingSAT(Player* a, Enemy* b);
	static SDL_FPoint rotatePoint(SDL_FPoint point, float cenx, float ceny, float angle);
};