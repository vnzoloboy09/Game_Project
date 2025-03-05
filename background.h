#pragma once

#include <SDL.h>
#include "graphics.h"

class Background {
private:
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

public:
	Background(const char* path, int x, int y);

	void render();
	void update(float x, float y);

	~Background();
};