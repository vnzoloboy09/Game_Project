#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "car.h"

class Game {
private:
	SDL_Window* window;
	bool running = true;

public:
	Game();
	~Game();

	void initSDL();

	void init();
	bool isRunning();
	void update();
	void render();
	void handleEvent();
	void clear();

	static SDL_Renderer* renderer;
	static SDL_Event event;
};