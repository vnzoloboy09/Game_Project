#include "stageManager.h"
#include "defs.h"
#include "graphics.h"

unsigned int StageManager::stage = 1;
SDL_Event StageManager::event;

SDL_Renderer* StageManager::renderer = NULL;
TTF_Font* StageManager::font;

SDL_Texture* menu_text;
SDL_Texture* press;

StageManager::StageManager() {
	game_stage = false;
	menu_stage = true;
	current_stage = &menu_stage;
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
	menu_text = Graphics::loadText("this is a MENU", font, { 255, 0, 0 });
	press = Graphics::loadText("press 9 to play", font, { 255, 0, 0 });
}

bool StageManager::isRunning() const {
	return running;
}

void StageManager::stopCurrentStage() {
	*current_stage = false;
}

void StageManager::changeStageTo(bool &stage) {
	stopCurrentStage();
	current_stage = &stage;
	*current_stage = true;
}

void StageManager::handleEvent() {
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		stopCurrentStage();
		running = false;

	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_0) {
			changeStageTo(menu_stage);
		}
		if (event.key.keysym.sym == SDLK_9) {
			changeStageTo(game_stage);
		}
	default:
		break;
	}
}

void StageManager::presentGameStage() {
	Uint32 frameStart;
	int frametime;

	while (game_stage) {
		frameStart = SDL_GetTicks();

		handleEvent();
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
	while (menu_stage) {
		handleEvent();
		Graphics::setColor(WHITE);
		SDL_RenderClear(StageManager::renderer);

		Graphics::draw(menu_text, SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 - 200, 400, 100);
		Graphics::draw(press, SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, 300, 100);

		SDL_RenderPresent(StageManager::renderer);
	}
}

void StageManager::presentStage() {
	switch (stage) {
	case 1:
		presentGameStage();
	case 2:
		presentMenuStage();
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