#pragma once

#include "car.h"

class Player : public Car {
public:
	Player(int x, int y, const char* path);

	void control();
	void update() override;
	void render() override;

	~Player();
};