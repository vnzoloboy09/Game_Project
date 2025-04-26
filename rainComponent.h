#pragma once

#include "components.h"

class RainComponent : public Component {
private:
	TransformComponent* transform;

	SDL_Rect srcRect, destRect;

	SDL_Texture* rainBgTexture = NULL;
	SDL_Texture* rainTexture = NULL;
	SDL_Texture* lightningTexture = NULL;

	Mix_Chunk* lightningChunk = NULL;

	int rainBg_transparency = 0;
	int lightning_trasparency = 0;
	int *posx, *posy;
	int frames = 4;

	bool raining = false;
	bool *check = NULL;
	bool raining_before = false;
	bool lightningInc = false;
	bool lightningStriking = false;

public:
	RainComponent() = default;

	RainComponent(int* x, int* y, bool* ck) {
		posx = x;
		posy = y;
		check = ck;

		srcRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		destRect = { 0, 0, SCREEN_WIDTH, SCREEN_WIDTH };
	}

	void lightningStrike() {
		if (lightningInc) {
			lightning_trasparency += 15;
			if (lightning_trasparency >= LIGHTNING_MAX) lightningInc = false;
		}
		else {
			lightning_trasparency -= 10;
			if (lightning_trasparency <= 0) {
				lightning_trasparency = 0;
				lightningInc = true;
				lightningStriking = false;
			}
		}
	};

	void init() override {
		transform = &entity->getComponent<TransformComponent>();
		lightningChunk = Audio::loadSound("chunks/lightning.wav");
		lightningTexture = Graphics::loadTexture("imgs/ob/lightning.png");
		rainBgTexture = Graphics::loadTexture("imgs/menu/pause_background.png");
		rainTexture = Graphics::loadTexture("imgs/ob/rain.png");
	}

	void reinit() {
		*check = false;
		raining = false;
		raining_before = false;
		rainBg_transparency = 0;

		lightningStriking = false;
		lightning_trasparency = 0;
		lightningInc = false;
	}

	void update() override {
		if (*check) {
			int r = rand() % (raining ? STOP_RAINNING_CHANCE : START_RAINNING_CHANCE);
			if (r) raining = false;
			else raining = true;
			//std::cerr << "rain: " << raining << '\n';
			*check = false;
		}

		transform->setPos(*posx, *posy);
		srcRect.x = srcRect.w * (static_cast<int>(SDL_GetTicks() / 100) % frames);

		if (raining) {
			if (!lightningStriking) {
				int r = rand() % LIGHTNING_STRIKE_CHANCE;
				if (!r) {
					lightningStriking = true;
					Audio::play(lightningChunk);
				}
				//std::cerr << r << '\n';
			}
			else lightningStrike();
		}

		if (raining && !raining_before) {
			rainBg_transparency += 1;
			if (rainBg_transparency >= 180) raining_before = true;
		}
		if (!raining && raining_before) {
			rainBg_transparency -= 1;
			if (rainBg_transparency <= 0) raining_before = false;
		}

		Graphics::makeTransparent(rainBgTexture, rainBg_transparency);
		Graphics::makeTransparent(rainTexture, rainBg_transparency);
		Graphics::makeTransparent(lightningTexture, lightning_trasparency);
	}

	void render() override {
		Graphics::draw(rainBgTexture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		if (raining) {
			if (lightningStriking) {
				Graphics::draw(lightningTexture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			}
		}
		Graphics::render(rainTexture, srcRect, destRect, 0, SDL_FLIP_NONE);
	}

	~RainComponent() {
		SDL_DestroyTexture(rainBgTexture);
		SDL_DestroyTexture(rainTexture);
		SDL_DestroyTexture(lightningTexture);
		Mix_FreeChunk(lightningChunk);
	}
};