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
void Game::initMap() {
    map = new Map();
    Map::loadMap("imgs/tilemap80x80.map", 80, 80);

    // rain map
    rainEntity.addComponent<TransformComponent>(0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT);
    rainEntity.addComponent<SpriteComponent>("imgs/ob/rain.png", 4, 100);
    rainEntity.getComponent<SpriteComponent>().setBlend(true);
    rainBg = Graphics::loadTexture("imgs/menu/pause_background.png"); // same as the pause bg =))

    lightningTexture = Graphics::loadTexture("imgs/ob/lightning.png");
}
void Game::initChunks() {
    healChunk = Audio::loadSound("chunks/heal.wav");
    ghostChunk = Audio::loadSound("chunks/ghost.wav");
    explosionChunk = Audio::loadSound("chunks/explosion.wav");
    gameoverChunk = Audio::loadSound("chunks/game_over.wav");
    driftChunk = Audio::loadSound("chunks/drift.wav");
    carEngineChunk = Audio::loadSound("chunks/car_engine.wav");
    lightningChunk = Audio::loadSound("chunks/lightning.wav");
    backgroundMusic = Audio::loadMusic("chunks/bg_music.mp3");
}
void Game::initUI() {

    pauseMenu = new PauseMenu;
    pauseMenu->init();

    deathMenu = new DeathMenu;
    deathMenu->init();

    UIS["healthBar"] = std::move(std::make_unique<UI>("imgs/UI/healthbar.png", 16, 0, 32 * 3, 32, 2));
    UIS["health"] = std::move(std::make_unique<UI>("imgs/UI/health.png", 48, 27, playerHealth, 2, 2));
    UIS["score"] = std::move(std::make_unique<UI>("0.00", StageManager::font, SCORE_POS, 0));

    for (auto& ui : UIS) {
        ui.second->activate();
    }
}   
void Game::initPowerUps() {
    // init -32 -32 out of the map
    auto &heal_powerup(manager.addEntity());
    heal_powerup.addComponent<TransformComponent>(-32, -32, CAR_WIDTH, CAR_WIDTH);
    heal_powerup.addComponent<SpriteComponent>("imgs/ob/heal.png", 8, 200);
    heal_powerup.addComponent<ColliderComponent>("heal power up");
    heal_powerup.addGroup(groupPowerUps);

    auto &shield_powerup(manager.addEntity());
    shield_powerup.addComponent<TransformComponent>(-32, -32, CAR_WIDTH, CAR_WIDTH);
    shield_powerup.addComponent<SpriteComponent>("imgs/ob/ghost.png", 4, 200);
    shield_powerup.addComponent<ColliderComponent>("ghost power up");
    shield_powerup.addGroup(groupPowerUps);

}
void Game::init() {
    initMap();
	tiles = manager.getGroup(groupMap);

    initEnemies();
    enemies = manager.getGroup(groupEnemies);
    
	initPowerUps();
    powerUps = manager.getGroup(groupPowerUps);

    initPlayer();
	players = manager.getGroup(groupPlayers);

	setEnemyTarget(&(player.getComponent<TransformComponent>().position));
    initUI();
    initChunks();
}
void Game::reinitWeather() {
    check_weather = false;
    rainning = false;
    rainning_before = false;
    rainBg_transparency = 0;

    lightningStriking = false;
    lightning_trasparency = 0;
    lightningInc = false;
}
void Game::reInit() {
    player.getComponent<TransformComponent>().setPos(START_POSITION_X, START_POSITION_X);
    player.getComponent<TransformComponent>().angle = 0.0f;
    player.getComponent<ColliderComponent>().eneable();
    player.getComponent<TransformComponent>().start();
    player.getComponent<KeyboardController>().activate();
    playerHealth = PLAYER_BASE_HEALTH;
    UIS["health"]->setDest(48, 27, playerHealth, 2);
    score = 0.0f;
    setPlayerSkin(playerSkin);
    for (auto e : enemies) {
		e->getComponent<TransformComponent>().setPos(-CAR_WIDTH, -CAR_HEIGHT);
    }
	for (auto p : powerUps) {
		p->getComponent<TransformComponent>().setPos(-32, -32); // move power up out of the map
		p->getComponent<ColliderComponent>().eneable();
	}
    
    reinitWeather();

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
        return;
    }
    if (deathMenu->isActive()) {
        deathMenu->update();
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
    rainUpdate();
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
            int xpos = rand() % MAP_WIDTH + 100;
            int ypos = rand() % MAP_HEIGHT + 100;
            while (xpos == p->getComponent<TransformComponent>().position.x) {
                int xpos = rand() % MAP_WIDTH + 100;
            }
            while (ypos == p->getComponent<TransformComponent>().position.y) {
                int ypos = rand() % MAP_WIDTH + 100;
            } // add 100 so it dont spawn too near the bound
    
            p->getComponent<TransformComponent>().setPos(rand() % MAP_WIDTH, rand() % MAP_HEIGHT);
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
void Game::rainUpdate() {
    if (check_weather) {
        int r = rand() % (rainning ? STOP_RAINNING_CHANCE : START_RAINNING_CHANCE);
        if (r) rainning = false;
        else rainning = true;
        check_weather = false;
    }
    rainEntity.getComponent<TransformComponent>().setPos(camera.x, camera.y);

    if (rainning) {
        if (!lightningStriking) {
            int r = rand() % LIGHTNING_STRIKE_CHANCE;
            if (!r) {
                lightningStriking = true;
                Audio::play(lightningChunk);
            }
            std::cerr << r << '\n';
        }
        else lightningStrike();
    }

    if (rainning && !rainning_before) {
        rainBg_transparency += 1;
        if (rainBg_transparency >= 180) rainning_before = true;
    }
    if (!rainning && rainning_before) {
        rainBg_transparency -= 1;
        if (rainBg_transparency <= 0) rainning_before = false;
    }
    rainEntity.getComponent<SpriteComponent>().setTransparency(rainBg_transparency);
}


// collisions
void Game::handlePowerUpsCollision() {
    for (auto& p : powerUps) {
        if (Collision::isCollidingSAT(player.getComponent<ColliderComponent>(),
            p->getComponent<ColliderComponent>())) {
            p->getComponent<ColliderComponent>().disable();
			p->getComponent<TransformComponent>().setPos(-32, -32); // move power up out of map
            
            if (p->getComponent<ColliderComponent>().tag == "heal power up") {
                playerHealth = (playerHealth + 10 > PLAYER_BASE_HEALTH ? PLAYER_BASE_HEALTH : playerHealth + 10);
                UIS["health"]->setDest(48, 27, playerHealth, 2);
                Audio::play(healChunk);
            }
            else if (p->getComponent<ColliderComponent>().tag == "ghost power up") {
                player.getComponent<TransformComponent>().speed = GHOST_SPEED;
                Audio::play(ghostChunk);
            }
        }
    }
}
void Game::handleEnemiesCollision() {
    for (int i = 0; i < MAX_ENEMIES; i++) {
        // player collides with enemy
        if (Collision::isCollidingSAT(player.getComponent<ColliderComponent>(),
            enemies[i]->getComponent<ColliderComponent>())) { 
            if (!StageManager::dev_mode) {// invisible when dev mode on
                playerHealth -= 10;
                UIS["health"]->setDest(48, 27, playerHealth, 2);
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
void Game::renderRain() {
    Graphics::makeTransparent(rainBg, rainBg_transparency);
    Graphics::draw(rainBg, 0, 0, SCREEN_WIDTH, SCREEN_WIDTH);
    if (rainning) {
        if (lightningStriking) {
            Graphics::makeTransparent(lightningTexture, lightning_trasparency);
            Graphics::draw(lightningTexture, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        }
        if (rainBg_transparency >= RAIN_IN) rainEntity.render();
    }
    else {
        if (rainBg_transparency >= RAIN_OUT) rainEntity.render();
    }
}
void Game::render() {
    SDL_RenderClear(StageManager::renderer);

    for (auto& t : tiles) t->render();
    for (auto& e : enemies) e->render();
    for (auto& po : powerUps) po->render();
    
    player.render();
    renderRain();

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
void Game::lightningStrike() {
	if (lightningInc) {
		lightning_trasparency += 15;
        if (lightning_trasparency >= LIGHTNING_MAX) lightningInc = false;
	}
	else {
		lightning_trasparency -= 10;
        if (lightning_trasparency <= 0) {
            lightning_trasparency = 0;
            lightningInc = true;
            lightningStriking = false;
        }
	}
};