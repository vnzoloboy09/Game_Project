#pragma once

#include "components.h";
#include "game.h"

class ExploderComponent : public Component {
private:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	TransformComponent* transform;

	bool exploding = false;
	const char* explosionPath = "imgs/car/explosion.png";
	int frames = 12;
	int frame = 0;
	int startTime = 0;

public:
	
	ExploderComponent() {
		texture = Graphics::loadTexture(explosionPath);
	}

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		
		srcRect.x = 0;
		srcRect.y = 0;
		srcRect.w = transform->width * 3;
		srcRect.h = transform->width * 3;
		destRect.x = static_cast<int>(transform->position.x);
		destRect.y = static_cast<int>(transform->position.y);
		destRect.w = srcRect.w;
		destRect.h = srcRect.h;
		
		exploding = false;
	}

	const char* getPath() {
		return explosionPath;
	}

	int getFrames() const {
		return frames;
	}

	void explode() { startTime = SDL_GetTicks();  exploding = true; }
	void stop() { exploding = false; }
	bool isExploding() const { return exploding; }

	void update() override {
		if (exploding) {
			frame = static_cast<int>((SDL_GetTicks() - startTime) / 100) % frames;
			if (frame == frames-1) {
				stop();
			}
			srcRect.x = frame * srcRect.w;

		}
		destRect.x = static_cast<int>(transform->position.x) - Game::camera.x;
		destRect.y = static_cast<int>(transform->position.y) - Game::camera.y;
	}

	void render() override {
		if (exploding) {
			Graphics::render(texture, srcRect, destRect, 0, SDL_FLIP_NONE);
		}
	}

	~ExploderComponent() = default;
};