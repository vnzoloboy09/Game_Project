#include "car.h"
#include "graphics.h"

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

std::vector<SDL_FPoint> Car::getCorners() const {
	return cur_corners;
}

SDL_FPoint Car::getAxis(const SDL_FPoint a, const SDL_FPoint b) const {
	SDL_FPoint axis = { b.x - a.x, b.y - a.y };
	SDL_FPoint normal = { -axis.y, axis.x };
	float length = sqrt(normal.x * normal.x + normal.y * normal.y);
	normal.x /= length;
	normal.y /= length;
	return normal;
}

void Car::printCurPoint() {
	for (auto i : cur_corners) {
		std::cerr << i.x << ' ' << i.y << '\n';
	}
	std::cerr << '\n';
}