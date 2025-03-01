#pragma once

#include <SDL.h>
#include "graphics.h"

class Background {
private:
	SDL_Texture *texture;
	int scrollingOffset = 0;
	SDL_Rect srcRect, destRect;

public:
	Background(const char* path, int y);
	void scroll(int scrollspeed);
	void render();
	void update(int scrollspeed);

	~Background();
};