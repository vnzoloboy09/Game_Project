#pragma once

#include <SDL_image.h>
#include <vector>
#include "button.h"
#include "stage.h"

class Menu : public Stage{
protected:
	SDL_Texture* background;

	SDL_Rect srcRect, destRect;
	std::vector<Button*> buttons;
	SDL_Point mouse;

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
