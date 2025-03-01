#include "car.h"

Car::Car() {}
Car::~Car() {}

void Car::update() {
}

void Car::render() {
}

Vector2D Car::getPosition() {
	return position;
}

SDL_Rect Car::getRect() {
	return destRect;
}