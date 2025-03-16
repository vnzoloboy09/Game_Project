#include "menu.h"
#include "graphics.h"
#include "stageManager.h"
#include <iostream>

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

void Menu::reInit() {}

void Menu::update() {}

void Menu::handleEvent() {
	SDL_PollEvent(&StageManager::event);
	SDL_GetMouseState(&mouse.x, &mouse.y);
	switch (StageManager::event.type) {
	case SDL_QUIT:
		StageManager::current_stage->deactivate();
		StageManager::running = false;
		break;

	default:
		break;
	}

	for (auto button : getButtons()) {
		if (button->isHover(mouse.x, mouse.y) &&
			StageManager::event.type == SDL_MOUSEBUTTONDOWN) {
			if (button->getTag() == "play") {
				StageManager::current_stage->deactivate();
				StageManager::changeStage("Game");
				break;
			}
			if (button->getTag() == "choose") {
				StageManager::current_stage->deactivate();
				StageManager::changeStage("ChooseMenu");
			}
		}
	}
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