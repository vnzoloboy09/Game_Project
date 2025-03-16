#pragma once

#include <SDL_image.h>
#include <vector>
#include "defs.h"
#include "stage.h"

class ColliderComponent;

class Game : public Stage {
private:
	float score = 0;
	float score_flag = 5;

public:
	Game();
	~Game();

	void initPlayer();
	void initEnemy();
	void initMap();
	void init() override;
	void reInit() override;
	void gameOver();
	void handleEvent() override;
	void update() override; 
	void render() override;
	void cameraUpdate();
	void respawnEnemies();
	void clear();

	static void addTile(int x, int y, int id);
	static std::vector<ColliderComponent*> colliders;
	static SDL_Rect camera;
	static Color playerSkin;
};