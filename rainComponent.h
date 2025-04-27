#pragma once

#include "components.h"
#include "stageManager.h"
#include "game.h"

class RainComponent : public Component {
private:
	TransformComponent* transform;

	SDL_Rect srcRect, destRect;
	SDL_Rect hl_srcRect, hl_destRect;

	SDL_Texture* rainBgTexture = NULL;
	SDL_Texture* rainTexture = NULL;
	SDL_Texture* lightningTexture = NULL;
	SDL_Texture* headlightTexture = NULL;

	Mix_Chunk* lightningChunk = NULL;

	int rainBg_transparency = 0;
	int lightning_trasparency = 0;
	int* posx, * posy;
	int frames = 4;

	bool raining = false;
	bool* check = NULL;
	bool raining_before = false;
	bool lightningInc = false;
	bool lightningStriking = false;

	bool headlight_on = false;
	bool cooldown = false;
	float* angle = NULL;

	Vector2D* player_pos;
	SDL_Point headlight_center;

public:
	RainComponent() = default;

	RainComponent(int* x, int* y, bool* ck) {
		posx = x;
		posy = y;
		check = ck;

		srcRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		destRect = { 0, 0, SCREEN_WIDTH, SCREEN_WIDTH };
	}

	RainComponent(int* x, int* y, bool* ck, Vector2D* hl_pos, float *angl) {
		posx = x;
		posy = y;
		check = ck;

		srcRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		destRect = { 0, 0, SCREEN_WIDTH, SCREEN_WIDTH };

		hl_srcRect = { 0,0,0,0 };
		hl_destRect = { 0, 0, CAR_WIDTH * 3, CAR_HEIGHT };
		player_pos = hl_pos;
		angle = angl;
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
		headlightTexture = Graphics::loadTexture("imgs/car/headlight.png");
		Graphics::makeTransparent(headlightTexture, 150);
	}

	void reinit() {
		*check = false;
		raining = false;
		raining_before = false;
		rainBg_transparency = 0;

		lightningStriking = false;
		lightning_trasparency = 0;
		lightningInc = false;

		headlight_on = false;
	}

	void headlightUpdate() {
		//SDL_PollEvent(&StageManager::event);
		switch (StageManager::event.type) {
		case SDL_KEYDOWN:
			if (!cooldown && StageManager::event.key.keysym.sym == SDLK_h) {
				headlight_on = !headlight_on;
				cooldown = true;
			}
		case SDL_KEYUP:
			if (StageManager::event.key.keysym.sym == SDLK_h) {
				headlight_on = !headlight_on;
				cooldown = false;
			}
		default:
			break;
		}

		hl_destRect.x = player_pos->x - CAR_WIDTH - Game::camera.x;
		hl_destRect.y = player_pos->y - CAR_HEIGHT - Game::camera.y;
		headlight_center.x = static_cast<int>(player_pos->x + CAR_WIDTH / 2.0f) - Game::camera.x - hl_destRect.x;
		headlight_center.y = static_cast<int>(player_pos->y + CAR_HEIGHT / 2.0f) - Game::camera.y - hl_destRect.y;
	}

	void lightningUpdate() {
		if (raining) {
			if (!lightningStriking) {
				int r = rand() % LIGHTNING_STRIKE_CHANCE;
				if (!r) {
					lightningStriking = true;
					Audio::play(lightningChunk);
				}
				//std::cerr << r << '\n';
			}
			if (lightningStriking) lightningStrike();
		}
		Graphics::makeTransparent(lightningTexture, lightning_trasparency);
	}

	void rainUpdate() {
		if (*check) {
			int r = rand() % (raining ? STOP_RAINNING_CHANCE : START_RAINNING_CHANCE);
			if (r) raining = false;
			else raining = true;
			//std::cerr << "rain: " << raining << '\n';
			*check = false;
		}
		if (raining && !raining_before) {
			rainBg_transparency += 1;
			if (rainBg_transparency >= 180) raining_before = true;
		}
		if (!raining && raining_before) {
			rainBg_transparency -= 1;
			if (rainBg_transparency <= 0) raining_before = false;
		}
		transform->setPos(*posx, *posy);
		srcRect.x = srcRect.w * (static_cast<int>(SDL_GetTicks() / RAIN_SPEED) % frames);
		
		Graphics::makeTransparent(rainBgTexture, rainBg_transparency);
		Graphics::makeTransparent(rainTexture, rainBg_transparency);
	}

	void update() override {
		rainUpdate();
		lightningUpdate();
		headlightUpdate();
	}

	void render() override {
		Graphics::draw(rainBgTexture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		
		Graphics::render(rainTexture, srcRect, destRect, 0, SDL_FLIP_NONE);
		
		if(headlight_on) Graphics::render(headlightTexture, hl_srcRect, hl_destRect, 
			*angle, headlight_center, SDL_FLIP_NONE);
		
		if (raining) {
			if (lightningStriking) {
				Graphics::draw(lightningTexture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			}
		}
	}

	~RainComponent() {
		SDL_DestroyTexture(rainBgTexture);
		SDL_DestroyTexture(rainTexture);
		SDL_DestroyTexture(lightningTexture);
		Mix_FreeChunk(lightningChunk);
	}
};