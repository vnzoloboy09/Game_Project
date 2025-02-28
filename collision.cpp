#include "collision.h"

bool Collision::isColliding(Car* a, Car* b) {
	SDL_Rect recA = a->getRect();
	SDL_Rect recB = b->getRect();
	if (recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		) {
		return true;
	}
	return false;
}