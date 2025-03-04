#pragma once

#include <SDL_image.h>
#include "game.h"

class Graphics {
public:
	Graphics();
	~Graphics();

	static SDL_Texture* loadTexture(const char* path);
	static void draw(SDL_Texture* texture, SDL_Rect scrRect, SDL_Rect destRect);
	static void setColor(Color color);
	static void render(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect,
		double angle, SDL_Point* center, SDL_RendererFlip flip);
};