#include "enemy.h"
#include "graphics.h"
#include "game.h"
#include "collision.h"

Enemy::Enemy(int x, int y, int _first_sprite) {
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

	base_corners.push_back({ static_cast<float>(destRect.x), static_cast<float>(destRect.y) });
	base_corners.push_back({ static_cast<float>(destRect.x + destRect.w), static_cast<float>(destRect.y) });
	base_corners.push_back({ static_cast<float>(destRect.x), static_cast<float>(destRect.y + destRect.h) });
	base_corners.push_back({ static_cast<float>(destRect.x + destRect.w), static_cast<float>(destRect.y + destRect.h) });

	for (int i = 0; i < 4; i++) {
		cur_corners.push_back(base_corners[i]);
	}

	first_sprite = _first_sprite;

	speed = 4;
	loadSprite("imgs/car/Police_1.png", 
		"imgs/car/Police_2.png", 
		"imgs/car/Police_3.png", 
		"imgs/car/Police_4.png", NULL);
}

Enemy::~Enemy() {
	SDL_DestroyTexture(texture);
	texture = NULL;
	for (auto& sprite : sprites) {
		SDL_DestroyTexture(sprite);
		sprite = NULL;
	}
}

void Enemy::setPosition(Vector2D pos) {
	position = pos;
}

void Enemy::update() {
	position.y += speed;

	if (position.y > SCREEN_HEIGHT) {
		position.y = -64;
		position.x = LANES[rand() % LANES.size()];
	}

	base_corners[0] = { static_cast<float>(destRect.x), static_cast<float>(destRect.y) };
	base_corners[1] = { static_cast<float>(destRect.x + destRect.w), static_cast<float>(destRect.y) };
	base_corners[2] = { static_cast<float>(destRect.x), static_cast<float>(destRect.y + destRect.h) };
	base_corners[3] = { static_cast<float>(destRect.x + destRect.w), static_cast<float>(destRect.y + destRect.h) };

	for (int i = 0; i < 4; i++) {
		cur_corners[i] = Collision::rotatePoint(base_corners[i],
			position.x + destRect.w / 2.0f, position.y + destRect.h / 2.0f, angle);
	}

	destRect.x = static_cast<int>(position.x);
	destRect.y = static_cast<int>(position.y);
}

void Enemy::render() {
	current_sprite_id = (SDL_GetTicks() / SPRITE_DELAY % sprites.size());
	current_sprite_id = (current_sprite_id + first_sprite) % sprites.size();
	//std::cerr << current_sprite_id << '\n';
	Graphics::draw(sprites[current_sprite_id], srcRect, destRect);
	SDL_RenderDrawRect(Game::renderer, &destRect);
}

void Enemy::addSpeed(float s) {
	speed += s;
}

void Enemy::stop() {
	speed = 0;
}