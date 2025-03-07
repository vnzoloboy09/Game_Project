#pragma once

#include "components.h"

class TransformComponent : public Component {
public:
	Vector2D position;
	Vector2D velocity;
	std::vector<SDL_FPoint> corners;
	int width = 32;
	int height = 64;
	float angle = 0.0f;
	float speed = 4.0f;

	TransformComponent() {
		position.Zero();
	}

	TransformComponent(float x, float y) {
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int w, int h) {
		position.x = x;
		position.y = y;
		width = w;
		height = h;
	}

	TransformComponent(float x, float y, int w, int h, float sp) {
		position.x = x;
		position.y = y;
		width = w;
		height = h;
		speed = sp;
	}

	void init() override {
		velocity.Zero();

		corners.push_back({ position.x, position.y });
		corners.push_back({ position.x + width, position.y });
		corners.push_back({ position.x + width, position.y + height });
		corners.push_back({ position.x, position.y + height });
	}

	void update() override {
		// 180 * PI to convert angle to radian
		position.x += static_cast<float>(velocity.x * sin(angle / 180 * PI) * speed);
		position.y += static_cast<float>(velocity.y * cos(angle / 180 * PI) * speed);

		corners[0] = { position.x, position.y };
		corners[1] = { position.x + width, position.y };
		corners[2] = { position.x + width, position.y + height };
		corners[3] = { position.x, position.y + height };
	}
};