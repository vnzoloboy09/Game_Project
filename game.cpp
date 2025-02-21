#include "game.h"
#include "player.h"

SDL_Renderer* Game::renderer = NULL;
SDL_Event Game::event;

//SDL_Texture *background = Graphics::loadTexture("imgs/Background.png");

Car* player;

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
    player = new Player(START_POISITION_X, START_POISITION_Y, "imgs/car/Yellow_car.png");
}

bool Game::isRunning() {
    return running;
}

void Game::handleEvent() {
    SDL_PollEvent(&event);
    switch (event.type) {
    case SDL_QUIT:
        running = false;
    default:
        break;
    }
}

void Game::update() {
    player->update();
}

void Game::render() {
    SDL_RenderClear(renderer);

    //SDL_RenderCopy(renderer, background, NULL, NULL);
    player->render();

    SDL_RenderPresent(renderer);
}

void Game::clear() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    IMG_Quit();
    SDL_Quit();
}