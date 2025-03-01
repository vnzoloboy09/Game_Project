#include "background.h"

Background::Background(const char* path, int y) {
	texture = Graphics::loadTexture(path);

	srcRect.x = 0;
	srcRect.y = 0;
	srcRect.w = BACKGROUND_WIDTH;
	srcRect.h = BACKGROUND_HEIGHT;

	destRect.x = 0;
	destRect.y = y;
	destRect.w = srcRect.w * SCALE;
	destRect.h = srcRect.h * SCALE;
}

void Background::update(int scrollspeed) {
	destRect.y += scrollspeed;
	if (destRect.y >= SCREEN_HEIGHT) {
		destRect.y = -BACKGROUND_HEIGHT;
	}
}

void Background::render() {
	Graphics::draw(texture, srcRect, destRect);
}