#include "chooseMenu.h"
#include "graphics.h"

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
		575, 300, CAR_WIDTH * scale, CAR_HEIGHT * scale, "red_car"));
	buttons.push_back(new Button("imgs/car/blue_car.png", 
		825, 300, CAR_WIDTH * scale, CAR_HEIGHT * scale, "blue_car"));
}