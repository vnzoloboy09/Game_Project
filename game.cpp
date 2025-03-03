#include "game.h"
#include "player.h"
#include "enemy.h"
#include "collision.h"
#include "background.h"
#include <vector>

SDL_Renderer* Game::renderer = NULL;
SDL_Event Game::event;

Player* player = NULL;
std::vector<Enemy*> enemies;
Enemy* enemy = NULL;
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
    enemy = new Enemy(75, 50);
    enemy->loadSprite("imgs/Car/Police_1.png",
        "imgs/Car/Police_2.png",
        "imgs/Car/Police_3.png",
        "imgs/Car/Police_4.png", NULL);

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
	if (Collision::isColliding(player, enemy)) {
		color = RED;
		//gameOver();
	}
	else {
		color = BLUE;
	}
}

void Game::update() {
    background_1->update(BACKGROUND_SCROLLING_SPEED);
    background_2->update(BACKGROUND_SCROLLING_SPEED);

    player->update();
    enemy->update();
}

void Game::render() {
    SDL_RenderClear(renderer);

    background_1->render();
    background_2->render();

    Graphics::setColor(GREEN);
    enemy->render();
   
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
