#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "defs.h"
#include "stage.h"
#include "bits/stdc++.h"
#include "struct.h"
using namespace std;
extern App app;
extern Stage stage;
extern Entity* player;
extern int enemySpawnTimer;
extern int stageResetTimer;
extern int backgroundX;
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
int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return (max(x1, x2) < min(x1 + w1, x2 + w2)) && (max(y1, y2) < min(y1 + h1, y2 + h2));
}
void resetStage(void)
{
    Explosion* ex;
    Debris* d;

    Entity* e;

    // Free all fighters
    while (stage.fighterHead.next)
    {
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
        delete e;
    }

    // Free all bullets
    while (stage.bulletHead.next)
    {
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        delete e;
    }

    while (stage.explosionHead.next)
    {
        ex = stage.explosionHead.next;
        stage.explosionHead.next = ex->next;
        delete ex;
    }

    while (stage.debrisHead.next)
    {
        d = stage.debrisHead.next;
        stage.debrisHead.next = d->next;
        delete d;
    }

    // Reset stage pointers without overwriting everything
    stage.fighterHead.next = NULL;
    stage.bulletHead.next = NULL;
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    // Reinitialize player
    initPlayer();


    initStarfield();

    enemySpawnTimer = 0;
    stageResetTimer = FPS * 3   ;
}
void calcSlope(int x1, int y1, int x2, int y2, float* dx, float* dy)
{
    int steps = max(abs(x1 - x2), abs(y1 - y2));

    if (steps == 0)
    {
        *dx = *dy = 0;
        return;
    }

    *dx = (x1 - x2);
    *dx /= steps;

    *dy = (y1 - y2);
    *dy /= steps;
}
void clipPlayer(void)
{
    if (player != NULL)
    {
        if (player->x < 0)
        {
            player->x = 0;
        }

        if (player->y < 0)
        {
            player->y = 0;
        }

        if (player->x > SCREEN_WIDTH - player->w)
        {
            player->x = SCREEN_WIDTH - player->w;
        }

        if (player->y > SCREEN_HEIGHT - player->h)
        {
            player->y = SCREEN_HEIGHT - player->h;
        }
    }
}