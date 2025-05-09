#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "bits/stdc++.h"
extern TTF_Font* gFont;
extern SDL_Renderer* gRenderer;
void initFont(const char* fontPath, int fontSize);
void renderText(SDL_Renderer* renderer, const std::string& message, int x, int y, SDL_Color color,float scale = 1.0f);
void cleanupFont();