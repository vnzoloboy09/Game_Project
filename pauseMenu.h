#pragma once

#include "menu.h"

class PauseMenu : public Menu {
private:
	SDL_Texture* title;

public:
	PauseMenu();
	~PauseMenu();

	void init() override;
	void update() override;
	void mouseClickEvent(Button* button) override;
	void mouseEvent() override;
	void handleEvent() override;
	void keyEvent() override;
	void render() override;
};