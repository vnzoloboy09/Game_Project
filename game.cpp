#include "stageManager.h"
#include "game.h"
#include "collision.h"
#include "components.h"
#include "map.h"
#include <sstream>

Manager manager;
Map *map;

SDL_Rect Game::camera = { 0, 0, MAP_WIDTH, MAP_HEIGHT };

auto& player(manager.addEntity());

std::vector<Entity*> enemies;
std::vector<Entity*> tiles;
std::vector<Entity*> players;

Game::Game() {}
Game::~Game() {}

void Game::setPlayerSkin(Color skin) {
    switch (skin) {
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
}

void Game::initPlayer() {
    player.addComponent<TransformComponent>(START_POSITION_X, START_POSITION_X, CAR_WIDTH, CAR_HEIGHT, PLAYER_SPEED);
	player.addComponent<SpriteComponent>();
    setPlayerSkin(playerSkin);
    player.addComponent<ExploderComponent>();
    player.addComponent<KeyboardController>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
    playerHealth = PLAYER_BASE_HEALTH;
}

void Game::initEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        auto& enemy(manager.addEntity());
        enemy.addComponent<TransformComponent>();
        enemy.addComponent<SpriteComponent>("imgs/car/spritesheet.png", 4, 300);
        enemy.addComponent<ExploderComponent>();
        enemy.addComponent<ColliderComponent>("enemy");
        enemy.addGroup(groupEnemies);
    }
}

void Game::setEnemyTarget(Vector2D* target) {
    for (auto &e : enemies) {
        e->addComponent<ChaseComponent>(target);
    }
}

void Game::initMap() {
    map = new Map();
    Map::loadMap("imgs/tilemap80x80.map", 80, 80);
}

void Game::initUI() {
    explosionChunk = Graphics::loadSound("chunks/explosion.wav");

    pauseMenu = new PauseMenu;
    pauseMenu->init();

    deathMenu = new DeathMenu;
    deathMenu->init();

    UIS["healthBar"] = std::move(std::make_unique<UI>("imgs/UI/healthbar.png", 16, 0, 32 * 3, 32, 2));
    UIS["health"] = std::move(std::make_unique<UI>("imgs/UI/health.png", 48, 27, playerHealth, 2, 2));
    UIS["score"] = std::move(std::make_unique<UI>("0.00", StageManager::font, SCORE_POS, 0));
    UIS["healthBar"]->activate();
    UIS["health"]->activate();
    UIS["score"]->activate();
}

void Game::init() {
    initMap();
	tiles = manager.getGroup(groupMap);

    initEnemies();
    enemies = manager.getGroup(groupEnemies);
    
    initPlayer();
	players = manager.getGroup(groupPlayers);

	setEnemyTarget(&(player.getComponent<TransformComponent>().position));
    initUI();
}

void Game::reInit() {
    player.getComponent<TransformComponent>().setPos(START_POSITION_X, START_POSITION_X);
    player.getComponent<TransformComponent>().angle = 0.0f;
    player.getComponent<ColliderComponent>().eneable();
    player.getComponent<TransformComponent>().start();
    playerHealth = PLAYER_BASE_HEALTH;
    UIS["health"]->setDest(48, 27, playerHealth, 2);
    score = 0.0f;
    setPlayerSkin(playerSkin);
    for (auto e : enemies) {
        if ((rand() % 2) % 2) {
            e->getComponent<TransformComponent>().setPos(rand() % MAP_WIDTH, -CAR_HEIGHT);
        }
        else {
            e->getComponent<TransformComponent>().setPos(rand() % MAP_WIDTH, MAP_HEIGHT);
        }
        //e->getComponent<TransformComponent>().setPos(0.0f, 0.0f);
    }
    player.getComponent<KeyboardController>().activate();
     
    deathScenceTime = DEATH_SCENCE_TIME;
    game_over = false;

    pauseMenu->deactivate();
    deathMenu->deactivate();
}

void Game::gameOver() {
    deathMenu->activate();
} 

