#include "bullet.h"
#include "struct.h"
#include "bits/stdc++.h"
#include "stage.h"

extern Stage stage;
extern Entity* player;
extern SDL_Texture* bulletTexture;
extern SDL_Texture* alienBullet;

void fireBullet()
{
    Entity* bullet = new Entity();
    int mouseX, mouseY;

    // Get mouse position
    SDL_GetMouseState(&mouseX, &mouseY);

    bullet->x = player->x;
    bullet->y = player->y;

    // Use calcSlope to calculate the direction of the bullet
    float dx, dy;
    calcSlope(mouseX, mouseY, player->x, player->y, &dx, &dy);

    
    bullet->dx = dx * PLAYER_BULLET_SPEED;
    bullet->dy = dy * PLAYER_BULLET_SPEED;

    bullet->health = 1;  // Bullet health (or lifetime)
    bullet->texture = bulletTexture;  
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    // Adjust bullet's vertical position to center it based on the player's height
    bullet->y += (player->h / 2) - (bullet->h / 2);

    // Add the bullet to the stage's linked list of bullets
    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    // Set player reload time
    player->reload = FPS * 0.1;
}


void doBullets()
{
    Entity* b, * prev;

    prev = &stage.bulletHead;

    for (b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        b->x += b->dx;
        b->y += b->dy;

        if (bulletHitFighter(b) || b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT)
        {
            if (b == stage.bulletTail)
            {
                stage.bulletTail = prev;
            }

            prev->next = b->next;
            delete b;
            b = prev;
        }

        prev = b;
    }
}

void fireAlienBullet(Entity* e)
{
    Entity* bullet = new Entity;

    stage.bulletTail->next = bullet;
    stage.bulletTail = bullet;

    bullet->x = e->x;
    bullet->y = e->y;
    bullet->health = 1;
    bullet->texture = alienBullet;
    bullet->side = SIDE_ALIEN;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);

    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;

    e->reload = (rand() % FPS * 2);
}
