#include "audio.h"
#include "stageManager.h"
#include "defs.h"
#include <iostream>

int Audio::volume = MAX_VOLUME / 2;
bool Audio::mute = false;

Mix_Chunk* Audio::loadSound(const char* path) {
	Mix_Chunk* gChunk = Mix_LoadWAV(path);
	if (gChunk == NULL) {
		std::cerr << Mix_GetError();
	}
	return gChunk;
}

void Audio::play(Mix_Chunk* chunk) {
	if (chunk != nullptr && !Audio::mute) {
		Mix_VolumeChunk(chunk, Audio::volume);
		Mix_PlayChannel(-1, chunk, 0);
	}
}

void Audio::play(Mix_Music* music) {
	if (music != nullptr) {
		Mix_VolumeMusic(Audio::volume);
		Mix_PlayMusic(music, -1);
	}
	if (Audio::mute) Mix_PauseMusic();
}

Mix_Music* Audio::loadMusic(const char* path) {
	Mix_Music* gMusic = Mix_LoadMUS(path);
	if (gMusic == NULL) {
		std::cerr << path << ' ' << Mix_GetError() << '\n';
	}
	return gMusic;
}