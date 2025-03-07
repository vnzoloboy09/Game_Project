#include "graphics.h"
#include "game.h"
#include <iostream>

Graphics::Graphics() {}
Graphics::~Graphics() {}

SDL_Texture* Graphics::loadTexture(const char* path) {
	SDL_Surface* tmpsurface = IMG_Load(path);
    if (tmpsurface == NULL) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return NULL;
    }
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, tmpsurface);
	SDL_FreeSurface(tmpsurface);
	if (texture != NULL) std::cerr << "Loading " << path << '\n';
	else std::cerr << IMG_GetError() << '\n';

	return texture;
}

void Graphics::draw(SDL_Texture* texture, SDL_Rect scrRect, SDL_Rect destRect) {
	SDL_RenderCopy(Game::renderer, texture, &scrRect, &destRect);
}

void Graphics::render(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect,
    float angle, SDL_RendererFlip flip) {

    SDL_RenderCopyEx(Game::renderer, texture, &srcRect, &destRect, angle, NULL, flip);
}

void Graphics::setColor(Color color) {
    switch (color) {
    case RED:
        SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);
        break;
    case GREEN:
        SDL_SetRenderDrawColor(Game::renderer, 0, 255, 0, 255);
        break;
    case BLUE:
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 255, 255);
        break;
    case WHITE:
        SDL_SetRenderDrawColor(Game::renderer, 255, 255, 255, 255);
        break;
    case BLACK:
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);
        break;
    }
}