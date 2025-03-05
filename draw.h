#pragma once

void prepareScene(void);
void presentScene(void);
SDL_Texture* loadTexture(const char* filename);

void blit(SDL_Texture* texture, int x, int y, float scale);
void draw(void);
void drawPlayer(void);
void drawBullets(void);