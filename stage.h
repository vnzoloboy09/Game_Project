#pragma once

class Stage {
protected:
	bool active = false;

public:
	Stage() = default;
	~Stage() = default;

	virtual void init() = 0;
	virtual void reInit() = 0;
	virtual void keyEvent() = 0;
	virtual void mouseEvent() = 0;
	virtual void handleEvent() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
	bool isActive() const {
		return active;
	}
	void activate() { active = true; }
	void deactivate() { active = false; }
};