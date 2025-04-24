#pragma once

#include "components.h"

class TransformComponent : public Component {
public:
	Vector2D position;
	std::vector<SDL_FPoint> corners; 
	// position is just the place of the car on the map and sprite will render according to the position
	// corners are the car actual state and direction in the map
	// corners contribute to the collision detection
	// position is for rendering
	
	Vector2D velocity;
	int width = CAR_WIDTH;
	int height = CAR_HEIGHT;
	float angle = 0.0f; // degree
	float speed = ENEMY_SPEED;

	bool stopped = false;

	TransformComponent() {
		position.Zero();
		velocity.Zero();
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

	SDL_FPoint rotatePoint(SDL_FPoint point, float cenx, float ceny, float angle) {
		float radian = angle / 180 * M_PI;
		float s = sin(radian);
		float c = cos(radian);

		// translate point back to origin:
		point.x -= cenx;
		point.y -= ceny;

		// rotate point
		float xnew = point.x * c - point.y * s;
		float ynew = point.x * s + point.y * c;

		// translate point back:
		point.x = xnew + cenx;
		point.y = ynew + ceny;

		return point;
	};

	void init() override {
		velocity.Zero();

		corners.push_back({ position.x, position.y });
		corners.push_back({ position.x + width, position.y });
		corners.push_back({ position.x + width, position.y + height });
		corners.push_back({ position.x, position.y + height });
	}

	void setPos(float x, float y) {
		position.x = x;
		position.y = y;
		corners[0] = { position.x, position.y };
		corners[1] = { position.x + width, position.y };
		corners[2] = { position.x + width, position.y + height };
		corners[3] = { position.x, position.y + height };
	}

	void stop() {
		stopped = true;
	}
	
	void start() {
		stopped = false;
	}	

	void update() override {
		if (stopped) return;

		position.x += static_cast<float>(velocity.x * speed);
		position.y += static_cast<float>(velocity.y * speed);

		corners[0] = { position.x, position.y };
		corners[1] = { position.x + width, position.y };
		corners[2] = { position.x + width, position.y + height };
		corners[3] = { position.x, position.y + height };

		for (int i = 0; i < 4; i++) {
			corners[i] = rotatePoint(corners[i], static_cast<int>(position.x + width / 2.0f),
				static_cast<int>(position.y + height / 2.0f), angle);
		}
	}
};