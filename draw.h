#pragma once

void prepareScene();
void presentScene();
SDL_Texture* loadTexture(const char* filename);

void blit(SDL_Texture* texture, int x, int y);
void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y);

void drawBackground();
void drawStarfield();
void drawDebris();
void drawExplosions();
void drawBullets();
void drawFighters();