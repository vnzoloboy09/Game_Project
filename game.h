#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"

class Game {
public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool running = true;
	SDL_Event event;

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
};