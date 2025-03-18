#pragma once

#include <SDL.h>
#include "graphics.h"
#include <SDL_ttf.h>

class UI {
protected:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;
	bool active;
	int scale;

public:
	UI(const char* text, TTF_Font* font, int xpos, int ypos) {
		texture = Graphics::loadText(text, font, {255,255,255});
		scale = 1;
		srcRect = { 0,0,0,0 };
		destRect = { xpos, ypos, 0, 0};
		TTF_SizeText(font, text, &destRect.w, &destRect.h);
		active = false;
	}

	UI(const char* path, int xpos, int ypos, int w, int h, int scl) {
		texture = Graphics::loadTexture(path);
		srcRect = { 0, 0, w, h };
		scale = scl;
		destRect = { xpos, ypos, w * scale, h * scale };
		active = false;
	}
	
	~UI() = default;
	
	void setSrc(int x, int y, int w, int h) {
		srcRect = { x, y, w, h };
	}

	void setDest(int x, int y, int w, int h) {
		destRect = { x, y, w * scale, h * scale };
	}

	void setDest(int x, int y) {
		destRect.x = x;
		destRect.y = y;
	}
	
	void setTexture(const char* text, TTF_Font* font) {
		texture = Graphics::loadText(text, font, { 0,0,0 });
		TTF_SizeText(font, text, &destRect.w, &destRect.h);
	}

	void render() {
		Graphics::render(texture, srcRect, destRect, 0, SDL_FLIP_NONE);
	}

	bool isActive() const { return active; }
	void activate() { active = true; }
	void deactivate() { active = false; }
};