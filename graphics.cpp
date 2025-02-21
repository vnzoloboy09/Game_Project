#include "graphics.h"

Graphics::Graphics() {}
Graphics::~Graphics() {}

SDL_Texture* Graphics::loadTexture(const char* path) {
	SDL_Surface* tmpsurface = IMG_Load(path);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmpsurface);
	SDL_FreeSurface(tmpsurface);

	return texture;
}

void Graphics::draw(SDL_Texture* texture, SDL_Rect scrRect, SDL_Rect destRect) {
	SDL_RenderCopy(Game::renderer, texture, &scrRect, &destRect);
}