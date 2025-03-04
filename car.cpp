#include "car.h"

Car::Car() {
	position.x = 0;
	position.y = 0;
	destRect = { 0,0,0,0 };
	srcRect = { 0,0,0,0 };
	current_sprite_id = 0;
	speed = 3;
	texture = NULL;
	angle = PI;
	center = NULL;
}
Car::~Car() = default;

void Car::update() {}

void Car::render() {}

void Car::loadSprite(const char* path, ...) {
	va_list args;
	va_start(args, path);
	const char* current_path = path;
	while (current_path) {
		sprites.push_back(Graphics::loadTexture(current_path));
		current_path = va_arg(args, const char*);
	}
	va_end(args);
}

Vector2D Car::getPosition() const {
	return position;
}

SDL_Rect Car::getRect() const {
	return destRect;
}