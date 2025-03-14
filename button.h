#pragma once

#include <SDL.h>

class Button {
private:
	SDL_Texture* texture;
	SDL_Rect destRect, srcRect, destRectZoom;
	SDL_Rect current_destRect;
	bool is_hover;
	const char* tag;

public:
	Button();
	Button(const char* path, int x, int y, int w, int h, const char* tg);
	~Button();

	void render();
	const char* getTag();
	bool isHover(int &x, int &y);
};
