#include "menu.h"
#include "graphics.h"
#include "stageManager.h"

Menu::Menu() {
	srcRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

void Menu::init() {
	background = Graphics::loadTexture("imgs/menu/background.png");
	Button* button = new Button("imgs/menu/play_button.png", 0, 0, 200, 100, "play");
	buttons.push_back(button);
	button = new Button("imgs/menu/choose_button.png", 500, 500, 300, 100, "choose");
	buttons.push_back(button);
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