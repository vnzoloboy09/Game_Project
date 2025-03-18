#include "chooseMenu.h"
#include "graphics.h"
#include "stageManager.h"
#include "game.h"

ChooseMenu::ChooseMenu() {
	background = Graphics::loadTexture("imgs/menu/choose_background.png");
	srcRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

ChooseMenu::~ChooseMenu() {
	SDL_DestroyTexture(background);
}

void ChooseMenu::init() {
	buttons.push_back(new Button("imgs/car/yellow_car.png", 
		325, 300, CAR_WIDTH * scale, CAR_HEIGHT * scale, "yellow_car"));
	buttons.push_back(new Button("imgs/car/red_car.png", 
		580, 300, CAR_WIDTH * scale, CAR_HEIGHT * scale, "red_car"));
	buttons.push_back(new Button("imgs/car/blue_car.png", 
		835, 300, CAR_WIDTH * scale, CAR_HEIGHT * scale, "blue_car"));
}

void ChooseMenu::mouseEvent() {
	SDL_GetMouseState(&mouse.x, &mouse.y);

	for (auto button : getButtons()) {
		Game* game = dynamic_cast<Game*>(StageManager::stages["Game"].get());
		if (button->isHover(mouse.x, mouse.y) && StageManager::event.type == SDL_MOUSEBUTTONDOWN) {
			button->select(true);
			std::string tag = button->getTag();
			if (tag == "red_car") game->playerSkin = RED;
			else if (tag == "blue_car") game->playerSkin = BLUE;
			else game->playerSkin = YELLOW;
			for (auto other_button : getButtons()) {
				if (button->getTag() != other_button->getTag()) other_button->select(false);
			}
		}
	}
}

void ChooseMenu::keyEvent() {
	switch (StageManager::event.type) {
	case SDL_QUIT:
		StageManager::current_stage->deactivate();
		StageManager::running = false;
		break;
	case SDL_KEYDOWN:
		if (StageManager::event.key.keysym.sym == SDLK_ESCAPE) {
			StageManager::current_stage->deactivate();
			StageManager::changeStage("Menu");
		}
		break;
	default:
		break;
	}
}

void ChooseMenu::handleEvent() {
	SDL_PollEvent(&StageManager::event);
	keyEvent();
	mouseEvent();
}

void ChooseMenu::update() {}