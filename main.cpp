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
#include "math.h"
#include "weapon.h"

GameState gameState = GS_MENU;
GameState previousState = GS_MENU;
    App app;
    Entity* player;
    Entity bullet;
    Stage stage;
    Star stars[MAX_STARS];
    Mix_Chunk* sounds[SND_MAX];
    Mix_Music* music;
    PlayerWeapons wpnList;
    std::vector<SDL_Texture*> tiles;
    CameraGhost cameraghost;
    static int   currentFPS = 0;
     int   frameCount = 0;
    static Uint32 fpsTimer = 0;


    int main(int argc, char* argv[]) {
        srand(static_cast<unsigned>(time(0)));
        long then = SDL_GetTicks();
        float remainder = 0;
        fpsTimer = SDL_GetTicks();
         int   currentFPS = 0;
         int   frameCount = 0;
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
                if (gameState == GS_PLAYING and e.type == SDL_KEYDOWN and( e.key.keysym.sym == SDLK_p or e.key.keysym.sym == SDLK_ESCAPE) )
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
                case GS_MODE_SELECTION: updateModeSelection(e); break;
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

                if (app.delegate.logic) app.delegate.logic();
                if (app.delegate.draw)  app.delegate.draw();

                
                if (settings.showFPS) {
                    frameCount++;
                    Uint32 now = SDL_GetTicks();
                    if (now - fpsTimer >= 1000) {
                        currentFPS = frameCount;
                        frameCount = 0;
                        fpsTimer = now;
                    }

                    SDL_Color white = { 255,255,255,255 };
                    std::string txt = "FPS: " + std::to_string(currentFPS);
                    renderText(app.renderer, txt, 200, 10, white);
                }
                

                presentScene();
                break;

            case GS_PAUSED:
                drawPauseMenu(app.renderer);
                break;
            case GS_MODE_SELECTION:
                drawModeSelection(app.renderer);  
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