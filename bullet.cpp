#include "bullet.h"
#include "struct.h"
#include "stage.h"
#include "sound.h"
#include <SDL.h>

extern Stage stage;
extern Entity* player;
extern SDL_Texture* bulletTexture;
extern SDL_Texture* alienBullet;

void fireBullet()
{
    SDL_Log("Using player at %p", (void*)player);

    if (!player)
    {
        SDL_Log("fireBullet called but player is NULL");
        return;
    }
    Entity* bullet = new Entity();
    bullet->next = nullptr;
    int mouseX, mouseY;

    SDL_GetMouseState(&mouseX, &mouseY);

    bullet->x = player->x;
    bullet->y = player->y;

    double dx, dy;
    calcSlope(mouseX, mouseY, player->x, player->y, &dx, &dy);

    bullet->dx = dx * PLAYER_BULLET_SPEED;
    bullet->dy = dy * PLAYER_BULLET_SPEED;

    bullet->health = 1;
    bullet->texture = bulletTexture;
    SDL_QueryTexture(bullet->texture, nullptr, nullptr, &bullet->w, &bullet->h);

    bullet->y += (player->h / 2) - (bullet->h / 2);

    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    player->reload = FPS * 0.1;
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
