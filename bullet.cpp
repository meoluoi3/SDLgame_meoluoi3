#include "bullet.h"
#include "struct.h"
#include "stage.h"
#include "sound.h"
#include <SDL.h>
#include "math.h"
#include "weapon.h"
extern Stage stage;
extern Entity* player;
extern SDL_Texture* bulletTexture;
extern SDL_Texture* alienBullet;

void fireBullet(PlayerWeapons& wpnList) {
    int idx = wpnList.currentIndex;
    if (idx < 0 || idx >= (int)wpnList.list.size()) return;
    Weapon& w = wpnList.list[idx];

    Uint32 currentTime = SDL_GetTicks();
    

    Uint32 fireDelay = static_cast<Uint32>(w.fireRate * 1000);
    if (currentTime - w.lastFireTime < fireDelay) return;
    if (w.type == WeaponType::Knife) {
        // Implement knife attack logic here (e.g., melee damage)
        w.lastFireTime = currentTime;  // Update last fire time for knife
        playSound(SND_PLAYER_FIRE, CH_PLAYER);
        return;
    }

    if (w.ammo > 0) {
        w.ammo--;  // Decrease ammo count for gun

        // Play gunfire sound
        playSound(SND_PLAYER_FIRE, CH_PLAYER);

        // Create and spawn the bullet
        Entity* bullet = new Entity();
        bullet->next = nullptr;
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        bullet->x = player->x - player->w / 2;
        bullet->y = player->y;
        double rad = player->angle * (PI / 180.0);
        bullet->dx = cos(rad) * PLAYER_BULLET_SPEED;
        bullet->dy = sin(rad) * PLAYER_BULLET_SPEED;

        bullet->health = 1;
        bullet->texture = bulletTexture;
        SDL_QueryTexture(bullet->texture, nullptr, nullptr, &bullet->w, &bullet->h);
        bullet->y += (player->h / 2) - (bullet->h / 2);

        stage.bulletTail->next = bullet;
        stage.bulletTail = bullet;

        // Update last fire time
        w.lastFireTime = currentTime;

        // If ammo is zero after firing, initiate reload
        if (w.ammo == 0) {
            w.reloadtime = currentTime;
            SDL_Log("[%s] reload started at %u", w.name.c_str(), currentTime);
        }
    }
}

void doBullets()
{
    Entity* b = stage.bulletHead.next;
    Entity* prev = &stage.bulletHead;

    while (b)
    {
        b->x += b->dx;
        b->y += b->dy;

        if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT)
        {
            Entity* toDelete = b;
            Entity* nextNode = b->next;

            if (b == stage.bulletTail)
                stage.bulletTail = prev;

            prev->next = nextNode;
            delete toDelete;

            b = nextNode;
        }
        else
        {
            prev = b;
            b = b->next;
        }
    }
}


void fireAlienBullet(Entity* e)
{
    Entity* bullet = new Entity;
    bullet->next = nullptr;
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->health = 1;
    bullet->texture = alienBullet;
    bullet->side = SIDE_ALIEN;
    SDL_QueryTexture(bullet->texture, nullptr, nullptr, &bullet->w, &bullet->h);

    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);

    double dx, dy;
    calcSlope(
        player->x + (player->w / 2),
        player->y + (player->h / 2),
        e->x,
        e->y,
        &dx,
        &dy
    );

    bullet->dx = dx * ALIEN_BULLET_SPEED;
    bullet->dy = dy * ALIEN_BULLET_SPEED;

    e->reload = std::rand() % (FPS * 2);
}
