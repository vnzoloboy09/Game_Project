#pragma once
#include "menu.h"

class ChooseMenu : public Menu {
private:
	int scale = 3;

public:
	ChooseMenu();
	~ChooseMenu();

	void init() override;
	void update() override;
	void handleEvent() override;
};