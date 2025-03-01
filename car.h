#pragma once

#include <SDL.h>
#include "vector2D.h"
#include "graphics.h"

class Car {
protected:
	SDL_Texture* texture;
	Vector2D velocity;
	Vector2D position;
	float speed = 3;
	SDL_Rect srcRect, destRect;

public:
	Car();
	~Car();

	virtual void update();
	virtual void render();
	
	Vector2D getPosition();
	SDL_Rect getRect();
};