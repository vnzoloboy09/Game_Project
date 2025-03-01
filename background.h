#pragma once

#include <SDL.h>
#include "graphics.h"

class Background {
private:
	SDL_Texture *texture;
	SDL_Rect srcRect, destRect;

public:
	Background(const char* path, int y);

	void render();
	void update(int scrollspeed);

	~Background();
};