#include "map.h"
#include "game.h"
#include <fstream>
#include "defs.h"
#include <iostream>

Map::Map() {}
Map::~Map() {}

void Map::loadMap(const char* path, int sizeX, int sizeY) {
	char tile;
	std::fstream mapFile;
	mapFile.open(path);
	int cnt = 0;

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(tile);
			Game::addTile(x * TILE_SIZE, y * TILE_SIZE, atoi(&tile));
			mapFile.ignore();
			cnt++;
		}
	}
	std::cerr << "_______________________";
	std::cerr << cnt << '\n';

	mapFile.close();
}