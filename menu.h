#pragma once

#include <SDL_image.h>
#include <vector>
#include "button.h"

class Menu {
protected:
	SDL_Texture* background;

	SDL_Rect srcRect, destRect;
	std::vector<Button*> buttons;

public:
	Menu();
	~Menu();

	virtual void init();
	std::vector<Button*> getButtons();
	void render();
};
