#pragma once

#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
#define WINDOW_TITLE "Car"
const float FPS = 60.0f;
const float frameDelay = 1000.0f/FPS;

// for tiles
const int TILE_SIZE = 32;
constexpr auto dirt = 0;
constexpr auto light_grass = 1;
constexpr auto dark_grass = 2;

// for cars
const int START_POSITION_X = SCREEN_WIDTH / 2 - 16;
const int START_POSITION_Y = SCREEN_HEIGHT / 2 - 32;
const int CAR_HEIGHT = 64;
const int CAR_WIDTH = 32;
const float ENEMY_SPEED = 4.0f;
const float PLAYER_SPEED = 5.0f;
const int GHOST_SPEED = 14.0f;
const int MAX_ENEMIES = 10;

// for map
const int MAP_WIDTH = 2560;
const int MAP_HEIGHT = 2560;

// weather
const int WEATHER_CHECK = 8;
const int RAIN_IN = 100;
const int RAIN_OUT = 50;
const int START_RAINNING_CHANCE = 2; // 50%
const int STOP_RAINNING_CHANCE = 3; // 66%
const int LIGHTNING_STRIKE_CHANCE = 210; // 0.0047%
const int LIGHTNING_MAX = 240;

// Some other constants
const int SPRITE_DELAY = 300;
const float BOUND_BLOCK = PLAYER_SPEED;
const int PLAYER_BASE_HEALTH = 77;
const int SCORE_POS = 1200;

const int NO_RANDOM_START_FRAME = 0;

const int TIME_PER_EXPLOSION = 80; // 80 game loop
const int DEATH_SCENCE_TIME = 3 * TIME_PER_EXPLOSION; 

const int MAX_VOLUME = 128;
const float DEGREE_PER_VOLUME = 360.0f / MAX_VOLUME;
const int VOLUME_STEP = 4;


typedef enum {
	RED,
	BLUE,
	GREEN,
	WHITE,
	BLACK,
	YELLOW
} Color;

enum groupLables : unsigned int {
	groupMap,
	groupPlayers,
	groupEnemies,
	groupColliders,
	groupPowerUps
};

typedef enum {
	GAME_STAGE,
	MENU_STAGE,
	CHOOSE_STAGE
} tStage;