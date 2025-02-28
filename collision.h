#pragma once

#include <SDL.h>
#include "car.h"

class Collision {
public:
	static bool isColliding(Car* a, Car* b);
};