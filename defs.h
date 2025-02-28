#pragma once

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 700;
#define WINDOW_TITLE "Car"
const int FPS = 60;
const int timePerFrame = 60/1000;

const int START_POISITION_X = SCREEN_WIDTH / 2 - 16;
const int START_POISITION_Y = 560;
const int SCALE = 1;
const int CAR_LENGTH = 64;
const float MAX_ACCELERATION = 5.0;
const float MAX_STEERING = 30;

typedef enum {
	RED,
	BLUE,
	GREEN,
	WHITE,
	BLACK
} Color;