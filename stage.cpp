#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "defs.h"
#include "stage.h"
#include "bits/stdc++.h"

#include "sound.h"
#include "init.h"
#include "dungeon.h"
#include "weapon.h"
#include "text.h"


using namespace std;

extern App app;
extern Stage stage;
extern Entity* player;
extern int enemySpawnTimer;
extern int stageResetTimer;
extern int backgroundX;

int modeSelectionIndex = 0;  

const char* modeSelectionLabels[] = { "Survivor Mode", "Dungeon Mode" };

void resetStage()
{
    Entity* e;
    Explosion* ex;
    Debris* d;

    while ((e = stage.fighterHead.next))
    {
        stage.fighterHead.next = e->next;
        delete e;
    }
    stage.fighterTail = &stage.fighterHead;

    while ((e = stage.bulletHead.next))
    {
        stage.bulletHead.next = e->next;
        delete e;
    }
    stage.bulletTail = &stage.bulletHead;

    while ((ex = stage.explosionHead.next))
    {
        stage.explosionHead.next = ex->next;
        delete ex;
    }
    stage.explosionTail = &stage.explosionHead;

    while ((d = stage.debrisHead.next))
    {
        stage.debrisHead.next = d->next;
        delete d;
    }
    stage.debrisTail = &stage.debrisHead;

    resetWeapons();
    initStarfield();
    stage.score = 0;
    enemySpawnTimer = 0;
    stageResetTimer = FPS * 3;
}



void updateModeSelection(SDL_Event& e) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    if (e.type == SDL_KEYDOWN) {
        switch (e.key.keysym.sym) {
        case SDLK_w:
        case SDLK_UP:
            modeSelectionIndex = (modeSelectionIndex - 1 + 2) % 2;
            break;
        case SDLK_s:
        case SDLK_DOWN:
            modeSelectionIndex = (modeSelectionIndex + 1) % 2;
            break;
        case SDLK_RETURN:
        case SDLK_SPACE:
            if (modeSelectionIndex == 0) {
                stage.mode = SURVIVOR_MODE;
                initSurvivor();
            }
            else {
                stage.mode = DUNGEON_MODE;
                initDungeon();
            }
            gameState = GS_PLAYING;
            break;
        case SDLK_ESCAPE:
            gameState = GS_MENU;
            break;
        }
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        for (int i = 0; i < 2; ++i) {
            int textW = 0, textH = 0;
            if (gFont) {
                TTF_SizeText(gFont, modeSelectionLabels[i], &textW, &textH);
            }

            SDL_Rect menuItemRect = {
                SCREEN_WIDTH / 2 - textW / 2,
                SCREEN_HEIGHT / 2 + i * 40 - textH / 2,
                textW,
                textH
            };

            if (mouseX >= menuItemRect.x && mouseX <= menuItemRect.x + menuItemRect.w &&
                mouseY >= menuItemRect.y && mouseY <= menuItemRect.y + menuItemRect.h) {
                modeSelectionIndex = i;
                if (i == 0) {
                    stage.mode = SURVIVOR_MODE;
                    initSurvivor();
                }
                else {
                    stage.mode = DUNGEON_MODE;
                    initDungeon();
                }
                gameState = GS_PLAYING;
                break;
            }
        }
    }
}





void drawModeSelection(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    int hoveredIndex = -1;

    // Check if mouse is hovering over any of the buttons
    for (int i = 0; i < 2; ++i) { // Two modes: Survivor Mode and Dungeon Mode
        int textW = 0, textH = 0;
        if (gFont) {
            TTF_SizeText(gFont, modeSelectionLabels[i], &textW, &textH);
        }

        SDL_Rect menuItemRect = {
            SCREEN_WIDTH / 2 - textW / 2,
            SCREEN_HEIGHT / 2 + i * 40 - textH / 2,
            textW,
            textH
        };

        // Check if the mouse is over the current button
        if (mouseX >= menuItemRect.x && mouseX <= menuItemRect.x + menuItemRect.w &&
            mouseY >= menuItemRect.y && mouseY <= menuItemRect.y + menuItemRect.h) {
            hoveredIndex = i;
        }
    }

    // Draw the menu items with proper color based on hover
    for (int i = 0; i < 2; ++i) {
        SDL_Color color;

        if (i == hoveredIndex) {
            color = SDL_Color{ 0, 255, 0, 255 }; // Hovered (green)
        }
        else if (hoveredIndex == -1 && i == modeSelectionIndex) {
            color = SDL_Color{ 0, 255, 0, 255 }; // Selected via keyboard
        }
        else {
            color = SDL_Color{ 255, 255, 255, 255 }; // Normal (white)
        }

        int textW = 0, textH = 0;
        if (gFont) {
            TTF_SizeText(gFont, modeSelectionLabels[i], &textW, &textH);
        }

        renderText(renderer,
            modeSelectionLabels[i],
            SCREEN_WIDTH / 2 - textW / 2,
            SCREEN_HEIGHT / 2 + i * 40 - textH / 2,
            color);
    }

    SDL_RenderPresent(renderer);
}

