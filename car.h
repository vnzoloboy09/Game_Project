#pragma once

#include <SDL.h>

class Car {
private:
	SDL_Texture* carTexture;
	int velocity_x = 5;
	int velocity_y = 1;
	int xpos = 0;
	int ypos = 0;
	SDL_Rect srcRect, destRect;

public:
	Car(int x, int y, const char* path);
	~Car();
	void update();
	void render();
};