#include "pauseMenu.h"
#include "graphics.h"
#include "stageManager.h"

PauseMenu::PauseMenu() {
	background = Graphics::loadTexture("imgs/menu/choose_background.png");
	srcRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	destRect = { 100, 100, 200, 200 };
}

PauseMenu::~PauseMenu() {
	SDL_DestroyTexture(background);
}

void PauseMenu::init() {
	buttons.push_back(new Button("imgs/car/yellow_car.png",
		325, 300, CAR_WIDTH, CAR_HEIGHT, "test"));
}

void PauseMenu::keyEvent() {
	switch (StageManager::event.type) {
	case SDL_QUIT:
		StageManager::current_stage->deactivate();
		StageManager::running = false;
		break;
	case SDL_KEYDOWN:
		if (StageManager::event.key.keysym.sym == SDLK_ESCAPE) {
			deactivate();
		}
		break;
	default:
		break;
	}
}

void PauseMenu::mouseEvent() {
	SDL_GetMouseState(&mouse.x, &mouse.y);

	for (auto button : getButtons()) {
		if (button->isHover(mouse.x, mouse.y) && StageManager::event.type == SDL_MOUSEBUTTONDOWN) {
			std::string tag = button->getTag();
			if (tag == "test") {
				StageManager::current_stage->deactivate();
				StageManager::changeStage("Menu");
			}
		}
	}

}

void PauseMenu::handleEvent() {
	SDL_PollEvent(&StageManager::event);
	keyEvent();
	mouseEvent();
}

void PauseMenu::update() {

}

void PauseMenu::render() {
	Graphics::render(background, srcRect, destRect, 0, SDL_FLIP_NONE);
	for (auto button : buttons) {
		button->render();
	}
}