#include "enemy.h"

Enemy::Enemy(int x, int y, const char* path) {
	texture = Graphics::loadTexture(path);
	position.x = x;
	position.y = y;

	srcRect.w = 32;
	srcRect.h = 64;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = static_cast<int>(position.x);
	destRect.y = static_cast<int>(position.y);
	destRect.w = srcRect.w * SCALE;
	destRect.h = srcRect.h * SCALE;

	speed = 1;
}

void Enemy::update() {
	position.y += speed;

	srcRect.w = 32;
	srcRect.h = 64;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = static_cast<int>(position.x);
	destRect.y = static_cast<int>(position.y);
	destRect.w = srcRect.w * SCALE;
	destRect.h = srcRect.h * SCALE;
}

void Enemy::render() {
	Graphics::draw(texture, srcRect, destRect);
	SDL_RenderDrawRect(Game::renderer, &destRect);
}