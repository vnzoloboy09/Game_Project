#pragma once

#include <SDL_image.h>
#include <vector>
#include "vector2D.h"

class ColliderComponent;

class Game {
private:
	bool running = true;
	float score = 0;
	float score_flag = 5;

public:
	Game();
	~Game();

	void initSDL();

	void initPlayer();
	void initEnemy();
	void initMap();
	void init();
	bool isRunning() const;
	void gameOver();
	void update(); 
	void render();
	void handleEvent();
	void cameraUpdate();
	void respawnEnemies();
	void clear();

	static void addTile(int x, int y, int id);
	static std::vector<ColliderComponent*> colliders;
	static SDL_Rect camera;
};