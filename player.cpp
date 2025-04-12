#include "player.h"
#include "struct.h"
#include "effect.h"
#include "bullet.h"
#include "sound.h"

extern App app;
extern Stage stage;
extern Entity* player;

void doPlayerMovement(void)
{
    if (player != nullptr)
    {
        player->dx = 0;
        player->dy = 0;

        if (player->reload > 0)
        {
            player->reload--;
        }

        if (app.keyboard[SDL_SCANCODE_UP])
        {
            player->dy = -PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_DOWN])
        {
            player->dy = PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_LEFT])
        {
            player->dx = -PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_RIGHT])
        {
            player->dx = PLAYER_SPEED;
        }

        if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload <= 0)
        {
            playSound(SND_PLAYER_FIRE, CH_PLAYER);
            fireBullet();
        }
    }
}

void doFighters(void)
{
    Entity* e;
    Entity* prev = &stage.fighterHead;

    for (e = stage.fighterHead.next; e != nullptr; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if (e != player && e->x + e->w < 0)
        {
            e->health = 0;
        }

        if (e->health == 0)
        {
            addExplosions(e->x - e->w / 2, e->y - e->h / 2, 15);
            addDebris(e);

            if (e == player)
            {
                player = nullptr;
            }

            if (e == stage.fighterTail)
            {
                stage.fighterTail = prev;
            }

            prev->next = e->next;
            delete e;
            e = prev;
        }

        prev = e;
    }
}
