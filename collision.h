#pragma once

#include <vector>
#include <SDL.h>

class Collision {
public:
	static bool isCollidingSAT(std::vector<SDL_FPoint> a, std::vector<SDL_FPoint> b);
};