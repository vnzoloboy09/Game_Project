#pragma once

#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
#define WINDOW_TITLE "Car 2"
const float FPS = 60.0f;
const float frameDelay = 1000.0f/FPS;

// for tiles
const int TILE_SIZE = 32;
constexpr auto DIRT = 0;
constexpr auto LIGHT_GRASS = 1;
constexpr auto DARK_GRASS = 2;

// for cars
const int START_POSITION_X = SCREEN_WIDTH / 2 - 16;
const int START_POSITION_Y = SCREEN_HEIGHT / 2 - 32;
const int CAR_HEIGHT = 64;
const int CAR_WIDTH = 32;
const float ENEMY_SPEED = 4.0f;
const float PLAYER_SPEED = 7.0f;
const float GHOST_SPEED = 14.0f;
const float STEER_ANGEL = 1.7f;
const int MAX_ENEMIES = 10;
const int ENEMY_SPRITE_SPEED = 280;
const int EXPLOSION_TIME = 100;

// health bar;
const int PLAYER_BASE_HEALTH = 77;
const int HIT_DAMAGE = 10;
const int HEALTH_XPOS = 48;
const int HEALTH_YPOS = 27;
const int HEALTH_BAR_XPOS = 16;
const int HEALTH_BAR_YPOS = 0;
const int HEALTH_BAR_WIDTH = 96;
const int HEALTH_BAR_HEIGHT = 32;
const int HEALTH_BAR_SCALE = 2;
const int HEALTH_BAR_THICKNESS = 2;

// for map
const int MAP_WIDTH = 2560;
const int MAP_HEIGHT = 2560;

// weather
const int WEATHER_CHECK = 8; // weather might change every 8 scores
const int RAIN_IN = 100;
const int RAIN_OUT = 50;
const int START_RAINNING_CHANCE = 2;     // 50% to start a rain every weather check
const int STOP_RAINNING_CHANCE = 3;      // 66% to end a rain every weather check
const int LIGHTNING_STRIKE_CHANCE = 210; // 0.47% to have thunder during the rain
const int LIGHTNING_MAX = 225;
const int RAIN_SPEED = 150;

// Some other constants
const int SPRITE_DELAY = 300;
const float BOUND_BLOCK = PLAYER_SPEED;
const int SCORE_POS = 1200;
const int POWER_SPAWN_BORDER = 370;

const int NO_RANDOM_START_FRAME = 0;

const int TIME_PER_EXPLOSION = 80; // 80 game loop
const int DEATH_SCENCE_TIME = 3 * TIME_PER_EXPLOSION; 

const int MAX_VOLUME = 128;
const float DEGREE_PER_VOLUME = 360.0f / MAX_VOLUME;
const int VOLUME_STEP = 4;
const int SHIELD_WIDTH = CAR_WIDTH + 4;
const int SHIELD_HEIGHT = CAR_HEIGHT + 4;
const int SHIELD_DURATON = 550;
const int SHIELD_DURATION_LOW = 200;
const int SHIELD_SPRITE_SPEED = 200;

const int TILE_NUM = 80;

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