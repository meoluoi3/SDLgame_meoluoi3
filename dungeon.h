#pragma once
void resetDungeon();
void logicDungeon();
void drawDungeon();
void initMap();
void loadTiles();
void drawMap();
void drawMapTiles();
void movePlayer(int dx, int dy);
void updateCameraDelay();
extern int moveDelay;