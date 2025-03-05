#pragma once

#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
#define WINDOW_TITLE "Car"
const int FPS = 60;
const int timePerFrame = FPS/1000;

const float PI = 3.1415926f;

// for cars
const int START_POSITION_X = SCREEN_WIDTH / 2 - 16;
const int START_POSITION_Y = SCREEN_HEIGHT / 2 - 32;
const int SCALE = 1;
const int CAR_HEIGHT = 64;
const int CAR_WIDTH = 32;
const float MAX_ACCELERATION = 20.0f;
const float MAX_VELOCITY = 20.0f;

// for background
const int LANE_WIDTH = 96; // 3 times the car width
const int BACKGROUND_SCROLLING_SPEED = 4;
const int BACKGROUND_WIDTH = SCREEN_WIDTH * 2;
const int BACKGROUND_HEIGHT = SCREEN_HEIGHT * 2;
const float BOUND_BLOCK = 2.5f;
const std::vector<int> LANES = { 132, 228, 324, 420 };

const int SPRITE_DELAY = 300;
const int MAX_ENEMIES = 5;

typedef enum {
	RED,
	BLUE,
	GREEN,
	WHITE,
	BLACK
} Color;
