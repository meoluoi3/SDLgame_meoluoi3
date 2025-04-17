#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "defs.h"
#include "stage.h"
#include "bits/stdc++.h"
#include "struct.h"
#include "sound.h"


using namespace std;

extern App app;
extern Stage stage;
extern Entity* player;
extern int enemySpawnTimer;
extern int stageResetTimer;
extern int backgroundX;

void capFrameRate(long* then, float* remainder)
{
    long wait = 16 + *remainder;
    *remainder -= (int)*remainder;

    long frameTime = SDL_GetTicks() - *then;
    wait -= frameTime;

    if (wait < 1) wait = 1;

    SDL_Delay(wait);
    *remainder += 0.667;
    *then = SDL_GetTicks();
}

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
    return (max(x1, x2) < min(x1 + w1, x2 + w2)) &&
        (max(y1, y2) < min(y1 + h1, y2 + h2));
}

void resetStage(void)
{
    // Free all entities
    Entity* e;
    Explosion* ex;
    Debris* d;

    while ((e = stage.fighterHead.next))
    {
        stage.fighterHead.next = e->next;
        delete e;
    }

    while ((e = stage.bulletHead.next))
    {
        stage.bulletHead.next = e->next;
        delete e;
    }

    while ((ex = stage.explosionHead.next))
    {
        stage.explosionHead.next = ex->next;
        delete ex;
    }

    while ((d = stage.debrisHead.next))
    {
        stage.debrisHead.next = d->next;
        delete d;
    }

    // Reset pointers
    stage.fighterHead.next = NULL;
    stage.fighterTail = &stage.fighterHead;

    stage.bulletHead.next = NULL;
    stage.bulletTail = &stage.bulletHead;

    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    initPlayer();
    initStarfield();

    enemySpawnTimer = 0;
    stageResetTimer = FPS * 3;
}

void calcSlope(int x1, int y1, int x2, int y2, float* dx, float* dy)
{
    int steps = max(abs(x1 - x2), abs(y1 - y2));
    if (steps == 0)
    {
        *dx = *dy = 0;
        return;
    }

    *dx = (x1 - x2) / (float)steps;
    *dy = (y1 - y2) / (float)steps;
}

void clipPlayer()
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




int bulletHitFighter(Entity* b)
{
    for (Entity* e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e->side != b->side &&
            collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
        {
            // Play sound based on target
            if (e == player)
                playSound(SND_PLAYER_DIE, CH_PLAYER);
            else
                playSound(SND_ALIEN_DIE, CH_ANY);

            b->health--;
            e->health--;

            return 1;
        }
    }

    return 0;
}

int playerHitEnemy(Entity* player)
{
    for (Entity* e = stage.fighterHead.next; e != NULL; e = e->next)
    {

        if (e != player && e->side != player->side &&
            collision(player->x, player->y, player->w, player->h, e->x, e->y, e->w, e->h))
        {
            // Play sound based on health and target
            
         playSound(SND_PLAYER_DIE, CH_PLAYER);
            
         playSound(SND_ALIEN_DIE, CH_ANY);

         e->health--;
         player->health--;
            // Return after handling the collision
            return 1;
        }
    }

    return 0;
}


