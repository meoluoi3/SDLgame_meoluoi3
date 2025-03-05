#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "struct.h"
#include "input.h"
#include "draw.h"
#include "defs.h"
#include "stage.h"

App app;
Entity* player;
Entity bullet;
Stage stage;


int main(int argc, char* argv[]) {
    long then;
    float remainder;

    initSDL();
    initStage();
    initPlayer();

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
