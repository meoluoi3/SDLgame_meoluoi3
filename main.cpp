#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include "struct.h"
#include "input.h"
#include "draw.h"
#include "defs.h"
#include "stage.h"
#include "bits/stdc++.h"
#include "sound.h"
#include "text.h"
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

    playMusic(10);
    then = SDL_GetTicks();
    remainder = 0;

    while (true) { 
       
        prepareScene();
        doInput();
        
        app.delegate.logic();
        app.delegate.draw();
        
        presentScene();
        capFrameRate(&then, &remainder);
        
    }
    
    cleanupFont();
    delete player;
    return 0;
}
