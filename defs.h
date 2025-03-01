#pragma once

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;
#define WINDOW_TITLE "Car"
const int FPS = 60;
const int timePerFrame = FPS/1000;

// For cars
const int START_POSITION_X = SCREEN_WIDTH / 2 - 16;
const int START_POSITION_Y = 560;
const int SCALE = 1;
const int CAR_LENGTH = 64;
const float MAX_ACCELERATION = 5.0;
const float MAX_STEERING = 30;

//for background
const int LANE_WIDTH = 96; // 3 times the car width
const int BACKGROUND_SCROLLING_SPEED = 4;
const int BACKGROUND_WIDTH = 900;
const int BACKGROUND_HEIGHT = 740;

typedef enum {
	RED,
	BLUE,
	GREEN,
	WHITE,
	BLACK
} Color;
