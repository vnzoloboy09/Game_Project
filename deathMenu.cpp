#include "deathMenu.h"
#include "graphics.h"
#include "audio.h"
#include "stageManager.h"
#include "game.h"

DeathMenu::DeathMenu() {
	background = Graphics::loadTexture("imgs/menu/pause_background.png");
	srcRect = { 0, 0, 0, 0 };
	destRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
}

DeathMenu::~DeathMenu() {
	SDL_DestroyTexture(background);
}

void DeathMenu::init() {
	buttonClicked = Audio::loadSound("chunks/click_button.wav");

	// the numbers are the button {xpos, ypos, width, height}

	buttons.push_back(new Button("imgs/menu/restart_button.png",
		SCREEN_WIDTH/2-150, 350, 300, 100, "restart"));
	buttons.push_back(new Button("imgs/menu/exit_button.png",
		SCREEN_WIDTH / 2 - 150, 500, 300, 100, "exit"));
	buttons.push_back(new Button("imgs/menu/speaker_button.png",
		SCREEN_WIDTH / 2 - 32, SCREEN_HEIGHT / 2 - 200, 64, 64, "speaker"));
	buttons[2]->hoverOff(); // 2 is speaker button
}

void DeathMenu::keyEvent() {
	switch (StageManager::event.type) {
	case SDL_QUIT:
		StageManager::quit();
		break;
	case SDL_KEYDOWN:
		if (StageManager::event.key.keysym.sym == SDLK_ESCAPE) {
			deactivate();
			StageManager::changeStage("Menu");
		}
		break;
	default:
		break;
	}
}

void DeathMenu::mouseEvent() {
	SDL_GetMouseState(&mouse.x, &mouse.y);
	Game* game = dynamic_cast<Game*>(StageManager::stages["Game"].get());

	for (auto button : getButtons()) {
		if (button->isHover(mouse.x, mouse.y)) {
			if (button->getTag() == "speaker" && StageManager::event.type == SDL_MOUSEWHEEL) {
				if (StageManager::event.wheel.y > 0) {
					Audio::volume += VOLUME_STEP;
					Audio::volume = Audio::volume > MAX_VOLUME ? MAX_VOLUME : Audio::volume;
				}
				else if (StageManager::event.wheel.y < 0) {
					Audio::volume -= VOLUME_STEP;
					Audio::volume = Audio::volume < 0 ? 0 : Audio::volume;
				}
			}
			if (StageManager::event.type == SDL_MOUSEBUTTONDOWN) {
				Audio::play(buttonClicked);
				std::string tag = button->getTag();
				if (tag == "restart") {
					game->reInit();
					deactivate();
				}
				else if (tag == "exit") {
					StageManager::changeStage("Menu");
				}
				else if (button->getTag() == "speaker") {
					Audio::mute = !Audio::mute;
					if (!Audio::mute) button->setTex("imgs/menu/speaker_button.png");
					else button->setTex("imgs/menu/mute_speaker_button.png");
				}
			}
		}
	}

}

void DeathMenu::handleEvent() {
	SDL_PollEvent(&StageManager::event);
	keyEvent();
	mouseEvent();
}

void DeathMenu::update() {
	Mix_HaltMusic();
}

void DeathMenu::render() {
	SDL_SetRenderDrawBlendMode(StageManager::renderer, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(background, 140);
	SDL_SetTextureColorMod(background, 255, 255, 255);
	Graphics::render(background, srcRect, destRect, 0, SDL_FLIP_NONE);
	for (auto button : buttons) {
		if (button->getTag() == "speaker") {
			if (!Audio::mute) button->setTex("imgs/menu/speaker_button.png");
			else button->setTex("imgs/menu/mute_speaker_button.png");
		}
		button->render();
	}

	renderVolumeControl(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 200 + 32);
}
