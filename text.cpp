#include "text.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "bits/stdc++.h"

TTF_Font* gFont = nullptr;
SDL_Renderer* gRenderer = nullptr;

void initFont(const char* fontPath, int fontSize) {
    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    gFont = TTF_OpenFont(fontPath, fontSize);
    if (gFont == nullptr) {
        SDL_Log("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}

void renderText(SDL_Renderer* renderer, const std::string& message, int x, int y, SDL_Color color, float scale) {
    if (!gFont) {
        SDL_Log("Font not loaded!\n");
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, message.c_str(), color);
    if (textSurface == nullptr) {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr) {
        SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        return;
    }

    SDL_Rect renderQuad = { x, y, textSurface->w * scale, textSurface->h * scale };
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    }



void cleanupFont() {
    if (gFont != nullptr) {
        TTF_CloseFont(gFont);
        gFont = nullptr;
    }
    TTF_Quit();
}
