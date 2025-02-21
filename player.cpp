#include "player.h"
#include <SDL.h>h

Player::Player(int x, int y, const char* path) {
	texture = Graphics::loadTexture(path);
	position.x = x;
	position.y = y;
}

void Player::control() {
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	if (keystate[SDL_SCANCODE_W]) velocity.y = -1;
	if (keystate[SDL_SCANCODE_S]) velocity.y = 1;
	if (keystate[SDL_SCANCODE_D]) velocity.x = 1;
	if (keystate[SDL_SCANCODE_A]) velocity.x = -1;

	if (!keystate[SDL_SCANCODE_W] && !keystate[SDL_SCANCODE_S]) velocity.y = 0;
	if (!keystate[SDL_SCANCODE_D] && !keystate[SDL_SCANCODE_A]) velocity.x = 0;
}

void Player::update() {
	control();

	position.x += velocity.x * speed;
	position.y += velocity.y * speed;

	srcRect.w = 32;
	srcRect.h = 64;
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = static_cast<int>(position.x);
	destRect.y = static_cast<int>(position.y);
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
}

void Player::render() {
	Graphics::draw(texture, srcRect, destRect);
}

Player::~Player() {
}