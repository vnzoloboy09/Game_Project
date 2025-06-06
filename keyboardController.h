#pragma once

#include "audio.h"
#include "game.h"
#include "components.h"

class KeyboardController : public Component {
private:
	TransformComponent* transform;
	bool active = true;
	Mix_Chunk* hornChunk;
	Uint32 cooldown = 200; // miliseconds
	Uint32 lastPress = 0;

public:
	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		hornChunk = Audio::loadSound("chunks/car_horn.wav");
	}

	void update() override {
		if (!active) return;
		const Uint8* keystate = SDL_GetKeyboardState(NULL);
		// 180 * PI to convert angle to radian
		// -90 since the imgs is import at 90 degree rather than 0 degree
		float angleRad = (transform->angle - 90) / 180 * M_PI;

		if (keystate[SDL_SCANCODE_W]) {
			transform->velocity = { cos(angleRad), sin(angleRad) };
		}
		if (keystate[SDL_SCANCODE_S]) {
			transform->velocity = { -cos(angleRad), -sin(angleRad) };
		}

		if (keystate[SDL_SCANCODE_D]) {
			if (keystate[SDL_SCANCODE_W]) transform->angle += STEER_ANGEL;
			if (keystate[SDL_SCANCODE_S]) transform->angle -= STEER_ANGEL;
		}

		if (keystate[SDL_SCANCODE_A]) {
			if (keystate[SDL_SCANCODE_W]) transform->angle -= STEER_ANGEL;
			if (keystate[SDL_SCANCODE_S]) transform->angle += STEER_ANGEL;
		}

		if (!keystate[SDL_SCANCODE_W] && !keystate[SDL_SCANCODE_S]) {
			transform->velocity = { 0.0f, 0.0f };
		}
		if (keystate[SDL_SCANCODE_SPACE] && SDL_GetTicks() - lastPress >= cooldown) {
			Audio::play(hornChunk);
			lastPress = SDL_GetTicks();
		}
	}

	void activate() { active = true; }
	void deactivate() { active = false; }
};