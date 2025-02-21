#include "game.h"

Game* game = NULL;

int main(int argc, char* argv[]) {
	game = new Game();

	game->init();
	while (game->isRunning()) {
		game->handleEvent();
		game->update();
		game->render();
	}

	game->clear();

	return 0;
}