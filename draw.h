#pragma once

void prepareScene(void);
void presentScene(void);
SDL_Texture* loadTexture(const char* filename, SDL_Renderer* renderer);

void blit(SDL_Texture* texture, int x, int y, float scale);