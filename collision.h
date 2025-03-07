#pragma once

#include <vector>
#include <SDL.h>

class ColliderComponent;

class Collision {
public:
	static bool isCollidingSAT(const std::vector<SDL_FPoint> a, const std::vector<SDL_FPoint> b);
	static bool isCollidingSAT(const ColliderComponent& colA, const ColliderComponent& colB);
};