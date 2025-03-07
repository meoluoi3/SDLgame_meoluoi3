#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "defs.h"
#include "stage.h"
#include "struct.h"
#include "util.h"
extern Stage stage;
void capFrameRate(long* then, float* remainder)
{
    long wait, frameTime;

    wait = 16 + *remainder;

    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;

    *then = SDL_GetTicks();
}

int bulletHitFighter(Entity* b,float scale)
{
    Entity* e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e->side != b->side && collision(b->x, b->y, b->w*scale, b->h*scale, e->x, e->y, e->w, e->h))
        {
            b->health = 0;
            e->health--;

            return 1;
        }
    }

    return 0;
}

int playerHitFighter(Entity* player, float scale) {
    Entity* e;
    
    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e->side != player->side && collision(player->x, player->y, player->w * scale, player->h * scale, e->x, e->y, e->w, e->h))
        {
            player->health = 0;
            e->health--;
            
            return 1;
        }
    }

    return 0;

}


