#include "tutorial.h"
#include "menu.h"
#include <SDL_image.h>
#include "text.h"
#include "bits/stdc++.h"
static std::vector<SDL_Texture*> tutorialSlides;
static int currentSlide;

void loadTutorial(SDL_Renderer* renderer) {
    const char* paths[] = {
        "img/slide1.png",
        "img/slide2.png",
        // add more slides here
    };
    for (auto& p : paths) {
        tutorialSlides.push_back(IMG_LoadTexture(renderer, p));
    }
    currentSlide = 0;
}

void updateTutorial(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_RIGHT)
            currentSlide = std::min(currentSlide + 1, (int)tutorialSlides.size() - 1);
        else if (e.key.keysym.sym == SDLK_LEFT)
            currentSlide = std::max(currentSlide - 1, 0);
        else if (e.key.keysym.sym == SDLK_ESCAPE)
            gameState = GS_MENU;
    }
}

void drawTutorial(SDL_Renderer* renderer) {
    SDL_RenderClear(renderer);
    if (!tutorialSlides.empty())
        SDL_RenderCopy(renderer, tutorialSlides[currentSlide], nullptr, nullptr);
    SDL_Color white = { 255,255,255,255 };
    renderText(renderer, "Press ESC to return", 10, 760, white);
    SDL_RenderPresent(renderer);
}
