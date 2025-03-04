#pragma once

#include "defs.h"
#include <SDL_image.h>

class Game {
private:
	SDL_Window* window;
	bool running = true;
	float score;
	float score_flag = 5; // addSpeed every 10 points

public:
	Game();
	~Game();

	void initSDL();

	void init();
	bool isRunning() const;
	void gameOver();
	void update(); 
	void render();
	void handleEvent();
	void clear();

	static SDL_Renderer* renderer;
	static SDL_Event event;
};