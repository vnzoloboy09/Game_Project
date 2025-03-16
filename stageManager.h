#pragma once

#include <SDL_ttf.h>
#include <SDL_image.h>
#include <memory>
#include <unordered_map>
#include "stage.h"
#include <string>

class StageManager {
private:
	SDL_Window* window;
	SDL_Point mouse;

public:
	StageManager();
	~StageManager();

	void initSDL();
	void init();
	bool isRunning() const;
	void addStage(const std::string& name, std::unique_ptr<Stage> stage);

	void presentStage();
	void clear();
	
	static std::unordered_map<std::string, std::unique_ptr<Stage>> stages;
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static TTF_Font* font;
	static bool running;
	static Stage* current_stage;
	static void changeStage(const std::string& name);
};