void Game::keyEvent() {
    switch (StageManager::event.type) {
    case SDL_QUIT:
        StageManager::quit();

    case SDL_KEYDOWN:
        if (StageManager::event.key.keysym.sym == SDLK_ESCAPE) {
            if (pauseMenu->isActive()) pauseMenu->deactivate();
            else pauseMenu->activate();
        }
        if (StageManager::event.key.keysym.sym == SDLK_F11) {
            StageManager::dev_mode = !StageManager::dev_mode;
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
    else if (deathMenu->isActive()) {
        deathMenu->handleEvent();
    }
    else {
        SDL_PollEvent(&StageManager::event);
        keyEvent();
        mouseEvent();
        handleCollision();
        stayInBound();
        if (playerHealth <= 0 && !game_over) {
            game_over = true;
            player.getComponent<KeyboardController>().deactivate();
            makeExplosion(&player);
        }
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
    if (pauseMenu->isActive()) {
        pauseMenu->update();
        return;
    }
    if (deathMenu->isActive()) {
        deathMenu->update();
        return;
    }
    if (game_over) {
        deathScenceTime--;
        if (deathScenceTime == 0) deathMenu->activate();
		if (deathScenceTime % TIME_PER_EXPLOSION == 0) makeExplosion(&player);
    }
    cameraUpdate();
    scoreUpdate();
	enemiesUpdate();
    manager.refresh();
    manager.update();
}
 
void Game::render() {
    SDL_RenderClear(StageManager::renderer);

    for (auto& t : tiles) t->render();
    for (auto& e : enemies) e->render();
    for (auto& p : players) p->render();
    for (auto& ui : UIS) {
        if(ui.second->isActive()) ui.second->render();
    }
    if (pauseMenu->isActive()) pauseMenu->render();
    if (deathMenu->isActive()) deathMenu->render();

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

void Game::makeExplosion(Entity* a) {
    if (a->hasComponent<ExploderComponent>() && !a->getComponent<ExploderComponent>().isExploding()) {
        a->getComponent<ExploderComponent>().explode();
        a->getComponent<ColliderComponent>().disable();
        a->getComponent<TransformComponent>().stop();

        if (a->getComponent<TransformComponent>().position.x >= camera.x &&
            a->getComponent<TransformComponent>().position.x < camera.x + SCREEN_WIDTH - CAR_WIDTH &&
            a->getComponent<TransformComponent>().position.y >= camera.y &&
            a->getComponent<TransformComponent>().position.y < camera.y + SCREEN_HEIGHT - CAR_HEIGHT) {
            Graphics::play(explosionChunk);
        } // if it in camera view then play exlosion sound
    }
}

void Game::handleCollision() {
    for (int i = 0; i < MAX_ENEMIES; i++) { 
        // player collides with enemy
        if (Collision::isCollidingSAT(player.getComponent<ColliderComponent>(),
            enemies[i]->getComponent<ColliderComponent>())) {
            playerHealth -= 10;
            UIS["health"]->setDest(48, 27, playerHealth, 2);
			makeExplosion(enemies[i]);
        }

        // enemy colides with other enemies
        for (int j = i + 1; j < MAX_ENEMIES; j++) {
            if (Collision::isCollidingSAT(
                enemies[i]->getComponent<ColliderComponent>(),
                enemies[j]->getComponent<ColliderComponent>()
            )) {
				makeExplosion(enemies[i]);
                makeExplosion(enemies[j]);
            }
        }
    }
}

void Game::respawnEnemyRandomly(Entity* enemy) {
    int r = rand() % 4;
	if (r == 0) {
		enemy->getComponent<TransformComponent>().setPos(rand() % MAP_WIDTH, -CAR_HEIGHT); // top
	}
	else if (r == 1) {
		enemy->getComponent<TransformComponent>().setPos(MAP_WIDTH, rand() % MAP_HEIGHT); // right
	}
	else if (r == 2) {
		enemy->getComponent<TransformComponent>().setPos(rand() % MAP_WIDTH, MAP_HEIGHT); // bottom
	}
	else {
		enemy->getComponent<TransformComponent>().setPos(-CAR_WIDTH, rand() % MAP_HEIGHT); // left
	} // spawn enemy randomly outside of the map 
}

void Game::enemiesUpdate() {
    for (auto& e : enemies) {
		if (playerHealth <= 0) {
            e->getComponent<TransformComponent>().stop();
        }
        else if (e->getComponent<ExploderComponent>().isExploding()) {
            continue;
		}
		else if(e->getComponent<TransformComponent>().stopped){
			respawnEnemyRandomly(e);
            e->getComponent<ColliderComponent>().eneable();
			e->getComponent<TransformComponent>().start();
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