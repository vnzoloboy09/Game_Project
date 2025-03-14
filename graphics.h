#pragma once

#include <SDL_image.h>
#include <SDL_ttf.h>
#include "defs.h"

class Graphics {
public:
	Graphics();
	~Graphics();

	static SDL_Texture* loadTexture(const char* path);
	static void draw(SDL_Texture* texture, int x, int y, int w, int h);
	static void setColor(Color color);
	// render sprites
	static void render(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect,
		float angle, SDL_RendererFlip flip);

	static TTF_Font* loadFont(const char* path, int size);
	// render text
	static SDL_Texture* loadText(const char* text, TTF_Font* font, SDL_Color textColor);
};