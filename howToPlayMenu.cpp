#include "HowToPlayMenu.h"
#include "graphics.h"
#include "stageManager.h"
#include "game.h"

HowToPlayMenu::HowToPlayMenu() {
	background = Graphics::loadTexture("imgs/menu/how_to_play_menu.png");
	srcRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

HowToPlayMenu::~HowToPlayMenu() {
	SDL_DestroyTexture(background);
}

void HowToPlayMenu::init() {
	buttonClicked = Graphics::loadSound("chunks/click_button.wav");
	buttons.push_back(new Button("imgs/menu/back_button.png",
		16, 16, CAR_WIDTH * 2, CAR_WIDTH * 2, "back_button"));
}

void HowToPlayMenu::mouseEvent() {
	SDL_GetMouseState(&mouse.x, &mouse.y);

	for (auto button : getButtons()) {
		Game* game = dynamic_cast<Game*>(StageManager::stages["Game"].get());
		if (button->isHover(mouse.x, mouse.y)) {
			if (StageManager::event.type == SDL_MOUSEBUTTONDOWN) {
				Graphics::play(buttonClicked);
				std::string tag = button->getTag();
				if (tag == "back_button") {
					StageManager::changeStage("Menu");
					break;
				}
			}
		}
	}
}

void HowToPlayMenu::keyEvent() {
	switch (StageManager::event.type) {
	case SDL_QUIT:
		StageManager::quit();
		break;
	case SDL_KEYDOWN:
		if (StageManager::event.key.keysym.sym == SDLK_ESCAPE) {
			StageManager::changeStage("Menu");
		}
		break;
	default:
		break;
	}
}

void HowToPlayMenu::handleEvent() {
	SDL_PollEvent(&StageManager::event);
	keyEvent();
	mouseEvent();
}

void HowToPlayMenu::update() {}

void HowToPlayMenu::render() {
	SDL_RenderClear(StageManager::renderer);

	Graphics::render(background, srcRect, destRect, 0, SDL_FLIP_NONE);

	for (auto button : buttons) {
		button->render();
	}

	SDL_RenderPresent(StageManager::renderer);
}