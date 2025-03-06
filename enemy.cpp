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
	base_corners.push_back({ static_cast<float>(destRect.x + destRect.w), static_cast<float>(destRect.y + destRect.h) });
	base_corners.push_back({ static_cast<float>(destRect.x), static_cast<float>(destRect.y + destRect.h) });

	for (int i = 0; i < 4; i++) {
		cur_corners.push_back(base_corners[i]);
	}

	first_sprite = _first_sprite;

	speed = 3.0f;
	velocity = { 1.0f, -1.0f };
	angle = 0.0f;

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

void Enemy::setPosition(int xpos, int ypos) {
	position.x = static_cast<float>(xpos);
	position.y = static_cast<float>(ypos);
}

void Enemy::chasePlayer(Vector2D player_pos) {
	float player_center_x = player_pos.x + static_cast<float>(CAR_WIDTH) / 2.0f;
	float player_center_y = player_pos.y + static_cast<float>(CAR_HEIGHT) / 2.0f;

	float enemy_center_x = position.x + static_cast<float>(CAR_WIDTH) / 2.0f;
	float enemy_center_y = position.y + static_cast<float>(CAR_HEIGHT) / 2.0f;

	// top left corner - bottom left corner -> direction
	SDL_FPoint direction = { cur_corners[0].x - cur_corners[3].x, 
		cur_corners[0].y - cur_corners[3].y };
	// a line from enemy center to player center
	SDL_FPoint line = { player_center_x - enemy_center_x, player_center_y - enemy_center_y };

	// decide to turn left or right arcorrding to the cross product of direction and line
	float cross = direction.x * line.y - direction.y * line.x;
	if (cross > 0) {
		angle += 1.5f;
	}
	else if (cross < 0) {
		angle -= 1.5f;
	}
}

void Enemy::update() {
	// 180 * PI to convert degree to radian
	position.x += static_cast<float>(velocity.x * sin(angle / 180 * PI) * speed);
	position.y += static_cast<float>(velocity.y * cos(angle / 180 * PI) * speed);

	base_corners[0] = { static_cast<float>(destRect.x), static_cast<float>(destRect.y) };
	base_corners[1] = { static_cast<float>(destRect.x + destRect.w), static_cast<float>(destRect.y) };
	base_corners[2] = { static_cast<float>(destRect.x + destRect.w), static_cast<float>(destRect.y + destRect.h) };
	base_corners[3] = { static_cast<float>(destRect.x), static_cast<float>(destRect.y + destRect.h) };

	for (int i = 0; i < 4; i++) {
		cur_corners[i] = Collision::rotatePoint(base_corners[i],
			position.x + destRect.w / 2.0f, position.y + destRect.h / 2.0f, angle);
	}

	destRect.x = static_cast<int>(position.x);
	destRect.y = static_cast<int>(position.y);
}

void Enemy::render() {
	current_sprite_id = (SDL_GetTicks() / SPRITE_DELAY + first_sprite) % sprites.size();
	Graphics::render(sprites[current_sprite_id], srcRect, destRect, angle, center, SDL_FLIP_NONE);
}

void Enemy::addSpeed(float s) {
	speed += s;
}

void Enemy::stop() {
	speed = 0;
}