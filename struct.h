#ifndef STRUCT_H
#define STRUCT_H

#include <SDL.h>
#include <SDL_image.h>

struct Entities
{
    int x;
    int y;
    int dx;
    int dy;
    int health;
    SDL_Texture* texture;
}; 



#endif
