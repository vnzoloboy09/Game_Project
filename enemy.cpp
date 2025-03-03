#include "enemy.h"

Enemy::Enemy(int x, int y) {
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

Enemy::~Enemy() {
	SDL_DestroyTexture(texture);
	texture = NULL;
	for (auto& sprite : sprites) {
		SDL_DestroyTexture(sprite);
		sprite = NULL;
	}
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
	current_sprite_id = SDL_GetTicks() / TIME_PER_SPRITE % sprites.size();
	//std::cerr << current_sprite_id << '\n';
	Graphics::draw(sprites[current_sprite_id], srcRect, destRect);
	SDL_RenderDrawRect(Game::renderer, &destRect);
}