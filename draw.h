#pragma once
#include "struct.h"
void prepareScene();
void presentScene();
SDL_Texture* loadTexture(const char* filename);

void blit(SDL_Texture* texture, int x, int y);
void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y);
void blitRotated(SDL_Texture* texture, int x, int y, double angle);

void drawBullets();
void drawFighters();
void drawHealthBar(SDL_Renderer* renderer, Entity* entity );
void drawHud(SDL_Renderer* renderer, int score, int highscore);
void drawHealthText(SDL_Renderer* renderer, Entity* entity);