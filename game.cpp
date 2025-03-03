#include "game.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"
#include "background.h"

SDL_Renderer* Game::renderer = NULL;
SDL_Event Game::event;

Player* player = NULL;
std::vector<Enemy*> enemies;
Color color = BLUE;


Background* background_1 = NULL;
Background* background_2 = NULL;


Game::Game() {}
Game::~Game() {}

void Game::initSDL() {
    if (!SDL_Init(SDL_INIT_EVERYTHING)) {
        SDL_Quit();
    }
    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, false);

    if (window == NULL) SDL_Quit();

    if (!(IMG_Init(IMG_INIT_PNG))) SDL_Quit();

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED
        | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) SDL_Quit();
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void Game::init() {
    initSDL();
    player = new Player(START_POSITION_X, START_POSITION_Y, "imgs/car/Yellow_car.png");
	srand(time(0));
	for (int i = 0; i < MAX_ENEMIES; i++) {
        enemies.push_back(new Enemy(LANES[rand() % LANES.size()], 
            -2 * CAR_LENGTH * i, rand() % 4));
	}

    background_1 = new Background("imgs/background.png", 0);
    background_2 = new Background("imgs/background.png", -BACKGROUND_HEIGHT);
}

bool Game::isRunning() {
    return running;
}

void Game::gameOver() {
    std::cerr << "game over!!";
    running = false;
}

void Game::handleEvent() {
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        running = false;
    default:
        break;
    }

    player->control();

    // check collision
	for (auto enemy : enemies) {
		if (Collision::isColliding(player, enemy)) {
			gameOver();
		}
	}

    if(score_flag <= 0) {
		for (auto enemy : enemies) {
			enemy->addSpeed(1);
		}
		score_flag = 5;
	}
}

void Game::update() {
    score += 0.01;
	score_flag -= 0.01;
    background_1->update(BACKGROUND_SCROLLING_SPEED);
    background_2->update(BACKGROUND_SCROLLING_SPEED);

	for (auto enemy : enemies) {
		enemy->update();
	}

    player->update();
}

void Game::render() {
	std::cerr << static_cast<int>(score) << '\n';
    SDL_RenderClear(renderer);

    background_1->render();
    background_2->render();

    Graphics::setColor(GREEN);
	for (auto enemy : enemies) {
		enemy->render();
	}
   
    Graphics::setColor(color);
    player->render();

    SDL_RenderPresent(renderer);
}

void Game::clear() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    IMG_Quit();
    SDL_Quit();
}
