#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

class Button {
private:
	SDL_Texture* texture;
	SDL_Rect destRect, srcRect, destRectZoom;
	SDL_Rect current_destRect;
	const char* tag;
	bool isSelected;
	bool playChunk = false;
	Mix_Chunk* buttonHover;

public:
	Button();
	Button(const char* path, int x, int y, int w, int h, const char* tg);
	~Button();

	void render();
	void select(const bool sel);
	const char* getTag();
	void setTex(const char* path);
	bool isHover(int &x, int &y);
};
