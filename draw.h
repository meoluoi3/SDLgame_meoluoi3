#pragma once

void prepareScene(void);
void presentScene(void);
SDL_Texture* loadTexture(const char* filename);

void blit(SDL_Texture* texture, int x, int y);

void drawBullets(void);
void drawFighters(void);