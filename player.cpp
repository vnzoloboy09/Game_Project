#include "player.h"
#include <SDL.h>

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

	angle = 0.0;
	speed = 4.0f;
}

void Player::stayInBound() {
	if (position.x < 0) position.x = 0;
	if (position.x > SCREEN_WIDTH - destRect.w) position.x = SCREEN_WIDTH - destRect.w;
	if (position.y < 0) position.y = 0;
	if (position.y > SCREEN_HEIGHT - destRect.h) position.y = SCREEN_HEIGHT - destRect.h;
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

	if (!keystate[SDL_SCANCODE_W] && !keystate[SDL_SCANCODE_S]) velocity = {0, 0};
}

void Player::update() {
	// 180 * PI to convert angle to radian
	position.x += static_cast<float>(velocity.x * sin(angle / 180 * PI) * speed);
    position.y += static_cast<float>(velocity.y * cos(angle / 180 * PI) * speed);

	destRect.x = static_cast<int>(position.x);
	destRect.y = static_cast<int>(position.y);
	destRect.w = srcRect.w * SCALE;
	destRect.h = srcRect.h * SCALE;
}

void Player::render() {
	//Graphics::draw(texture, srcRect, destRect);
	Graphics::render(texture, srcRect, destRect, angle, center, SDL_FLIP_NONE);
	SDL_RenderDrawRect(Game::renderer, &destRect);
}

Player::~Player() {
}