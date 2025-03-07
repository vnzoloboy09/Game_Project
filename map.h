#pragma once

class Map {
private:

public:
	Map();
	~Map();

	static void loadMap(const char* path, int sizeX, int sizeY);
};