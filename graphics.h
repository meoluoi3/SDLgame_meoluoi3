#ifndef _GRAPHICS__H
#define _GRAPHICS__H

#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "struct.h"
struct Graphics {
    SDL_Renderer* renderer;
    SDL_Window* window;
    int up;
    int down;
    int left;
    int right;
    int fire;

    void logErrorAndExit(const char* msg, const char* error);
    void init();
    void prepareScene(SDL_Texture* background);
    void presentScene();
    SDL_Texture* loadTexture(const char* filename);
    void blit(SDL_Texture* texture, int x, int y);
    void quit();
};


#endif // _GRAPHICS__H
	