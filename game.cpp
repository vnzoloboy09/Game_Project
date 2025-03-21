#include "stageManager.h"
#include "game.h"
#include "collision.h"
#include "components.h"
#include "map.h"
#include <sstream>

Manager manager;
Map *map;

SDL_Rect Game::camera = { 0, 0, MAP_WIDTH, MAP_HEIGHT };
std::vector<ColliderComponent*> Game::colliders;

auto& player(manager.addEntity());

Game::Game() {}
Game::~Game() {}

void Game::initPlayer() {
    player.addComponent<TransformComponent>(START_POSITION_X, START_POSITION_X, CAR_WIDTH, CAR_HEIGHT, PLAYER_SPEED);
    switch (playerSkin) {
    case YELLOW:
        player.addComponent<SpriteComponent>("imgs/car/yellow_car.png");
        break;
    case RED:
        player.addComponent<SpriteComponent>("imgs/car/red_car.png");
        break;
    case BLUE:
        player.addComponent<SpriteComponent>("imgs/car/blue_car.png");
        break;
    default:
        break;
    }
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
    playerHealth = PLAYER_BASE_HEALTH;
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

void Game::initUI() {
    pauseMenu = new PauseMenu;
    pauseMenu->init();

    UIS["healthBar"] = std::move(std::make_unique<UI>("imgs/UI/healthbar.png", 16, 0, 32 * 3, 32, 2));
    UIS["health"] = std::move(std::make_unique<UI>("imgs/UI/health.png", 48, 27, playerHealth, 2, 2));
    UIS["score"] = std::move(std::make_unique<UI>("0.00", StageManager::font, SCORE_POS, 0));
    UIS["healthBar"]->activate();
    UIS["health"]->activate();
    UIS["score"]->activate();
}

void Game::init() {
    initMap();
    initPlayer();
    initEnemy();
    initUI();
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game::reInit() {
    player.getComponent<TransformComponent>().setPos(START_POSITION_X, START_POSITION_X);
    player.getComponent<TransformComponent>().angle = 0.0f;
    playerHealth = PLAYER_BASE_HEALTH;
    UIS["health"]->setDest(48, 27, playerHealth, 2);
    score = 0.0f;
    switch (playerSkin) {
    case YELLOW:
        player.getComponent<SpriteComponent>().setTex("imgs/car/yellow_car.png");
        break;
    case RED:
        player.getComponent<SpriteComponent>().setTex("imgs/car/red_car.png");
        break;
    case BLUE:
        player.getComponent<SpriteComponent>().setTex("imgs/car/blue_car.png");
        break;
    default:
        break;
    }
    for (auto e : enemies) {
        e->getComponent<TransformComponent>().setPos(0.0f, 0.0f);
    }

    pauseMenu->deactivate();
}

void Game::gameOver() {
    StageManager::current_stage->deactivate();
    StageManager::changeStage("Menu");
    std::cerr << "game over!!";
}

void Game::keyEvent() {
    switch (StageManager::event.type) {
    case SDL_QUIT:
        StageManager::current_stage->deactivate();
        StageManager::running = false;

    case SDL_KEYDOWN:
        if (StageManager::event.key.keysym.sym == SDLK_ESCAPE) {
            if (pauseMenu->isActive()) pauseMenu->deactivate();
            else pauseMenu->activate();
        }
    default:
        break;
    }
}

void Game::mouseEvent() {}

void Game::handleEvent() {
    if (pauseMenu->isActive()) {
        pauseMenu->handleEvent();
    }
    else {
        SDL_PollEvent(&StageManager::event);
        keyEvent();
        mouseEvent();
        handleCollision();
        stayInBound();
        if (playerHealth <= 0) gameOver();
    }
}

void Game::scoreUpdate() {
    timeElapsed += 0.1f;
    if (timeElapsed > incrementInterval) {
        score += 0.25f;  
        timeElapsed = 0.0f;
    }
    std::ostringstream os;
    os << static_cast<int>(score);

    UIS["score"]->setTexture(os.str().c_str(), StageManager::font);
    UIS["score"]->setDest(SCORE_POS - 30*(os.str().size()), 0); // prevent score render out of screen
} 

void Game::update() {  
    if (!pauseMenu->isActive()) {
        cameraUpdate();
        scoreUpdate();
        manager.refresh();
        manager.update();
    }
    else pauseMenu->update();
}

void Game::render() {
    SDL_RenderClear(StageManager::renderer);

    for (auto& t : tiles) t->render();
    for (auto& p : players) p->render();
    for (auto& e : enemies) e->render();
    for (auto& ui : UIS) {
        if(ui.second->isActive()) ui.second->render();
    }
    if (pauseMenu->isActive()) pauseMenu->render();

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

void Game::handleCollision() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        // decrease player health if got hit
        if (Collision::isCollidingSAT(player.getComponent<ColliderComponent>(),
            enemies[i]->getComponent<ColliderComponent>())) {
            playerHealth -= 10;
            UIS["health"]->setDest(48, 27, playerHealth, 2);
            if ((rand() % 2) % 2) {
                enemies[i]->getComponent<TransformComponent>().setPos(rand() % MAP_WIDTH, -CAR_HEIGHT);
            }
            else {
                enemies[i]->getComponent<TransformComponent>().setPos(rand() % MAP_WIDTH, MAP_HEIGHT);
            }
        }

        // spawn enemy at random position outside of the map if got hit by another enemy
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

void Game::stayInBound() {
    player.getComponent<TransformComponent>().position;
    for (int i = 0; i < 4; i++) {
        if (player.getComponent<TransformComponent>().corners[i].x >= MAP_WIDTH) {
            player.getComponent<TransformComponent>().position.x -= BOUND_BLOCK;
        }
        if (player.getComponent<TransformComponent>().corners[i].x <= 0) {
            player.getComponent<TransformComponent>().position.x += BOUND_BLOCK;
        }
        if (player.getComponent<TransformComponent>().corners[i].y >= MAP_HEIGHT) {
            player.getComponent<TransformComponent>().position.y -= BOUND_BLOCK;
        }
        if (player.getComponent<TransformComponent>().corners[i].y <= 0) {
            player.getComponent<TransformComponent>().position.y += BOUND_BLOCK;
        }
    }
}

void Game::addTile(int x, int y, int id) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, id);
    tile.addGroup(groupMap);
}