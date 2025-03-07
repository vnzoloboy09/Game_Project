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

		if (keystate[SDL_SCANCODE_W]) transform->velocity = { 1, -1 };
		if (keystate[SDL_SCANCODE_S]) transform->velocity = { -1, 1 };

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