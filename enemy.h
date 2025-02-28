#pragma once

#include "car.h"

class Enemy : public Car {
public:
	Enemy(int x, int y, const char* path);

	void update() override;
	void render() override;

	~Enemy();
};