#include "stageManager.h"
#include "graphics.h"

SDL_Event StageManager::event;

SDL_Renderer* StageManager::renderer = NULL;
TTF_Font* StageManager::font;
tStage StageManager::stage;

StageManager::StageManager() {
	stage = MENU_STAGE;
	stage_is_running = true;
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
	StageManager::font = Graphics::loadFont("fonts/Pixellntv.ttf", 100);
}

void StageManager::init() {
	initSDL();
	srand(time(0));

	game = new Game();
	game->init();

	menu = new Menu(); 
	menu->init();
}

bool StageManager::isRunning() const {
	return running;
}

void StageManager::stopCurrentStage() {
	stage_is_running = false;
}

void StageManager::handleMenuEvent() {
	SDL_PollEvent(&event);
	SDL_GetMouseState(&mouse.x, &mouse.y);
	switch (event.type) {
	case SDL_QUIT:
		stopCurrentStage();
		running = false;

	default:
		break;
	}

	for (auto button : menu->getButtons()) {
		if (button->isHover(mouse.x, mouse.y) &&
			event.type == SDL_MOUSEBUTTONDOWN) {
			if (button->getTag() == "play") {
				stopCurrentStage();
				stage = GAME_STAGE;
			}
		}
	}
}

void StageManager::handleGameEvent() {
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		stopCurrentStage();
		running = false;

	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			stopCurrentStage();
			stage = MENU_STAGE;
		}
	default:
		break;
	}
}

void StageManager::presentGameStage() {
	Uint32 frameStart;
	int frametime;

	while (stage_is_running) {
		frameStart = SDL_GetTicks();

		handleGameEvent();
		game->update();
		game->render();

		// limiting frame time
		frametime = SDL_GetTicks() - frameStart;
		if (frametime < timePerFrame) {
			SDL_Delay(timePerFrame - frametime);
		}
	}
}

void StageManager::presentMenuStage() {
	while (stage_is_running) {
		handleMenuEvent();
		menu->render();
	}
}

void StageManager::presentStage() {
	stage_is_running = true;
	switch (stage) {
	case GAME_STAGE:
		presentGameStage();
		break;
	case MENU_STAGE:
		presentMenuStage();
		break;
	default:
		break;
	}
}

void StageManager::clear() {
	game->clear();
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}