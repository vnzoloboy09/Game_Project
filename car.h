#pragma once

#include <SDL.h>
#include <vector>
#include "vector2D.h"

class Car {
protected:
	SDL_Texture* texture;
	std::vector<SDL_Texture*> sprites;
	int current_sprite_id;
	int first_sprite;
	SDL_Rect srcRect, destRect;
	std::vector<SDL_FPoint> base_corners;
	std::vector<SDL_FPoint> cur_corners;

	Vector2D velocity;
	Vector2D position;
	Vector2D acceleration;
	float speed;
	float angle;
    SDL_Point* center;
public:
	Car();
	~Car();

	void loadSprite(const char* path, ...);
	virtual void update();
	virtual void render();
	
	Vector2D getPosition() const;
	SDL_Rect getRect() const;
	std::vector<SDL_FPoint> getCorners() const;
	SDL_FPoint getAxis(const SDL_FPoint a, const SDL_FPoint b) const;
	void printCurPoint();
};