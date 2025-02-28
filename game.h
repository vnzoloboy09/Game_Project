#pragma once

#include <SDL_image.h>
#include "defs.h"

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
	bool isColliding(SDL_Rect& recA, SDL_Rect& recB);

	static SDL_Renderer* renderer;
	static SDL_Event event;
};