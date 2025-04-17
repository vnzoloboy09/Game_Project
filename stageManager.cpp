#include "stageManager.h"
#include "graphics.h"
#include <iostream>
#include "chooseMenu.h"
#include "game.h"

SDL_Event StageManager::event;

SDL_Renderer* StageManager::renderer = NULL;
TTF_Font* StageManager::font; 
bool StageManager::running;
bool StageManager::dev_mode;
bool StageManager::mute = false;
Color Game::playerSkin;
std::unordered_map<std::string, std::unique_ptr<Stage>> StageManager::stages;
Stage* StageManager::current_stage = NULL;

StageManager::StageManager() {
	running = true;
}

StageManager::~StageManager() = default;

void StageManager::initSDL() {
	// init Window
	if (!SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cerr << SDL_GetError() << '\n';
		SDL_Quit();
	}
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	if (window == NULL) SDL_Quit();

	// init IMG
	if (!(IMG_Init(IMG_INIT_PNG))) {
		std::cerr << IMG_GetError() << '\n';
		SDL_Quit();
	}

	// init Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED
		| SDL_RENDERER_PRESENTVSYNC);

	if (renderer == NULL) SDL_Quit();
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	// init ttf
	if (TTF_Init() == -1) {
		std::cerr << TTF_GetError() << '\n';
		SDL_Quit();
	}
	StageManager::font = Graphics::loadFont("fonts/Pixellntv.ttf", 50);

	// init mixer 
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
		std::cerr << Mix_GetError() << '\n';
	}
}

void StageManager::quit() {
	current_stage->deactivate();
	running = false;
}

void StageManager::addStage(const std::string& name, std::unique_ptr<Stage> stage) {
	stages[name] = std::move(stage);
}

void StageManager::changeStage(const std::string& name) {
	current_stage->deactivate();
	if (stages.find(name) != stages.end()) {
		if (current_stage) {
			current_stage->deactivate();
		}
		current_stage = stages[name].get();
		current_stage->activate();
	}
	else {
		std::cerr << "Stage '" << name << "' not found!\n";
	}
}

void StageManager::init() {
	initSDL();
    srand(static_cast<unsigned int>(time(0)));

	addStage("Menu", std::make_unique<Menu>());
	addStage("ChooseMenu", std::make_unique<ChooseMenu>());
	addStage("Game", std::make_unique<Game>());
	current_stage = stages["Menu"].get();

	for (auto& s : stages) {
		s.second->init();
	}
	current_stage->activate();
}

bool StageManager::isRunning() const {
	return running;
}

void StageManager::presentStage() {
	for (auto& s : stages) {
		if (s.second->isActive()) {
			s.second->reInit();

			Uint32 frameStart;
			int frametime;
			while (s.second->isActive()) {
				frameStart = SDL_GetTicks();

				s.second->handleEvent();
				s.second->update();
				s.second->render();

				frametime = SDL_GetTicks() - frameStart;
				if (frametime < frameDelay) {
					SDL_Delay(frameDelay - frametime);
				}
				//std::cerr << frametime << ' ' << frameDelay << '\n';
			}
		}
	}
}

void StageManager::clear() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}