#pragma once

#include <SDL_mixer.h>

class Audio {
public:
	Audio();
	~Audio();

	static int volume;
	static bool mute;

	static Mix_Chunk* loadSound(const char* path);
	static void play(Mix_Chunk* chunk);

	static Mix_Music* loadMusic(const char* path);
	static void play(Mix_Music* music);
};