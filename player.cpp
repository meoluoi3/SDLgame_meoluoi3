#include "player.h"
#include "struct.h"
#include "effect.h"
#include "bullet.h"
#include "sound.h"
#include "stage.h"
#include "math.h"
#include "weapon.h"

extern App app;
extern Stage stage;
extern Entity* player;
//xtern PlayerWeapons wpnList;
void doPlayerMovement(PlayerWeapons& wpnList)
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

        if (app.keyboard[SDL_SCANCODE_1] && !app.prevKeyboard[SDL_SCANCODE_1]) {
            switchWeapon(wpnList, SDLK_1);
        }
        if (app.keyboard[SDL_SCANCODE_2] && !app.prevKeyboard[SDL_SCANCODE_2]) {
            switchWeapon(wpnList, SDLK_2);
        }
        if (app.keyboard[SDL_SCANCODE_3] && !app.prevKeyboard[SDL_SCANCODE_3]) {
            switchWeapon(wpnList, SDLK_3);
        }

        // Fire input
        if (app.mouseButtons[SDL_BUTTON_LEFT]) {
           
                //SDL_Log("Firing bullet...");
                
                fireBullet(wpnList);
            
            
        }
    }
}


void doFighters() {
    Entity* e = stage.fighterHead.next;
    Entity* prev = &stage.fighterHead;

    while (e != nullptr) {
        if (e != player && player != nullptr) {
            calcSlope(player->x, player->y, e->x, e->y, &e->dx, &e->dy);
            double speedIncreaseRate = 1;
            double speed = 4.0;
            e->dx *= speed;
            e->dy *= speed;
            speed += speedIncreaseRate;
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
            if (e == player) {
                player = nullptr;

            }
            addExplosions(e->x - e->w / 2, e->y - e->h / 2, 15);
            addDebris(e);

            

            if (e == stage.fighterTail) {
                stage.fighterTail = prev;
            }

            prev->next = e->next;

            Entity* temp = e->next;
            SDL_Log("Deleting entity at %p, health = %d", (void*)e, e->health);
            if (e != player) {
                SDL_Log("Enemy texture: %p, health: %d", (void*)e->texture, e->health);
            }

            delete e;
            e = temp;
        }
        else {
            prev = e;
            e = e->next;
        }
    }
}



