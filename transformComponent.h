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

	int x() const { return position.x; }
	int y() const { return position.y; }

	void setPos(float x, float y) {
		position.x = x;
		position.y = y;
	}

	void update() override {
		// 180 * PI to convert angle to radian
		// -90 since the imgs is import 90 degree
		float angleRad = (angle - 90) / 180 * M_PI;
		position.x += static_cast<float>(velocity.x * cos(angleRad) * speed);
		position.y += static_cast<float>(velocity.y * sin(angleRad) * speed);

		corners[0] = { position.x, position.y };
		corners[1] = { position.x + width, position.y };
		corners[2] = { position.x + width, position.y + height };
		corners[3] = { position.x, position.y + height };
	}
};