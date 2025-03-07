#pragma once

#include "components.h"

class ColliderComponent : public Component {
public:
	std::vector<SDL_FPoint> collider;
	TransformComponent* transform;

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
		for (int i = 0; i < transform->corners.size(); i++) {
			collider.push_back(transform->corners[i]);
		}
	}

	SDL_FPoint rotatePoint(SDL_FPoint point, float cenx, float ceny, float angle) {
		float radian = angle / 180 * PI;
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

	void update() override {
		for (int i = 0; i < collider.size(); i++) {
			collider[i] = rotatePoint(collider[i], transform->position.x + transform->width / 2.0f,
				transform->position.y + transform->height / 2.0f, transform->angle);
		}
	}
};