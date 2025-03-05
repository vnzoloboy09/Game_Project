#include "collision.h"
#include "defs.h"

SDL_FPoint Collision::rotatePoint(SDL_FPoint point, float cenx, float ceny, float angle) {
	float radian = angle / 180 * PI;
	float s = sin(radian);
	float c = cos(radian);
	
	// translate point back to origin:
	point.x -= cenx;
	point.y -= ceny;
	
	// rotate point
	float xnew = point.x * c - point.y * s;
	float ynew = point.x * s + point.y * c;
	
	// translate point back:
	point.x = xnew + cenx;
	point.y = ynew + ceny;

	return point;
};

bool Collision::isCollidingSAT(Player* player, Enemy* enemy) {
	std::vector<SDL_FPoint> player_corners = player->getCorners();
	std::vector<SDL_FPoint> enemy_corners = enemy->getCorners();
	const float EPSILON = 0.0001f;

	// loop through the 4 player's corners
	for (int i = 0; i < 4; i++) {
		SDL_FPoint axis = player->getAxis(player_corners[i], player_corners[(i + 1) % 4]);

		// project the player's corners to the axis
		float minPlayer = FLT_MAX, maxPlayer = FLT_MIN;
		for (int j = 0; j < 4; j++) {
			float projection = player_corners[j].x * axis.x + player_corners[j].y * axis.y;
			if (projection < minPlayer) minPlayer = projection;
			if (projection > maxPlayer) maxPlayer = projection;
		}
		
		// project the enemy's corners to the axis
		float minEnemy = FLT_MAX, maxEnemy = FLT_MIN;
		for (int j = 0; j < 4; j++) {
			float projection = enemy_corners[j].x * axis.x + enemy_corners[j].y * axis.y;
			if (projection < minEnemy) minEnemy = projection;
			if (projection > maxEnemy) maxEnemy = projection;
		}

		// check if the projections are overlapping
		if (maxPlayer < minEnemy - EPSILON|| maxEnemy < minPlayer - EPSILON) {
			return false;
		}
	}

	// loop throught the 4 enemy's corners
	for (int i = 0; i < 4; i++) {
		SDL_FPoint axis = enemy->getAxis(enemy_corners[i], enemy_corners[(i + 1) % 4]);

		// project the player's corners to the axis
		float minPlayer = FLT_MAX, maxPlayer = FLT_MIN;
		for (int j = 0; j < 4; j++) {
			float projection = player_corners[j].x * axis.x + player_corners[j].y * axis.y;
			if (projection < minPlayer) minPlayer = projection;
			if (projection > maxPlayer) maxPlayer = projection;
		}

		// project the enemy's corners to the axis
		float minEnemy = FLT_MAX, maxEnemy = FLT_MIN;
		for (int j = 0; j < 4; j++) {
			float projection = enemy_corners[j].x * axis.x + enemy_corners[j].y * axis.y;
			if (projection < minEnemy) minEnemy = projection;
			if (projection > maxEnemy) maxEnemy = projection;
		}

		// check if the projections are overlapping
		if (maxPlayer < minEnemy - EPSILON || maxEnemy < minPlayer - EPSILON) {
			return false;
		}
	}
	return true;
}

// some variables name might be conffusing, but it's just a copy of the above function.
// I'm too lazy to change the name.
// Work the same as the above function expect it takes 2 enemies.
bool Collision::isCollidingSAT(Enemy* enemy1, Enemy* enemy2) {
	std::vector<SDL_FPoint> player_corners = enemy1->getCorners();
	std::vector<SDL_FPoint> enemy_corners = enemy2->getCorners();

	// loop through the 4 enemy1's corners
	for (int i = 0; i < 4; i++) {
		SDL_FPoint axis = enemy1->getAxis(player_corners[i], player_corners[(i + 1) % 4]);

		// project the player's corners to the axis
		float minPlayer = FLT_MAX, maxPlayer = FLT_MIN;
		for (int j = 0; j < 4; j++) {
			float projection = player_corners[j].x * axis.x + player_corners[j].y * axis.y;
			if (projection < minPlayer) minPlayer = projection;
			if (projection > maxPlayer) maxPlayer = projection;
		}

		// project the enemy's corners to the axis
		float minEnemy = FLT_MAX, maxEnemy = FLT_MIN;
		for (int j = 0; j < 4; j++) {
			float projection = enemy_corners[j].x * axis.x + enemy_corners[j].y * axis.y;
			if (projection < minEnemy) minEnemy = projection;
			if (projection > maxEnemy) maxEnemy = projection;
		}

		// check if the projections are overlapping
		if (maxPlayer < minEnemy || maxEnemy < minPlayer) {
			return false;
		}
	}

	// loop throught the 4 enemy2's corners
	for (int i = 0; i < 4; i++) {
		SDL_FPoint axis = enemy2->getAxis(enemy_corners[i], enemy_corners[(i + 1) % 4]);

		// project the player's corners to the axis
		float minPlayer = FLT_MAX, maxPlayer = FLT_MIN;
		for (int j = 0; j < 4; j++) {
			float projection = player_corners[j].x * axis.x + player_corners[j].y * axis.y;
			if (projection < minPlayer) minPlayer = projection;
			if (projection > maxPlayer) maxPlayer = projection;
		}

		// project the enemy's corners to the axis
		float minEnemy = FLT_MAX, maxEnemy = FLT_MIN;
		for (int j = 0; j < 4; j++) {
			float projection = enemy_corners[j].x * axis.x + enemy_corners[j].y * axis.y;
			if (projection < minEnemy) minEnemy = projection;
			if (projection > maxEnemy) maxEnemy = projection;
		}

		// check if the projections are overlapping
		if (maxPlayer < minEnemy || maxEnemy < minPlayer) {
			return false;
		}
	}
	return true;
}