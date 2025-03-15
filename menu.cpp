#include "menu.h"
#include "graphics.h"
#include "stageManager.h"

Menu::Menu() {
	srcRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

Menu::~Menu() {
	SDL_DestroyTexture(background);
}

void Menu::init() {
	background = Graphics::loadTexture("imgs/menu/background.png");
	// the numbers are the button {xpos, ypos, width, height}
	buttons.push_back(new Button("imgs/menu/play_button.png", 520, 270, 200, 100, "play"));
	buttons.push_back(new Button("imgs/menu/choose_button.png", 475, 450, 300, 100, "choose"));
}

std::vector<Button*> Menu::getButtons() {
	return buttons;
}

void Menu::render() {
	SDL_RenderClear(StageManager::renderer);

	Graphics::draw(background, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	for (auto button : buttons) {
		button->render();
	}

	SDL_RenderPresent(StageManager::renderer);
}