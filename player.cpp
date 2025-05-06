#include "player.h"
#include "struct.h"
#include "effect.h"
#include "bullet.h"
#include "sound.h"
#include "stage.h"

extern App app;
extern Stage stage;
extern Entity* player;

void doPlayerMovement()
{
    if (player != nullptr)
    {
        player->dx = 0;
        player->dy = 0;

        // Reduce reload time
        if (player->reload > 0)
        {
            player->reload--;
        }

        // Movement input
        if (app.keyboard[SDL_SCANCODE_W]) player->dy = -PLAYER_SPEED;
        if (app.keyboard[SDL_SCANCODE_S]) player->dy = PLAYER_SPEED;
        if (app.keyboard[SDL_SCANCODE_A]) player->dx = -PLAYER_SPEED;
        if (app.keyboard[SDL_SCANCODE_D]) player->dx = PLAYER_SPEED;


        // Fire input
        if (app.mouseButtons[SDL_BUTTON_LEFT] && player->reload <= 0)
        {
            playSound(SND_PLAYER_FIRE, CH_PLAYER);
            fireBullet();
        }
    }
}


void doFighters() {
    Entity* e = stage.fighterHead.next;
    Entity* prev = &stage.fighterHead;

    while (e != nullptr) {
        if (e != player && player != nullptr) {
            calcSlope(player->x, player->y, e->x, e->y, &e->dx, &e->dy);
            double speed = 4.0;
            e->dx *= speed;
            e->dy *= speed;
        }

       

        // Apply movement
        e->x += e->dx;
        e->y += e->dy;

        // Remove off-screen enemies
        if (e != player && e->x + e->w < 0) {
            e->health = 0;
        }

        // Collision (placeholder)
        if (player != nullptr && playerHitEnemy(player)) {
            // e.g. player->health = 0;
        }

        // Handle death
        if (e->health <= 0) {
            addExplosions(e->x - e->w / 2, e->y - e->h / 2, 15);
            addDebris(e);

            if (e == player) {
                player = nullptr;
            }

            if (e == stage.fighterTail) {
                stage.fighterTail = prev;
            }

            prev->next = e->next;

            Entity* temp = e->next;
            delete e;
            e = temp;
        }
        else {
            prev = e;
            e = e->next;
        }
    }
}



