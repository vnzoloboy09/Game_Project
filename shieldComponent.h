#pragma once

#include "components.h"
#include "game.h"

class ShieldComponent : public Component {
private:
	int shield_duration;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	SDL_Point center;
	Mix_Chunk* shieldChunk;
	TransformComponent* transform;
	int frames = 2;

public:
	ShieldComponent() {
		srcRect = { 0,0, SHIELD_WIDTH, SHIELD_HEIGHT };
		destRect = { 0,0, SHIELD_WIDTH, SHIELD_HEIGHT };
		shield_duration = 0;
	};
	
	~ShieldComponent() {
		SDL_DestroyTexture(texture);
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		texture = Graphics::loadTexture("imgs/ob/shield.png");
		shieldChunk = Audio::loadSound("chunks/shield.wav");
	}

	void shieldOn() {
		shield_duration = SHIELD_DURATON;
		srcRect.x = 0;
		Audio::play(shieldChunk);
	}

	void shieldOff() {
		shield_duration = 0;
	}

	bool hasShiled() const {
		return shield_duration > 0;
	}

	void update() override {
		if (shield_duration == 0) return;
		shield_duration--;
		if (shield_duration < SHIELD_DURATION_LOW) {
			srcRect.x = srcRect.w * (static_cast<int>(SDL_GetTicks() / SHIELD_SPRITE_SPEED) % frames);
		}

		destRect.x = static_cast<int>(transform->position.x) - 2 - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - 2 - Game::camera.y;

		center.x = CAR_WIDTH / 2 + 2;
		center.y = CAR_HEIGHT / 2 + 2;
	}

	void render() override {
		if (shield_duration) {
			Graphics::render(texture, srcRect, destRect, transform->angle, center, SDL_FLIP_NONE);
		}
	}
};