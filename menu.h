#pragma once

#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
#include "button.h"
#include "stage.h"

class Menu : public Stage{
protected:
	SDL_Texture* background;
	SDL_Texture* highestScore;

	SDL_Rect srcRect, destRect;
	SDL_Rect textRect = {10, 10, 0 ,0}, tsrcRect = { 0,0,0,0 };
	std::vector<Button*> buttons;
	SDL_Point mouse;
	Mix_Chunk* buttonClicked;

public:
	Menu();
	~Menu();

	void init() override;
	void reInit() override;
	void mouseEvent() override;
	void keyEvent() override;
	void handleEvent() override;
 	void update() override;
	std::vector<Button*> getButtons();
	void render();
};
