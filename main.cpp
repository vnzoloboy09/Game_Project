#include "stageManager.h"
#include "defs.h"

StageManager* stageManager = NULL;

int main(int argc, char* argv[]) {
	stageManager = new StageManager();
	stageManager->init();
	
	while (stageManager->isRunning()) {
		stageManager->presentStage();
	}

	stageManager->clear();

	return 0;
}