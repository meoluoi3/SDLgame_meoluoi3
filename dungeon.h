#pragma once
#include <string>
#include "struct.h"
void initMap(void);
void loadTiles(void);
void drawMapTiles(void);
bool loadMapFromCSV(const std::string& filename, Map& map);
void logicDungeon(void);
void drawDungeon(void);
void drawFightersDungeon(void);