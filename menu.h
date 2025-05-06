#pragma once
#include <SDL.h>
#include "defs.h"

// Menu initialization/cleanup
bool initMenu(SDL_Renderer* renderer);
void cleanupMenu();

// Main Menu
void updateMainMenu(const SDL_Event& e);
void drawMainMenu(SDL_Renderer* renderer);

// Pause Menu
void updatePauseMenu(const SDL_Event& e);
void drawPauseMenu(SDL_Renderer* renderer);
