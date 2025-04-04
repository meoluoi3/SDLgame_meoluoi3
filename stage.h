#pragma once
#include "struct.h"
void capFrameRate(long* then, float* remainder);
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void resetStage(void);
void calcSlope(int x1, int y1, int x2, int y2, float* dx, float* dy);
int bulletHitFighter(Entity* b);
