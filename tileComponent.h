#pragma once

#include "components.h"

class SpriteComponent;

class TileComponent : public Component {
public:
	Vector2D posistion;
	SDL_Texture* texture;
	SDL_Rect scrRect, destRect;
	int tileID;
	const char* path;

	TileComponent() = default;

	TileComponent(int x, int y, int id) {
		tileID = id;
		switch (tileID) {
		case dirt:
			path = "imgs/tile/dirt.png";
			break;
		case light_grass:
			path = "imgs/tile/light_grass.png";
			break;
		case dark_grass:
			path = "imgs/tile/dark_grass.png";
			break;
		default:
			path = "imgs/tile/grass.png";
			break;
		}
		texture = Graphics::loadTexture(path);
		
		posistion.x = static_cast<float>(x);
		posistion.y = static_cast<float>(y);
		
		scrRect.x = 0;
		scrRect.y = 0;
		scrRect.w = TILE_SIZE;
		scrRect.h = TILE_SIZE;

		destRect.x = x;
		destRect.y = y;
		destRect.w = scrRect.w;
		destRect.h = scrRect.h;
	}

	~TileComponent() {
		SDL_DestroyTexture(texture);
	}

	void update() override {
		destRect.x = posistion.x - Game::camera.x;
		destRect.y = posistion.y - Game::camera.y;
	}

	void render() override {
		Graphics::render(texture, scrRect, destRect, 0, SDL_FLIP_NONE);
	}
};