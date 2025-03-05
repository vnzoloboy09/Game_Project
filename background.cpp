#include "background.h"
#include <iostream>

Background::Background(const char* path, int x, int y) {
	texture = Graphics::loadTexture(path);

	srcRect.x = x;
	srcRect.y = y;
	srcRect.w = SCREEN_WIDTH;
	srcRect.h = SCREEN_HEIGHT;

	destRect.x = 0;
	destRect.y = 0;
	destRect.w = srcRect.w * SCALE;
	destRect.h = srcRect.h * SCALE;
}

void Background::update(float x, float y) {
	srcRect.x = x + 16;
	srcRect.y = y + 32;
	srcRect.w = SCREEN_WIDTH;
	srcRect.h = SCREEN_HEIGHT;
}

void Background::render() {
	Graphics::draw(texture, srcRect, destRect);
}