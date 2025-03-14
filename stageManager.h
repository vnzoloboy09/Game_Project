#pragma once

#include "game.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

class StageManager {
private:
	SDL_Window* window;
	Game* game;
	bool running;

public:
	StageManager();
	~StageManager();

	void initSDL();
	void init();
	bool isRunning() const;
	void presentGameStage();
	void presentMenuStage();
	void presentStage();
	void handleEvent();
	void clear();
	

	static SDL_Renderer* renderer;
	static unsigned int stage;
	static SDL_Event event;
	static TTF_Font* font;
};
