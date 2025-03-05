#include "player.h"
#include "collision.h"
#include "graphics.h"
#include "game.h"
#include <SDL.h>
#include <iostream>

Player::Player(int x, int y, const char* path) {
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

	base_corners.push_back({ static_cast<float>(destRect.x), static_cast<float>(destRect.y) });
	base_corners.push_back({ static_cast<float>(destRect.x + destRect.w), static_cast<float>(destRect.y) });
	base_corners.push_back({ static_cast<float>(destRect.x + destRect.w), static_cast<float>(destRect.y + destRect.h) });
	base_corners.push_back({ static_cast<float>(destRect.x), static_cast<float>(destRect.y + destRect.h) });

	for (int i = 0; i < 4; i++) {
		cur_corners.push_back(base_corners[i]);
	}

	angle = 0.0;
	speed = 4.0f;
	velocity = { 0.0f, 0.0f };
	acceleration = { 0.0f, 0.0f };
}

void Player::stayInBound() {
	for (int i = 0; i < 4; i++) {
		if (cur_corners[i].x >= SCREEN_WIDTH) {
			position.x -= BOUND_BLOCK;
		}
		if (cur_corners[i].x <= 0) {
			position.x += BOUND_BLOCK;
		}
		if (cur_corners[i].y >= SCREEN_HEIGHT) {
			position.y -= BOUND_BLOCK;
		}
		if (cur_corners[i].y <= 0) {
			position.y += BOUND_BLOCK;
		}	
	}

}

void Player::control() {
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_W]) velocity = { 1, -1 };
	if (keystate[SDL_SCANCODE_S]) velocity = { -1, 1 };
	
	if (keystate[SDL_SCANCODE_D]) {
		if (keystate[SDL_SCANCODE_W]) angle += 1.5f;
		if (keystate[SDL_SCANCODE_S]) angle -= 1.5f;
	}
	
	if (keystate[SDL_SCANCODE_A]) {
		if (keystate[SDL_SCANCODE_W]) angle -= 1.5f;
		if (keystate[SDL_SCANCODE_S]) angle += 1.5f;
	}

	if (!keystate[SDL_SCANCODE_W] && !keystate[SDL_SCANCODE_S]) {
		velocity = { 0.0f, 0.0f };
	}
}

void Player::update() {
	// 180 * PI to convert angle to radian
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

void Player::render() {
	//Graphics::draw(texture, srcRect, destRect);
	Graphics::render(texture, srcRect, destRect, angle, center, SDL_FLIP_NONE);
	SDL_RenderDrawRect(Game::renderer, &destRect);
}

Player::~Player() {
	SDL_DestroyTexture(texture);
	texture = NULL;
	for (auto& sprite : sprites) {
		SDL_DestroyTexture(sprite);
		sprite = NULL;
	}
}