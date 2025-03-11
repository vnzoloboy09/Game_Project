#pragma once

#include "game.h"
#include "components.h"

class KeyboardController : public Component {
private:
	TransformComponent* transform;

public:

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override {
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		// 180 * PI to convert angle to radian
		// -90 since the imgs is import 90 degree rather then 0 degree
		float angleRad = (transform->angle - 90) / 180 * M_PI;

		if (keystate[SDL_SCANCODE_W]) transform->velocity = { cos(angleRad), sin(angleRad)};
		if (keystate[SDL_SCANCODE_S]) transform->velocity = { -cos(angleRad), -sin(angleRad) };

		if (keystate[SDL_SCANCODE_D]) {
			if (keystate[SDL_SCANCODE_W]) transform->angle += 1.5f;
			if (keystate[SDL_SCANCODE_S]) transform->angle -= 1.5f;
		}

		if (keystate[SDL_SCANCODE_A]) {
			if (keystate[SDL_SCANCODE_W]) transform->angle -= 1.5f;
			if (keystate[SDL_SCANCODE_S]) transform->angle += 1.5f;
		}

		if (!keystate[SDL_SCANCODE_W] && !keystate[SDL_SCANCODE_S]) {
			transform->velocity = { 0.0f, 0.0f };
		}
	}
};