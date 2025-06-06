#pragma once

#include "menu.h"

class DeathMenu : public Menu {
public:
	DeathMenu();
	~DeathMenu();

	void init() override;
	void update() override;
	void mouseClickEvent(Button* button);
	void mouseEvent() override;
	void handleEvent() override;
	void keyEvent() override;
	void render() override;
};