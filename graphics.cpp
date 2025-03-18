#include "graphics.h"
#include "stageManager.h"
#include <iostream>

Graphics::Graphics() {}
Graphics::~Graphics() {}

SDL_Texture* Graphics::loadTexture(const char* path) {
	SDL_Surface* tmpsurface = IMG_Load(path);
    if (tmpsurface == NULL) {
        std::cerr << "Failed to load image: " << IMG_GetError() << std::endl;
        return NULL;
    }
	SDL_Texture* texture = SDL_CreateTextureFromSurface(StageManager::renderer, tmpsurface);
	SDL_FreeSurface(tmpsurface);
	if (texture == NULL) std::cerr << IMG_GetError() << '\n';

	return texture;
}

void Graphics::draw(SDL_Texture* texture, int x, int y, int w, int h) {
    SDL_Rect destRect = { x, y, w, h };
	SDL_RenderCopy(StageManager::renderer, texture, NULL, &destRect);
}

void Graphics::render(SDL_Texture* texture, SDL_Rect &srcRect, SDL_Rect &destRect,
    float angle, SDL_RendererFlip flip) {

    if (srcRect.x !=  0 || srcRect.y != 0 || srcRect.w != 0 || srcRect.h != 0) {
        SDL_RenderCopyEx(StageManager::renderer, texture, &srcRect, &destRect, angle, NULL, flip);
    }
    else {
        SDL_RenderCopyEx(StageManager::renderer, texture, NULL, &destRect, angle, NULL, flip);
    }
}

TTF_Font* Graphics::loadFont(const char* path, int size) {
    TTF_Font* gFont = TTF_OpenFont(path, size);
    if (gFont == NULL) {
        std::cerr << TTF_GetError();
    }
    return gFont;
}

SDL_Texture* Graphics::loadText(const char* text, TTF_Font* font, SDL_Color textColor) {
    SDL_Surface* tmpSur = TTF_RenderText_Solid(font, text, textColor);
    if (tmpSur == NULL) {
        std::cerr << "Failed to load text: " << TTF_GetError() << std::endl;
        return NULL;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(StageManager::renderer, tmpSur);
    SDL_FreeSurface(tmpSur);
    if (texture == NULL) std::cerr << TTF_GetError() << '\n';

    return texture;
}

void Graphics::drawRect(SDL_Rect rect, Color color, int thickness) {
    Graphics::setColor(color);
    for (int i = 0; i < thickness; ++i) {
        SDL_RenderDrawRect(StageManager::renderer, &rect);
        rect.x -= 1;
        rect.y -= 1;
        rect.w += 2;
        rect.h += 2;
    }
}

void Graphics::setColor(Color color) {
    switch (color) {
    case RED:
        SDL_SetRenderDrawColor(StageManager::renderer, 255, 0, 0, 255);
        break;
    case GREEN:
        SDL_SetRenderDrawColor(StageManager::renderer, 0, 255, 0, 255);
        break;
    case BLUE:
        SDL_SetRenderDrawColor(StageManager::renderer, 0, 0, 255, 255);
        break;
    case WHITE:
        SDL_SetRenderDrawColor(StageManager::renderer, 255, 255, 255, 255);
        break;
    case BLACK:
        SDL_SetRenderDrawColor(StageManager::renderer, 0, 0, 0, 255);
        break;
    }
}