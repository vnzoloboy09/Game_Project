#pragma once

#include <SDL_image.h>
#include <vector>
#include <time.h>

class ColliderComponent;

class Game {
private:
	SDL_Window* window;
	bool running = true;
	float score;
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
	void clear();

	static void addTile(int x, int y, int id);
	static std::vector<ColliderComponent*> colliders;
	static SDL_Renderer* renderer;
	static SDL_Event event;
	static SDL_Rect camera;
};