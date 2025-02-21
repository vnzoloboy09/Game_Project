#include "car.h"
#include "graphics.h"

Car::Car(int x, int y, const char* path) {
	carTexture = Graphics::loadTexture(path);
}
Car::~Car() {}

void Car::update() {
	xpos += velocity_x;
	ypos += velocity_y;

	srcRect.w = 32;
	srcRect.h = 64;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}

void Car::render() {
	Graphics::draw(carTexture, srcRect, destRect);
}