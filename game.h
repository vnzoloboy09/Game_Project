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
	Mix_Chunk* explosionChunk;
	PauseMenu* pauseMenu;
	DeathMenu* deathMenu;
	int deathScenceTime = DEATH_SCENCE_TIME; 
	bool game_over = false;

public:
	Game();
	~Game();

	void initPlayer();
	void initEnemies();
	void setEnemyTarget(Vector2D* target);
	void setPlayerSkin(Color skin);
	void initMap();
	void initUI();
	void init() override;
	void reInit() override;
	void gameOver();

	void keyEvent() override;
	void mouseEvent() override;
	void handleEvent() override;
	void handleCollision();
	void update() override;
	void render() override;

	void stayInBound();
	void scoreUpdate();
	void cameraUpdate();
	void enemiesUpdate();
	void clear();

	void makeExplosion(Entity* a);
	void respawnEnemyRandomly(Entity* enemy);

	static void addTile(int x, int y, int id);
	static SDL_Rect camera;
	static Color playerSkin;
};