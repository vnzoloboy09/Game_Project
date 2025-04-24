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

// use for quick rendering mostly for debug
void Graphics::draw(SDL_Texture* texture, int x, int y, int w, int h) {
    SDL_Rect destRect = { x, y, w, h };
	SDL_RenderCopy(StageManager::renderer, texture, NULL, &destRect);
}

// use for rendering sprites, ones that need spinning
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

void Graphics::drawArc(int cx, int cy, int r, float startDeg, float endDeg) {
    const int segments = MAX_VOLUME;
    float startRad = startDeg * M_PI / 180.0f;
    float endRad = endDeg * M_PI / 180.0f;
    float step = (endRad - startRad) / segments;

    for (int i = 0; i < segments; ++i) {
        float theta1 = startRad + i * step;
        float theta2 = startRad + (i + 1) * step;

        int x1 = static_cast<int>(cx + r * cos(theta1));
        int y1 = static_cast<int>(cy + r * sin(theta1));
        int x2 = static_cast<int>(cx + r * cos(theta2));
        int y2 = static_cast<int>(cy + r * sin(theta2));

        SDL_RenderDrawLine(StageManager::renderer, x1, y1, x2, y2);
    }
}

Mix_Chunk* Graphics::loadSound(const char* path) {
    Mix_Chunk* gChunk = Mix_LoadWAV(path);
    if (gChunk == NULL) {
        std::cerr << Mix_GetError();
    }
    return gChunk;
}

void Graphics::play(Mix_Chunk* chunk) {  
   if (chunk != nullptr && !StageManager::mute) {  
       Mix_VolumeChunk(chunk, StageManager::volume);
       Mix_PlayChannel(-1, chunk, 0);  
   }  
}

void Graphics::play(Mix_Music* music) {
	if (music != nullptr && !StageManager::mute) {
		Mix_VolumeMusic(StageManager::volume);
		Mix_PlayMusic(music, -1);
	}
}

Mix_Music* Graphics::loadMusic(const char* path) {
	Mix_Music* gMusic = Mix_LoadMUS(path);
	if (gMusic == NULL) {
		std::cerr << path << ' ' << Mix_GetError() << '\n';
	}
	return gMusic;
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