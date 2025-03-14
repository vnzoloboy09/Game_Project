#pragma once

#include <SDL_image.h>
#include <vector>
#include "button.h"

class Menu {
private:
	SDL_Texture* background;

	SDL_Rect srcRect, destRect;

	std::vector<Button*> buttons;

public:
	Menu();
	~Menu();

	void init();
	std::vector<Button*> getButtons();
	void update();
	void render();
};
