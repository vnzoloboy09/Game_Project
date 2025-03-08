#include "game.h"
#include "collision.h"
#include "components.h"
#include "map.h"

Manager manager;
Map *map;

SDL_Renderer* Game::renderer = NULL;
SDL_Event Game::event;

std::vector<ColliderComponent*> Game::colliders;

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

    map = new Map();
    Map::loadMap("imgs/tilemap80x80.map", MAP_WIDTH / TILE_SIZE, MAP_HEIGHT / TILE_SIZE);

	player.addComponent<TransformComponent>(START_POSITION_X, START_POSITION_Y, CAR_WIDTH, CAR_HEIGHT);
	player.addComponent<SpriteComponent>("imgs/car/yellow_car.png");
	player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);

    enemy.addComponent<TransformComponent>(300.0f, 300.0f, CAR_WIDTH, CAR_HEIGHT);
    enemy.addComponent<SpriteComponent>("imgs/car/Police_1.png");
    enemy.addComponent<ColliderComponent>("enemy");
    enemy.addGroup(groupEnemies);
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
}

void Game::update() {
	manager.refresh();
	manager.update();

    for (auto collider : colliders) {
        Collision::isCollidingSAT(player.getComponent<ColliderComponent>(), *collider);
    }
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game::render() {
    SDL_RenderClear(renderer);

    for (auto& t : tiles) t->render();
    for (auto& p : players) p->render();
    for (auto& e : enemies) e->render();

    SDL_RenderPresent(renderer);
}

void Game::clear() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    IMG_Quit();
    SDL_Quit();
}

void Game::addTile(int x, int y, int id) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, TILE_SIZE, TILE_SIZE, id);
    tile.addGroup(groupMap);
}