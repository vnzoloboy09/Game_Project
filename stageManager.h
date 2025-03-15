#pragma once

#include "game.h"
#include "chooseMenu.h"
#include <SDL_ttf.h>
#include <SDL_image.h>

class StageManager {
private:
	SDL_Window* window;
	Game* game;
	Menu* menu;
	ChooseMenu* chooseMenu;
	bool running;
	bool stage_is_running;
	SDL_Point mouse;

public:
	StageManager();
	~StageManager();

	void initSDL();
	void init();
	bool isRunning() const;
	void stopCurrentStage();

	void presentGameStage();
	void presentMenuStage();
	void presentChooseMenuStage();
	void presentStage();


	void handleGameEvent();
	void handleMenuEvent();
	void handleChooseMenuEvent();
	void clear();
	
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static TTF_Font* font;
	static tStage stage;
};
