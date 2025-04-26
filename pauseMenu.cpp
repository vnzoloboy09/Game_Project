#include "pauseMenu.h"
#include "graphics.h"
#include "audio.h"
#include "stageManager.h"
#include "game.h"

PauseMenu::PauseMenu() {
	background = Graphics::loadTexture("imgs/menu/pause_background.png");
	title = Graphics::loadTexture("imgs/menu/pause_title.png");
	srcRect = { 0, 0, 0, 0 };
	destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

PauseMenu::~PauseMenu() {
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(title);
}

void PauseMenu::init() {
	buttonClicked = Audio::loadSound("chunks/click_button.wav");

	// the numbers are the button {xpos, ypos, width, height}

	buttons.push_back(new Button("imgs/menu/continue_button.png",
		200, 200, 300, 100, "continue"));
	buttons.push_back(new Button("imgs/menu/restart_button.png",
		200, 350, 300, 100, "restart"));
	buttons.push_back(new Button("imgs/menu/exit_button.png",
		200, 500, 300, 100, "exit"));
	buttons.push_back(new Button("imgs/menu/speaker_button.png", 
		900, SCREEN_HEIGHT / 2 - 32, 64, 64, "speaker"));
	
	buttons[3]->hoverOff(); // 3 is speaker button
}

void PauseMenu::keyEvent() {
	switch (StageManager::event.type) {
	case SDL_QUIT:
		StageManager::quit();
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

void PauseMenu::mouseClickEvent(Button* button) {
	Audio::play(buttonClicked);
	std::string tag = button->getTag();
	if (tag == "continue") {
		deactivate();
	}
	else if (tag == "restart") {
		Game* game = dynamic_cast<Game*>(StageManager::stages["Game"].get());
		game->reInit();
		deactivate();
	}
	else if (tag == "exit") {
		StageManager::changeStage("Menu");
	}
	else if (button->getTag() == "speaker") {
		Audio::mute = !Audio::mute;
		if (!Audio::mute) {
			if (Audio::volume == 0) Audio::volume = MAX_VOLUME / 2;
			button->setTex("imgs/menu/speaker_button.png");
			Mix_ResumeMusic();
		}
		else button->setTex("imgs/menu/mute_speaker_button.png");
	}
}

void PauseMenu::mouseEvent() {
	SDL_GetMouseState(&mouse.x, &mouse.y);

	for (auto button : getButtons()) {
		if (button->isHover(mouse.x, mouse.y)) {
			if (button->getTag() == "speaker" && StageManager::event.type == SDL_MOUSEWHEEL) {
				mouseWheelEvent();
			}
			if (StageManager::event.type == SDL_MOUSEBUTTONDOWN) {
				mouseClickEvent(button);
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
	if (Audio::mute) {
		Mix_PauseMusic();
	}
	else {
		Mix_VolumeMusic(Audio::volume);
	}
}

void PauseMenu::render() {
	Graphics::makeTransparent(background, 140);
	Graphics::draw(background, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	Graphics::draw(title, SCREEN_WIDTH / 2 - 150, 50, 300, 100);
	for (auto button : buttons) {
		if (button->getTag() == "speaker") {
			if (Audio::volume == 0) Audio::mute = true;
			if (!Audio::mute) button->setTex("imgs/menu/speaker_button.png");
			else button->setTex("imgs/menu/mute_speaker_button.png");
		}
		button->render();
	}

	renderVolumeControl(932, 360); // 932 and 360 are the center of speaker button
}