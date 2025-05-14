#pragma once
// Tutorial
#include <SDL.h>  

void loadTutorial(SDL_Renderer* renderer);
void updateTutorial(SDL_Event& e);
void drawESC(SDL_Renderer* renderer);
void logicTutorial();
void drawTutorial();