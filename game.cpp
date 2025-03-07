#include "game.h"
#include "collision.h"
#include "background.h"
#include "components.h"


SDL_Renderer* Game::renderer = NULL;
SDL_Event Game::event;
Background* background = NULL;
Color color = BLUE;

Manager manager;
auto& enemy(manager.addEntity());
auto& player(manager.addEntity());

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

	player.addComponent<TransformComponent>();
	player.addComponent<SpriteComponent>("imgs/car/yellow_car.png");
	player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>();

    enemy.addComponent<TransformComponent>(300.0f, 300.0f);
    enemy.addComponent<SpriteComponent>("imgs/car/Police_1.png");
    enemy.addComponent<ColliderComponent>();

    background = new Background("imgs/background.png", 640, 360);
}

bool Game::isRunning() const {
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
    if (Collision::isCollidingSAT(player.getComponent<ColliderComponent>().collider,
        enemy.getComponent<ColliderComponent>().collider)) {
        color = RED;
    }
    else color = BLUE;
}

void Game::update() {
	manager.refresh();
	manager.update();
}

void Game::render() {
    SDL_RenderClear(renderer);

    background->render();

    Graphics::setColor(color);
	manager.render();

    SDL_RenderPresent(renderer);
}

void Game::clear() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    IMG_Quit();
    SDL_Quit();
}
