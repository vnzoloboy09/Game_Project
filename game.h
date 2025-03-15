#pragma once

#include <SDL_image.h>
#include <vector>
#include "defs.h"

class ColliderComponent;

class Game {
private:
	float score = 0;
	float score_flag = 5;

public:
	Game();
	~Game();

	void initPlayer();
	void initEnemy();
	void initMap();
	void init();
	void reInit();
	void gameOver();
	void update(); 
	void render();
	void cameraUpdate();
	void respawnEnemies();
	void clear();

	static void addTile(int x, int y, int id);
	static std::vector<ColliderComponent*> colliders;
	static SDL_Rect camera;
	static Color playerSkin;
};