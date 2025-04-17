#include "button.h"
#include "graphics.h"

Button::Button() = default;

Button::Button(const char* path, int x, int y, int w, int h, const char* tg) {
	texture = Graphics::loadTexture(path);
	buttonHover = Graphics::loadSound("chunks/hover_button.wav");

	srcRect = { 0, 0, w, h };
	destRect.x = x;
	destRect.y = y;
	destRect.w = w;
	destRect.h = h;

	destRectZoom.x = x - w / 4;
	destRectZoom.y = y - h / 4;
	destRectZoom.w = w + w / 2;
	destRectZoom.h = h + h / 2;

	current_destRect = destRect;
	tag = tg;
	isSelected = false;
}

void Button::select(const bool sel) {
	isSelected = sel;
}

bool Button::isHover(int &x, int &y) {
	if (current_destRect.x <= x && x <= current_destRect.x + current_destRect.w &&
		current_destRect.y <= y && y <= current_destRect.y + current_destRect.h)
	{
		current_destRect = destRectZoom;
		if (playChunk == true) {
			Graphics::play(buttonHover);
			playChunk = false;
		}
		return true;
	}
	current_destRect = destRect;
	playChunk = true;
	return false;
}

void Button::render() {
	if (isSelected) {
		Graphics::drawRect(destRectZoom, GREEN, 3); // 3 is the thickness of the rect
		Graphics::render(texture, srcRect, destRectZoom, 0, SDL_FLIP_NONE);
	}
	else Graphics::render(texture, srcRect, current_destRect, 0, SDL_FLIP_NONE);
}

const char* Button::getTag() {
	return tag;
}

void Button::setTex(const char* path) {
	SDL_DestroyTexture(texture);
	texture = Graphics::loadTexture(path);
}

Button::~Button() {
	SDL_DestroyTexture(texture);
}

