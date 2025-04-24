#pragma once
#include "menu.h"

class HowToPlayMenu : public Menu {
private:
	int scale = 3;

public:
	HowToPlayMenu();
	~HowToPlayMenu();

	void init() override;
	void update() override;
	void mouseEvent() override;
	void handleEvent() override;
	void keyEvent() override;
	void render() override;
};
