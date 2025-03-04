#pragma once

#include <vector>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;
#define WINDOW_TITLE "Car"
const int FPS = 60;
const int timePerFrame = FPS/1000;

const float PI = 3.1415926535;

// For cars
const int START_POSITION_X = SCREEN_WIDTH / 2 - 16;
const int START_POSITION_Y = 560;
const int SCALE = 1;
const int CAR_HEIGHT = 64;
const int CAR_WIDTH = 32;
const float MAX_ACCELERATION = 5.0;
const float MAX_STEERING = 30;

//for background
const int LANE_WIDTH = 96; // 3 times the car width
const int BACKGROUND_SCROLLING_SPEED = 4;
const int BACKGROUND_WIDTH = 900;
const int BACKGROUND_HEIGHT = 740;
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
