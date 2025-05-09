#pragma once
#include "struct.h"
void doBackground();
void doStarfield();
void doExplosions();
void doDebris();
void addExplosions(int x, int y, int num);
void addDebris(Entity* e);


void drawBackground();
void drawStarfield();
void drawDebris();
void drawExplosions();

extern SDL_Texture* explosionTexture;
extern SDL_Texture* background;
extern Star stars[MAX_STARS];

extern int backgroundX;