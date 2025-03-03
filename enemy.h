#pragma once

#include "car.h"

class Enemy : public Car {
public:
	Enemy(int x, int y, int _first_sprite);

	void update() override;
	void render() override;
	void setPosition(Vector2D pos);
	void addSpeed(float s);

	~Enemy();
};