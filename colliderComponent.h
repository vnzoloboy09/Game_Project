#pragma once

#include "components.h"

class ColliderComponent : public Component {
public:
	std::vector<SDL_FPoint> collider;
	TransformComponent* transform;
	const char* tag;

	ColliderComponent(const char* tag) {
		this->tag = tag;
	}

	void init() override {
		if (!entity->hasComponent<TransformComponent>()) {
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();
		for (int i = 0; i < transform->corners.size(); i++) {
			collider.push_back(transform->corners[i]);
		}

		Game::colliders.push_back(this);
	}

	void update() override {
		for (int i = 0; i < collider.size(); i++) {
			collider[i] = transform->corners[i];
		}
	}
};