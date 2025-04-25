#pragma once

#include <SDL_image.h>
#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include "defs.h"
#include "pauseMenu.h"
#include "deathMenu.h"
#include "UI.h"
#include "ECS.h"
#include "vector2D.h"

class ColliderComponent;

class Game : public Stage {
private:
	float score = 0.0f;
	std::unordered_map<std::string, std::unique_ptr<UI>> UIS;
	int playerHealth;
	float timeElapsed = 0.0f;
	float incrementInterval = 2.0f;

	SDL_Texture* rainBg;

	Mix_Chunk* healChunk;
	Mix_Chunk* ghostChunk;
	Mix_Chunk* explosionChunk;
	Mix_Chunk* gameoverChunk;
	Mix_Chunk* driftChunk;
	Mix_Chunk* carEngineChunk;
	Mix_Music* backgroundMusic;

	PauseMenu* pauseMenu;
	DeathMenu* deathMenu;

	int deathScenceTime = DEATH_SCENCE_TIME; 
	bool game_over = false;
	bool drifting = false;
	int rainBg_transparency = 0;
	bool rainning = false;
	bool check_weather = false;
	bool rainning_before = false;

public:
	Game();
	~Game();

	// inits
	static void addTile(int x, int y, int id);
	void initPlayer();
	void initEnemies();
	void initMap();
	void initUI();
	void initPowerUps();
	void initChunks();
	void init() override;
	void reInit() override;

	// events
	void keyEvent() override;
	void mouseEvent() override;
	void handleEvent() override;
	
	// collisions
	void handlePowerUpsCollision();
	void handleEnemiesCollision();
	void handleCollision();
	
	// updates
	void update() override;
	void scoreUpdate();
	void enemiesUpdate();
	void cameraUpdate();
	void powerUpsUpdate();
	void updateHightestScore();
	void rainUpdate();
	
	// render
	void renderRain();
	void render() override;
	
	// extras
	void setEnemyTarget(Vector2D* target);
	void setPlayerSkin(Color skin);
	void stayInBound();
	void gameOver();
	void makeExplosion(Entity* a);
	void respawnEnemyRandomly(Entity* enemy);

	static SDL_Rect camera;
	static Color playerSkin;
};