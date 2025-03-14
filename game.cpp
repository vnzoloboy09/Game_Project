#include "game.h"
#include "collision.h"
#include "components.h"
#include "map.h"
#include "defs.h"
#include "stageManager.h"

Manager manager;
Map *map;

SDL_Rect Game::camera = { 0, 0, MAP_WIDTH, MAP_HEIGHT };
std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());

Game::Game() {}
Game::~Game() {}

void Game::initPlayer() {
    player.addComponent<TransformComponent>(START_POSITION_X, START_POSITION_Y, CAR_WIDTH, CAR_HEIGHT, PLAYER_SPEED);
    player.addComponent<SpriteComponent>("imgs/car/yellow_car.png");
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
} 

void Game::initEnemy() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        auto& enemy(manager.addEntity());
        enemy.addComponent<TransformComponent>();
        enemy.addComponent<SpriteComponent>("imgs/car/spritesheet.png", 4, 300);
        enemy.addComponent<ChaseComponent>(&(player.getComponent<TransformComponent>().position));
        enemy.addComponent<ColliderComponent>("enemy");
        enemy.addGroup(groupEnemies);
    }
}

void Game::initMap() {
    map = new Map();
    Map::loadMap("imgs/tilemap80x80.map", 80, 80);
}

void Game::init() {
    //initSDL();
    //initMap();
    initPlayer();
    initEnemy();
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game::gameOver() {
    std::cerr << "game over!!";
}

void Game::handleEvent() {
}

void Game::update() {
    cameraUpdate(); 
	manager.refresh();
	manager.update();
    respawnEnemies();
}

void Game::render() {
    SDL_RenderClear(StageManager::renderer);

    for (auto& t : tiles) t->render();
    for (auto& p : players) p->render();
    for (auto& e : enemies) e->render();

    SDL_RenderPresent(StageManager::renderer);
}

void Game::clear() {
    
}

void Game::cameraUpdate() {
    camera.x = std::round(player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH / 2.0f);
    camera.y = std::round(player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT / 2.0f);

    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x + SCREEN_WIDTH > MAP_WIDTH) camera.x = MAP_WIDTH - SCREEN_WIDTH;
    if (camera.y + SCREEN_HEIGHT > MAP_HEIGHT) camera.y = MAP_HEIGHT - SCREEN_HEIGHT;
}

void Game::respawnEnemies() {
    // spawn at random position outside of the map if got hit by another enemy
    for (int i = 0; i < MAX_ENEMIES; i++) {
        for (int j = i + 1; j < MAX_ENEMIES; j++) {
            if (Collision::isCollidingSAT(
                enemies[i]->getComponent<ColliderComponent>(),
                enemies[j]->getComponent<ColliderComponent>()
            )) {
                if ((rand() % 2) % 2) {
                    enemies[i]->getComponent<TransformComponent>().setPos(rand() % MAP_WIDTH, -CAR_HEIGHT);
                    enemies[j]->getComponent<TransformComponent>().setPos(-CAR_WIDTH, rand() % MAP_HEIGHT);
                }
                else {
                    enemies[i]->getComponent<TransformComponent>().setPos(rand() % MAP_WIDTH, MAP_HEIGHT);
                    enemies[j]->getComponent<TransformComponent>().setPos(MAP_WIDTH, rand() % MAP_HEIGHT);
                }
            }
        }
    }
}

void Game::addTile(int x, int y, int id) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, id);
    tile.addGroup(groupMap);
}