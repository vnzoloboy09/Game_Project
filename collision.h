#pragma once

#include <SDL.h>
#include "car.h"

class Collision {
public:
	static bool isColliding(Car* a, Car* b);
	static void rotatePoint(float& x, float& y, float cenx, float ceny, float angle);
};