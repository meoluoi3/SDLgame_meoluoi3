#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "struct.h"
#include "input.h"
#include "draw.h"
#include "defs.h"
#include "stage.h"
#include "bits/stdc++.h"
App app;
Entity* player;
Entity bullet;
Stage stage;
Star stars[MAX_STARS];


int main(int argc, char* argv[]) {
    long then;
    float remainder;

    initSDL();
    initStage();    

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
    

    delete player;
    return 0;
}
