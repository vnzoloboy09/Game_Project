#pragma once

#include <SDL.h>
#include <vector>
#include "vector2D.h"
#include "graphics.h"

class Car {
protected:
	SDL_Texture* texture;
	std::vector<SDL_Texture*> sprites;
	int current_sprite_id;
	int first_sprite;
	Vector2D velocity;
	Vector2D position;
	float speed;
	SDL_Rect srcRect, destRect;

public:
	Car();
	~Car();

	void loadSprite(const char* path, ...);
	virtual void update();
	virtual void render();
	
	Vector2D getPosition() const;
	SDL_Rect getRect() const;
};