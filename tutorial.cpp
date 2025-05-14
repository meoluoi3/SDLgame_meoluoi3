#include "tutorial.h"
#include "menu.h"
#include <SDL_image.h>
#include "text.h"
#include "bits/stdc++.h"
#include "effect.h"
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "math.h"
#include "stage.h"
#include "draw.h"
#include "init.h"
#include "input.h"

static std::vector<SDL_Texture*> tutorialSlides;
static int currentSlide;
extern App app;
extern Stage stage;
extern int stageResetTimer;
bool first = false;
void loadTutorial(SDL_Renderer* renderer) {
    if (first == true) return;
    const char* paths[] = {
        "img/slide1.png",
        "img/slide2.png",
        
    };
    for (auto& p : paths) {
        tutorialSlides.push_back(IMG_LoadTexture(renderer, p));
    }
    currentSlide = 0;
    first == true;
}

void updateTutorial(SDL_Event& e) {
    switch (e.type) {
    case SDL_QUIT:
        exit(0);
        break;

    case SDL_KEYDOWN:
        doKeyDown(&e.key);
        if (e.key.keysym.sym == SDLK_RIGHT) {
            if (currentSlide < tutorialSlides.size() - 1) {
                currentSlide++;
                SDL_Log("Moved to slide %d (total slides: %d)", currentSlide, (int)tutorialSlides.size());
            }
            else {
                SDL_Log("At last slide (%d), cannot go right", currentSlide);
            }
        }
        else if (e.key.keysym.sym == SDLK_LEFT) {
            if (currentSlide > 0) {
                currentSlide--;
                SDL_Log("Moved to slide %d (total slides: %d)", currentSlide, (int)tutorialSlides.size());
            }
            else {
                SDL_Log("At first slide (0), cannot go left");
            }
        }
        else if (e.key.keysym.sym == SDLK_ESCAPE) {
            gameState = GS_MENU;
        }
        break;

    case SDL_KEYUP:
        doKeyUp(&e.key);
        break;

    case SDL_MOUSEBUTTONDOWN:
        handleMouseDown(&e.button);
        break;

    case SDL_MOUSEBUTTONUP:
        handleMouseUp(&e.button);
        break;

    default:
        break;
    }
}

void drawESC(SDL_Renderer* renderer) {
    if (!tutorialSlides.empty()) {
        SDL_RenderCopy(renderer, tutorialSlides[currentSlide], nullptr, nullptr);
    }
    SDL_Color white = { 255, 255, 255, 255 };
    renderText(renderer, "Press ESC to return", 10, 760, white);
}

bool play = false;
void logicTutorial() {
    if (currentSlide >= 1) {
        doPlayerMovement(wpnList);
        doFighters();
        doBullets();
        updateWeaponReloads(wpnList, SDL_GetTicks());
        clipPlayer();
    }

    if (currentSlide >= 2) {
        spawnEnemies();
        doEnemies();
        doExplosions();
        doDebris();
    }

    if (player == nullptr && --stageResetTimer <= 0) {
        SDL_Log("Calling resetStage()...\n");
        resetStage();
    }
}

void drawTutorial() {
    drawESC(app.renderer); // Draw the slide first
    drawFighters();
    drawBullets();
    drawDebris();
    drawExplosions();
    playerAndWeaponTexture(wpnList);
    int idx = wpnList.currentIndex;
    if (idx >= 0 && idx < (int)wpnList.list.size()) {
        drawAmmoHUD(app.renderer, wpnList.list[idx]);
    }
}