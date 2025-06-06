#include "stageManager.h"
#include "game.h"
#include "collision.h"
#include "audio.h"
#include "components.h"
#include "map.h"
#include <sstream>
#include <fstream>

Manager manager;
Map *map;

SDL_Rect Game::camera = { 0, 0, MAP_WIDTH, MAP_HEIGHT };

auto& player(manager.addEntity());
auto& rainEntity(manager.addEntity());
  
std::vector<Entity*> enemies; 
std::vector<Entity*> tiles;
std::vector<Entity*> players;
std::vector<Entity*> powerUps;

Game::Game() = default;
Game::~Game() {
	delete map;
	delete pauseMenu;
	delete deathMenu;
	Mix_FreeChunk(healChunk);
	Mix_FreeChunk(ghostChunk);
	Mix_FreeChunk(explosionChunk);
	Mix_FreeChunk(gameoverChunk);
    Mix_FreeChunk(driftChunk);
	Mix_FreeMusic(backgroundMusic);
}
  

// inits  
void Game::addTile(int x, int y, int id) {
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(x, y, id);
    tile.addGroup(groupMap);
}
  
void Game::initPlayer() {
    player.addComponent<TransformComponent>(START_POSITION_X, START_POSITION_X, CAR_WIDTH, CAR_HEIGHT, PLAYER_SPEED);
	player.addComponent<SpriteComponent>();
    setPlayerSkin(playerSkin);
    player.addComponent<ExploderComponent>(); 
    player.addComponent<KeyboardController>();
    player.addComponent<ShieldComponent>();
    player.addComponent<ColliderComponent>("player");
    player.addGroup(groupPlayers);
    playerHealth = PLAYER_BASE_HEALTH; 
}   

void Game::initEnemies() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        auto& enemy(manager.addEntity());
        enemy.addComponent<TransformComponent>(-32, -32, CAR_WIDTH, CAR_HEIGHT, ENEMY_SPEED + i / 3.0f); // -32: outside the map
        enemy.addComponent<SpriteComponent>("imgs/car/spritesheet.png", 4, ENEMY_SPRITE_SPEED); // 4 frames
        enemy.addComponent<ExploderComponent>();
        enemy.addComponent<ColliderComponent>("enemy");
        enemy.addGroup(groupEnemies);
    }
}

