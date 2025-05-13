#pragma once
#include <string>
#include "struct.h"
void initMap();
void loadTiles();
void drawMapTiles();
bool loadMapFromCSV(const std::string& filename, Map& map);
void logicDungeon();
void drawDungeon();
void drawFightersDungeon();
void initCameraGhost();
void updateCamera();
void resetDungeon();
void drawBulletDungeon();