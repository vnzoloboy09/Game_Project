#include "game.h"
#include "defs.h"

Game* game = NULL;

int main(int argc, char* argv[]) {
	game = new Game();

	Uint32 frameStart;
	int frametime;

	game->init();
	while (game->isRunning()) {
		frameStart = SDL_GetTicks();
		
		game->handleEvent();
		game->update();
		game->render();

		// limiting frame time
		frametime = SDL_GetTicks() - frameStart;
		if (frametime < timePerFrame) {
			SDL_Delay(timePerFrame - frametime);
		}
	}

	game->clear();

	return 0;
}