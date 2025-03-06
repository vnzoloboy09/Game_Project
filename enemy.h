#pragma once

#include "car.h"

class Enemy : public Car {
public:
	Enemy(int x, int y, int _first_sprite);

	void update() override;
	void render() override;
	void setPosition(int xpos, int ypos);
	void addSpeed(float s);
	void stop();
	void chasePlayer(Vector2D player_pos);

	~Enemy();
};