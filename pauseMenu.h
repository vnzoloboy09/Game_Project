#pragma once

#include "menu.h"

class PauseMenu : public Menu {
private:
	
public:
	PauseMenu();
	~PauseMenu();

	void init() override;
	void update() override;
	void mouseEvent() override;
	void handleEvent() override;
	void keyEvent() override;
	void render() override;
};