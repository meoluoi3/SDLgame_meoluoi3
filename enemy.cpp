#include "enemy.h"
#include "struct.h"
#include "bullet.h"
#include "sound.h"
#include "bits/stdc++.h"
#include "stage.h"

extern Stage stage;
extern Entity* player;
extern SDL_Texture* enemyTexture;

int enemySpawnTimer = 0;

void doEnemies()
{
    for (Entity* e = stage.fighterHead.next; e != NULL; e = e->next)
    {

        if (e != player && player != NULL && --e->reload <= 0)
        {
            fireAlienBullet(e);
            playSound(SND_ALIEN_FIRE, CH_ALIEN_FIRE);
        }
    }
}

void spawnEnemies()
{
    if (--enemySpawnTimer <= 0)
    {
        Entity* enemy = new Entity();

        enemy->next = nullptr;
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        int side = rand() % 4; // 0: top, 1: bottom, 2: left, 3: right

        switch (side)
        {
        case 0: // Top
            enemy->x = rand() % SCREEN_WIDTH;
            enemy->y = 0;
            break;
        case 1: // Bottom
            enemy->x = rand() % SCREEN_WIDTH;
            enemy->y = SCREEN_HEIGHT - enemy->h;
            break;
        case 2: // Left
            enemy->x = 0;
            enemy->y = rand() % SCREEN_HEIGHT;
            break;
        case 3: // Right
            enemy->x = SCREEN_WIDTH - enemy->w;
            enemy->y = rand() % SCREEN_HEIGHT;
            break;
        }

        // Calculate direction toward player
        if (player != nullptr) calcSlope(player->x, player->y, enemy->x, enemy->y, &enemy->dx, &enemy->dy);
        else calcSlope(SCREEN_WIDTH/2 * rand() + 1, SCREEN_HEIGHT/2 * rand() +1, enemy->x, enemy->y, &enemy->dx, &enemy->dy);


        enemy->texture = enemyTexture;
        enemy->side = SIDE_ALIEN;
        enemy->health = (rand() % 5) + 1;

        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

//nemy->dx = -(2 + (rand() % 4));  // Speed: -2 to -5
        enemy->reload = FPS * (3 + (rand() % 3));  // Reload: 3-5 seconds

        enemySpawnTimer = 30 + (rand() % 60);  // Next spawn: 0.5–1.5s later
    }
}
