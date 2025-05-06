#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include "bits/stdc++.h "    

#include "defs.h"
#include "struct.h"
#include "input.h"
#include "draw.h"
#include "stage.h"
#include "sound.h"
#include "text.h"
#include "init.h"
#include "settings.h"
#include "tutorial.h"
#include "menu.h"

GameState gameState = GS_MENU;
GameState previousState = GS_MENU;
    App app;
    Entity* player;
    Entity bullet;
    Stage stage;
    Star stars[MAX_STARS];
    Mix_Chunk* sounds[SND_MAX];
    Mix_Music* music;

    int main(int argc, char* argv[]) {
        srand(static_cast<unsigned>(time(0)));
        long then{};
        float remainder{};

        initSDL();
        initStage();    
        initMenu(app.renderer);
        loadTutorial(app.renderer);
        applySettings();


        playMusic(10);
        then = SDL_GetTicks();
        remainder = 0;

        SDL_Event e;
        while (gameState != GS_EXIT) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                    gameState = GS_EXIT;
                    break;
                }

                // Pause toggle
                if (gameState == GS_PLAYING and e.type == SDL_KEYDOWN and e.key.keysym.sym == SDLK_p)
                {
                    gameState = GS_PAUSED;
                    continue;
                }

                // Route input
                switch (gameState) {
                case GS_MENU:     updateMainMenu(e);    break;
                case GS_TUTORIAL: updateTutorial(e);    break;
                case GS_SETTINGS: updateSettings(e);    break;
                case GS_PLAYING:  handleInputEvent(e);  break;
                case GS_PAUSED:   updatePauseMenu(e);   break;
                default: break;
                }
            }
           

            // Draw / logic by state...
            switch (gameState) {
            case GS_MENU:     drawMainMenu(app.renderer); break;
            case GS_TUTORIAL: drawTutorial(app.renderer); break;
            case GS_SETTINGS: drawSettings(app.renderer); break;
            case GS_PLAYING:
                prepareScene();
                app.delegate.logic();
                app.delegate.draw();
                presentScene();
                break;
            case GS_PAUSED:
                drawPauseMenu(app.renderer);
                break;
            default: break;
            }

            capFrameRate(&then, &remainder);
        }

        cleanupMenu();
        cleanupFont();
        SDL_Quit();
        return 0;
    }