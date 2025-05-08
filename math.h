#pragma once
#include "struct.h"
double getAngle(int x1, int y1, int x2, int y2);
bool checkForQuitEvent();
void capFrameRate(long* then, float* remainder);
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void calcSlope(int x1, int y1, int x2, int y2, double* dx, double* dy);
int bulletHitFighter(Entity* b);
int playerHitEnemy(Entity* player);