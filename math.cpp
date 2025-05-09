#include "math.h"
#include "defs.h"
#include <SDL.h>
#include "struct.h"
#include "sound.h"
#include "bits/stdc++.h"
using namespace std;
extern Entity* player;
extern Stage stage;
double getAngle(int x1, int y1, int x2, int y2)
{
    double angle = -180 + atan2(y1 - y2, x1 - x2) * (180 / PI);
    return angle >= 0 ? angle : 360 + angle;
}
bool checkForQuitEvent() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            return true;
        }
    }
    return false;
}

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
    return (max(x1, x2) < min(x1 + w1, x2 + w2)) && (max(y1, y2) < min(y1 + h1, y2 + h2));
}

void calcSlope(int x1, int y1, int x2, int y2, double* dx, double* dy)
{
    double steps = max(abs(x1 - x2), abs(y1 - y2));
    if (steps == 0)
    {
        *dx = *dy = 0;
        return;
    }

    *dx = (x1 - x2) / steps;
    *dy = (y1 - y2) / steps;
}

void clipPlayer()
{
    if (player != nullptr)
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
    for (Entity* e = stage.fighterHead.next; e != nullptr; e = e->next)
    {
        if (e->side != b->side &&
            collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
        {
            b->health--;

            if (e == player)
            {


                if (player->health == 1)playSound(SND_PLAYER_DIE, CH_PLAYER);
                player->health--;
                // player->isImmortal = true;
                // player->immortalityStartTime = SDL_GetTicks();


            }
            else
            {
                if (e->health == 1)  playSound(SND_ALIEN_DIE, CH_ANY);
                e->health--;

            }

            if (e->health == 0) {
                stage.score++;
                stage.highscore = max(stage.score, stage.highscore);
            }
            return 1;
        }
    }

    return 0;
}

int playerHitEnemy(Entity* player)
{
    for (Entity* e = stage.fighterHead.next; e != nullptr; e = e->next)
    {

        if (e != player && e->side != player->side &&
            collision(player->x, player->y, player->w, player->h, e->x, e->y, e->w, e->h))
        {
            // Play sound based on health and target

            playSound(SND_PLAYER_DIE, CH_PLAYER);

            playSound(SND_ALIEN_DIE, CH_ANY);
            int pl = player->health;
            int en = e->health;
            if (pl < en) {
                e->health -= player->health;
                player->health = 0;
            }
            else if (pl > en) {
                player->health -= e->health;
                e->health = 0;

            }
            else {
                player->health = 0;
                e->health = 0;
            }
            // Return after handling the collision
            return 1;
        }
    }

    return 0;
}
