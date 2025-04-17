#include "collision.h"
#include "colliderComponent.h"

bool Collision::isCollidingSAT(const std::vector<SDL_FPoint> a_conrners, const std::vector<SDL_FPoint> b_corners) {
	// loop through the 4 a's corners
	for (int i = 0; i < a_conrners.size(); i++) {
		int next = (i + 1) % a_conrners.size();
		SDL_FPoint axis = { a_conrners[next].x - a_conrners[i].x, 
			a_conrners[next].y - a_conrners[i].y };
		SDL_FPoint normal = { -axis.y, axis.x };

		float minA = INFINITY;
		float maxA = -INFINITY;
		// project a's corners to the normal vector
		for (int j = 0; j < a_conrners.size(); j++) {
			float projection = normal.x * a_conrners[j].x + normal.y * a_conrners[j].y;
			if (projection < minA) minA = projection;
			if (projection > maxA) maxA = projection;
		}

		float minB = INFINITY;
		float maxB = -INFINITY;
		// project b's corners to the normal vector
		for (int j = 0; j < b_corners.size(); j++) {
			float projection = normal.x * b_corners[j].x + normal.y * b_corners[j].y;
			if (projection < minB) minB = projection;
			if (projection > maxB) maxB = projection;
		}
		
		// check if the projections are overlaping
		if (maxA < minB || maxB < minA) return false;
	}

	// the code below has the same logic as the above, lazy to write a function for it
	// loop through the 4 b's corners same as a's
	for (int i = 0; i < b_corners.size(); i++) {
		int next = (i + 1) % b_corners.size();
		SDL_FPoint axis = { b_corners[next].x - b_corners[i].x, 
			b_corners[next].y - b_corners[i].y };
		SDL_FPoint normal = { -axis.y, axis.x };

		float minA = INFINITY;
		float maxA = -INFINITY;
		// project a's corners to the normal vector
		for (int j = 0; j < a_conrners.size(); j++) {
			float projection = normal.x * a_conrners[j].x + normal.y * a_conrners[j].y;
			if (projection < minA) minA = projection;
			if (projection > maxA) maxA = projection;
		}

		float minB = INFINITY;
		float maxB = -INFINITY;
		// project b's corners to the normal vector
		for (int j = 0; j < b_corners.size(); j++) {
			float projection = normal.x * b_corners[j].x + normal.y * b_corners[j].y;
			if (projection < minB) minB = projection;
			if (projection > maxB) maxB = projection;
		}

		// check if the projections are overlaping
		if (maxA < minB || maxB < minA) return false;
	}

	return true;
}

bool Collision::isCollidingSAT(const ColliderComponent& colA, const ColliderComponent& colB) {
	//std::cerr << colA.active << ' ' << colB.active << '\n';
	if (!colA.active || !colB.active) return false; // if the collider is not active then skip it
	
	std::vector<SDL_FPoint> a_corners = colA.collider;
	std::vector<SDL_FPoint> b_corners = colB.collider;

	if (isCollidingSAT(a_corners, b_corners)) {
		std::cerr << colA.tag << " hit " << colB.tag << '\n';
		return true;
	}
	return false;
}