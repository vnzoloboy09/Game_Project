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

	buttons[3]->hoverOff(); // 3 is speaker button

	buttonClicked = Graphics::loadSound("chunks/click_button.wav");
}

void Menu::reInit() {}

void Menu::update() {
	StageManager::getHightestScore();

	std::string text = "Hightest Score: " + std::to_string(StageManager::hightest_score);
	SDL_DestroyTexture(highestScore);
	highestScore = Graphics::loadText(text.c_str(), StageManager::font, { 0, 0, 0 });
	TTF_SizeText(StageManager::font, text.c_str(), &textRect.w, &textRect.h);
}

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
			if (button->getTag() == "speaker" && StageManager::event.type == SDL_MOUSEWHEEL) {
				if (StageManager::event.wheel.y > 0) {
					StageManager::volume += VOLUME_STEP;
					StageManager::volume = StageManager::volume > MAX_VOLUME ? MAX_VOLUME : StageManager::volume;
				}
				else if (StageManager::event.wheel.y < 0) {
					StageManager::volume -= VOLUME_STEP;
					StageManager::volume = StageManager::volume < 0 ? 0 : StageManager::volume;
				}
			}
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

void Menu::renderVolumeControl(int x, int y) {
	// render volume control
	if (!StageManager::mute) {
		Graphics::setColor(GREEN);
		Graphics::drawArc(x, y, 64, 0.0f, static_cast<float>(StageManager::volume) * DEGREE_PER_VOLUME);
		Graphics::drawArc(x, y, 65, 0.0f, static_cast<float>(StageManager::volume) * DEGREE_PER_VOLUME);
	} 
}

void Menu::render() {
	SDL_RenderClear(StageManager::renderer);

	Graphics::render(background, srcRect, destRect, 0, SDL_FLIP_NONE);
	Graphics::render(highestScore, tsrcRect, textRect, 0, SDL_FLIP_NONE);
	for (auto button : buttons) {
		if (button->getTag() == "speaker") {
			if (!StageManager::mute) button->setTex("imgs/menu/speaker_button.png");
			else button->setTex("imgs/menu/mute_speaker_button.png");
		}
		button->render();
	}
	renderVolumeControl(1132, 82); // 1132 and 82 are the speaker button center

	SDL_RenderPresent(StageManager::renderer);
}