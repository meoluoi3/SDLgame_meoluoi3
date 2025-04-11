#include "enemy.h"
#include "struct.h"
#include "bits/stdc++.h"
#include "bullet.h"
#include "struct.h"
#include "sound.h"
extern Stage stage;
extern Entity* player;
int enemySpawnTimer = 0;
extern SDL_Texture* enemyTexture;
void doEnemies()
{
    Entity* e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
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
        

        enemy->next = NULL;  // Explicitly set to NULL
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->x = SCREEN_WIDTH - 100;
        enemy->y = rand() % (SCREEN_HEIGHT - 200);
        enemy->texture = enemyTexture;
        enemy->side = SIDE_ALIEN;
        enemy->health = rand() % 5+1;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

        enemy->dx = -(2 + (rand() % 4));
        enemySpawnTimer = 30 + (rand() % 60);
        enemy->reload = FPS * (1 + (rand() % 3));
    }
}