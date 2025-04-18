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
	buttons.push_back(new Button("imgs/menu/play_button.png", 520, 240, 200, 100, "play"));
	buttons.push_back(new Button("imgs/menu/choose_button.png", 475, 420, 300, 100, "choose"));
	buttons.push_back(new Button("imgs/menu/exit_button.png", 475, 600, 300, 100, "exit"));
	buttons.push_back(new Button("imgs/menu/speaker_button.png", 1100, 50, 64, 64, "speaker"));
	buttons.push_back(new Button("imgs/menu/turtorial_button.png", 900, 600, 300, 100, "how to play"));

	buttonClicked = Graphics::loadSound("chunks/click_button.wav");
}

void Menu::reInit() {}

void Menu::update() {}

void Menu::keyEvent() {
	switch (StageManager::event.type) {
	case SDL_QUIT:
		StageManager::quit();
		break;

	default:
		break;
	}
}

void Menu::mouseEvent() {
	SDL_GetMouseState(&mouse.x, &mouse.y);
	for (auto button : getButtons()) {
		if (button->isHover(mouse.x, mouse.y)) {
			if (StageManager::event.type == SDL_MOUSEBUTTONDOWN) {
				Graphics::play(buttonClicked);
				if (button->getTag() == "play") {
					StageManager::changeStage("Game");
					break;
				}
				else if (button->getTag() == "choose") {
					StageManager::changeStage("ChooseMenu");
				}
				else if (button->getTag() == "exit") {
					StageManager::quit();
				}
				else if (button->getTag() == "how to play") {
					StageManager::changeStage("HowToPlayMenu");
				}
				else if (button->getTag() == "speaker") {
					StageManager::mute = !StageManager::mute;
					if (!StageManager::mute) button->setTex("imgs/menu/speaker_button.png");
					else button->setTex("imgs/menu/mute_speaker_button.png");
				}
			}
		}
	}
}

void Menu::handleEvent() {
	SDL_PollEvent(&StageManager::event);
	keyEvent();
	mouseEvent();
}

std::vector<Button*> Menu::getButtons() {
	return buttons;
}

void Menu::render() {
	SDL_RenderClear(StageManager::renderer);

	Graphics::render(background, srcRect, destRect, 0, SDL_FLIP_NONE);
	for (auto button : buttons) {
		if (button->getTag() == "speaker") {
			if (!StageManager::mute) button->setTex("imgs/menu/speaker_button.png");
			else button->setTex("imgs/menu/mute_speaker_button.png");
		}
		button->render();
	}

	SDL_RenderPresent(StageManager::renderer);
}