void Game::initMap() { 
    map = new Map();
    Map::loadMap("imgs/tilemap80x80.map", TILE_NUM, TILE_NUM);

    // rain map
    rainEntity.addComponent<TransformComponent>(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    rainEntity.addComponent<RainComponent>(&camera.x, &camera.y, &check_weather, 
        &(player.getComponent<TransformComponent>().position), &player.getComponent<TransformComponent>().angle);

}
 
void Game::initChunks() {
    healChunk = Audio::loadSound("chunks/heal.wav");
    ghostChunk = Audio::loadSound("chunks/ghost.wav");
    explosionChunk = Audio::loadSound("chunks/explosion.wav");
    gameoverChunk = Audio::loadSound("chunks/game_over.wav");
    driftChunk = Audio::loadSound("chunks/drift.wav");
    backgroundMusic = Audio::loadMusic("chunks/bg_music.mp3");
}

void Game::initUI() {

    pauseMenu = new PauseMenu;
    pauseMenu->init();

    deathMenu = new DeathMenu;
    deathMenu->init();

    UIS["healthBar"] = std::move(std::make_unique<UI>("imgs/UI/healthbar.png", 
        HEALTH_BAR_XPOS, HEALTH_BAR_YPOS, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT, HEALTH_BAR_SCALE));
    UIS["health"] = std::move(std::make_unique<UI>("imgs/UI/health.png", 
        HEALTH_XPOS, HEALTH_YPOS, playerHealth, HEALTH_BAR_THICKNESS, HEALTH_BAR_SCALE));
    UIS["score"] = std::move(std::make_unique<UI>("0.00", StageManager::font, SCORE_POS, 0));

    for (auto& ui : UIS) {
        ui.second->activate();
    }
}   

void Game::initPowerUps() {
    // init -32 -32 out of the map
    auto &heal_powerup(manager.addEntity());
    heal_powerup.addComponent<TransformComponent>(-CAR_WIDTH, -CAR_WIDTH, CAR_WIDTH, CAR_WIDTH);
    heal_powerup.addComponent<SpriteComponent>("imgs/ob/heal.png", 8, 200);
    heal_powerup.addComponent<ColliderComponent>("heal power up");
    heal_powerup.addGroup(groupPowerUps);

    auto& ghost_powerup(manager.addEntity());
    ghost_powerup.addComponent<TransformComponent>(-CAR_WIDTH, -CAR_WIDTH, CAR_WIDTH, CAR_WIDTH);
    ghost_powerup.addComponent<SpriteComponent>("imgs/ob/ghost.png", 4, 200);
    ghost_powerup.addComponent<ColliderComponent>("ghost power up");
    ghost_powerup.addGroup(groupPowerUps);
     
    auto &shield_powerup(manager.addEntity());
    shield_powerup.addComponent<TransformComponent>(-CAR_WIDTH, -CAR_WIDTH, CAR_WIDTH, CAR_WIDTH);
    shield_powerup.addComponent<SpriteComponent>("imgs/ob/shield_icon.png", 7, 200);
    shield_powerup.addComponent<ColliderComponent>("shield power up");
    shield_powerup.addGroup(groupPowerUps);
}

void Game::init() {
    initEnemies();
    enemies = manager.getGroup(groupEnemies);
    
	initPowerUps();
    powerUps = manager.getGroup(groupPowerUps);

    initPlayer();
	players = manager.getGroup(groupPlayers);
    
    initMap();
	tiles = manager.getGroup(groupMap);

	setEnemyTarget(&(player.getComponent<TransformComponent>().position));
    initUI();
    initChunks();
}

void Game::reInit() {
    player.getComponent<TransformComponent>().setPos(START_POSITION_X, START_POSITION_X);
    player.getComponent<TransformComponent>().angle = 0.0f;
    player.getComponent<ColliderComponent>().eneable();
    player.getComponent<TransformComponent>().start();
    player.getComponent<KeyboardController>().activate();
    player.getComponent<ShieldComponent>().shieldOff();
    playerHealth = PLAYER_BASE_HEALTH;
    UIS["health"]->setDest(HEALTH_XPOS, HEALTH_YPOS, playerHealth, HEALTH_BAR_SCALE);
    score = 0.0f;
    setPlayerSkin(playerSkin);
    for (auto e : enemies) {
		e->getComponent<TransformComponent>().setPos(-CAR_WIDTH, -CAR_HEIGHT);
    }
	for (auto p : powerUps) {
		p->getComponent<TransformComponent>().setPos(-CAR_WIDTH, -CAR_WIDTH); // move power up out of the map
		p->getComponent<ColliderComponent>().eneable();
	}
    
    rainEntity.getComponent<RainComponent>().reinit();
     
    deathScenceTime = DEATH_SCENCE_TIME;
    game_over = false;

    pauseMenu->deactivate();
    deathMenu->deactivate();

    Audio::play(backgroundMusic);
}


// events
void Game::mouseEvent() {} 
 
void Game::keyEvent() {
    switch (StageManager::event.type) {
    case SDL_QUIT:
        StageManager::quit();

    case SDL_KEYDOWN:
        if (!drifting && !game_over) {
            if (StageManager::event.key.keysym.sym == SDLK_d ||
                StageManager::event.key.keysym.sym == SDLK_a) {
                drifting = true;
                Audio::play(driftChunk);
            }
        }
        if (StageManager::event.key.keysym.sym == SDLK_ESCAPE && !game_over) {
            if (pauseMenu->isActive()) pauseMenu->deactivate();
            else pauseMenu->activate();
        }
        if (StageManager::event.key.keysym.sym == SDLK_F11) {
            StageManager::dev_mode = !StageManager::dev_mode;
        }
        break;
    case SDL_KEYUP:
        if (StageManager::event.key.keysym.sym == SDLK_d ||
            StageManager::event.key.keysym.sym == SDLK_a) {
            drifting = false;
        }
        break;
    default:
        break;
    }
}

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

void Game::gameOver() {
    Audio::play(gameoverChunk);
    if(score > StageManager::hightest_score) updateHightestScore();
    deathMenu->activate();
} 


// updates 
void Game::update() {  
    if (pauseMenu->isActive()) {
        pauseMenu->update();
        keepRaining();
        return;
    }
    if (deathMenu->isActive()) {
        deathMenu->update();
        keepRaining();
        return;
    }
    if (game_over) {
        // make the death scene (play 3 exlosion)
        deathScenceTime--;
        if (deathScenceTime == 0) gameOver();
		else if (deathScenceTime % TIME_PER_EXPLOSION == 0) makeExplosion(&player);
    }
    else scoreUpdate();

    cameraUpdate();
	enemiesUpdate();
    powerUpsUpdate();
    manager.refresh();
    manager.update();
}

void Game::scoreUpdate() {
    timeElapsed += 0.1f; // prevent score update too fast
    if (timeElapsed > incrementInterval) {
        score += 0.25f;  
        timeElapsed = 0.0f;
        if (static_cast<int>(score * 100) % 100 == 0 && // if score are int (x.00)
            static_cast<int>(score) % WEATHER_CHECK == 0) {
            check_weather = true; // every WEATHER_CHECK scores make a weather check
        }
    }
    std::ostringstream os;
    os << static_cast<int>(score);

    UIS["score"]->setTexture(os.str().c_str(), StageManager::font);
    UIS["score"]->setDest(SCORE_POS - 30*(os.str().size()), 0); // prevent score render out of screen
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

void Game::cameraUpdate() {
    // camera follow player
    camera.x = std::round(player.getComponent<TransformComponent>().position.x - SCREEN_WIDTH / 2.0f);
    camera.y = std::round(player.getComponent<TransformComponent>().position.y - SCREEN_HEIGHT / 2.0f);

    // prevent camera out of map
    if (camera.x < 0) camera.x = 0;
    if (camera.y < 0) camera.y = 0;
    if (camera.x + SCREEN_WIDTH > MAP_WIDTH) camera.x = MAP_WIDTH - SCREEN_WIDTH;
    if (camera.y + SCREEN_HEIGHT > MAP_HEIGHT) camera.y = MAP_HEIGHT - SCREEN_HEIGHT;
}

void Game::powerUpsUpdate() {
    if (player.getComponent<TransformComponent>().speed > PLAYER_SPEED) {
		player.getComponent<TransformComponent>().speed -= 0.1;
    } // slowly decrease ghost effect

    for (auto& p : powerUps) {
        if (p->getComponent<TransformComponent>().position.x < 0) {
            // make sure power ups dont spawn too near the border
            int xpos = POWER_SPAWN_BORDER + rand() % (MAP_WIDTH - 2 * POWER_SPAWN_BORDER);
            int ypos = POWER_SPAWN_BORDER + rand() % (MAP_HEIGHT - 2 * POWER_SPAWN_BORDER);
            
            //std::cerr << xpos << " " << ypos << '\n';

            p->getComponent<TransformComponent>().setPos(xpos, ypos); 
	        p->getComponent<ColliderComponent>().eneable();
        }
    }
}
   
void Game::updateHightestScore() {
	std::ofstream file("hightest_score.dat");
	if (file.is_open()) {
		file << static_cast<int>(score);
		file.close();
	}
	else {
		std::cerr << "Unable to open score file.\n";
	}
}


// collisions 
void Game::handlePowerUpsCollision() {
    for (auto& p : powerUps) {
        if (Collision::isCollidingSAT(player.getComponent<ColliderComponent>(),
            p->getComponent<ColliderComponent>())) {
            p->getComponent<ColliderComponent>().disable();
			p->getComponent<TransformComponent>().setPos(-32, -32); // move power up out of map
            
            if (p->getComponent<ColliderComponent>().tag == "heal power up") {
                playerHealth = (playerHealth + HIT_DAMAGE > PLAYER_BASE_HEALTH ? PLAYER_BASE_HEALTH : playerHealth + 10);
                UIS["health"]->setDest(HEALTH_XPOS, HEALTH_YPOS, playerHealth, HEALTH_BAR_SCALE);
                Audio::play(healChunk);
            }
            else if (p->getComponent<ColliderComponent>().tag == "ghost power up") {
                player.getComponent<TransformComponent>().speed = GHOST_SPEED;
                Audio::play(ghostChunk);
            }
            else if (p->getComponent<ColliderComponent>().tag == "shield power up") {
                player.getComponent<ShieldComponent>().shieldOn();
            }
        }
    }
}

void Game::handleEnemiesCollision() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        // player collides with enemy
        if (Collision::isCollidingSAT(player.getComponent<ColliderComponent>(),
            enemies[i]->getComponent<ColliderComponent>())) { 
            // invisible when dev mode on or has shield
            if (!StageManager::dev_mode && !player.getComponent<ShieldComponent>().hasShiled()) {
                playerHealth -= 10;
                UIS["health"]->setDest(HEALTH_XPOS, HEALTH_YPOS, playerHealth, HEALTH_BAR_SCALE);
            }
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

void Game::handleCollision() {
    handlePowerUpsCollision();
    handleEnemiesCollision();
}


// render
void Game::render() {
    SDL_RenderClear(StageManager::renderer);

    for (auto& t : tiles) t->render();
    for (auto& e : enemies) e->render();
    for (auto& po : powerUps) po->render();
    
    player.render();
    rainEntity.render();

    for (auto& ui : UIS) {
        if(ui.second->isActive()) ui.second->render();
    }

    if (pauseMenu->isActive()) pauseMenu->render();
    if (deathMenu->isActive()) deathMenu->render();

    SDL_RenderPresent(StageManager::renderer);
}


// extras
void Game::makeExplosion(Entity* a) {
    if (a->hasComponent<ExploderComponent>() && !a->getComponent<ExploderComponent>().isExploding()) {
        a->getComponent<ExploderComponent>().explode();
        a->getComponent<ColliderComponent>().disable();
        a->getComponent<TransformComponent>().stop();

        // if it in camera view then play exlosion sound
        if (a->getComponent<TransformComponent>().position.x >= camera.x &&
            a->getComponent<TransformComponent>().position.x < camera.x + SCREEN_WIDTH - CAR_WIDTH &&
            a->getComponent<TransformComponent>().position.y >= camera.y &&
            a->getComponent<TransformComponent>().position.y < camera.y + SCREEN_HEIGHT - CAR_HEIGHT) {
            Audio::play(explosionChunk);
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

void Game::stayInBound() {
    // prevent player out of map
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

void Game::setEnemyTarget(Vector2D* target) {
    for (auto &e : enemies) {
        e->addComponent<ChaseComponent>(target);
    }
}

void Game::keepRaining() {
    rainEntity.update();
    rainEntity.render();
}