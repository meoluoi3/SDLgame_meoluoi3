#include <SDL.h>
#include <SDL_image.h>
#include "struct.h"
#include <stdio.h>
#include "defs.h"
#include "draw.h"

#include "stage.h"
#include "util.h"
#include "bits/stdc++.h"
extern App app;
extern Stage stage;
extern Entity* player;
SDL_Texture* bulletTexture = NULL;
SDL_Texture* enemyTexture = NULL;
SDL_Texture* alienBulletTexture = NULL;
SDL_Texture* playerTexture = NULL;

int enemySpawnTimer = 0;
int stageResetTimer{};
void initSDL(void) // Initialize SDL
{

    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED;

    windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_Log("Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    app.window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

    if (!app.window)
    {
        SDL_Log("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
        exit(1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

    app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

    if (!app.renderer)
    {
        SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }
}

void initStage(void) // Initialize the stage
{
    
    app.delegate.logic = logic;
    app.delegate.draw = draw;

   
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    initPlayer();

    bulletTexture = loadTexture("img/bullet.png");
    enemyTexture = loadTexture("img/enemy_emoji.png");
    alienBulletTexture = loadTexture("img/enemy_bullet.png");
    //playerTexture = loadTexture("gfx/player.png");

    //resetStage();

    
}

 void initPlayer()
{
    player = new Entity;
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x = 100;
    player->y = 100;
    player->texture = loadTexture("img/smile_emojigun.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
    player->side = SIDE_PLAYER;
}
    
 void logic(void)
 {
     doPlayer();

     doFighters();

     doBullets();

     spawnEnemies();    

     doEnemies();

     clipPlayer();

    if (player == NULL && --stageResetTimer <= 0)
     {
         resetStage();
     }
 }

 void draw(void)
 {
    //drawPlayer();

     drawFighters();

     drawBullets();
 }

 void doPlayer(void)
 {
     if(player != NULL){
     player->dx = player->dy = 0;

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

     if (app.keyboard[SDL_SCANCODE_LCTRL ] && player->reload == 0)
     {
         fireBullet();
     }

     player->x += player->dx;
     player->y += player->dy;
 }
}   
 void fireBullet(void) //taking bullet's info
    {
        Entity* bullet = new Entity;

        
        
        stage.bulletTail->next = bullet;
        stage.bulletTail = bullet;

        bullet->x = player->x+30;
        bullet->y = player->y+20;
        bullet->dx = PLAYER_BULLET_SPEED;
        bullet->health = 1;
        bullet->texture = bulletTexture;
        bullet->side = SIDE_PLAYER;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

        bullet->y += (player->h / 2) - (bullet->h / 2);

        
        //second bullet
        /*
        Entity* bullet2 = new Entity;
        stage.bulletTail->next = bullet2;
        stage.bulletTail = bullet2;

        bullet2->x = player->x + player->w;  // right of the player
        bullet2->y = player->y + (player->h / 2) + 10; // slightly below center
        bullet2->dx = PLAYER_BULLET_SPEED;
        bullet2->health = 1;
        bullet2->texture = bulletTexture;
        SDL_QueryTexture(bullet2->texture, NULL, NULL, &bullet2->w, &bullet2->h);
        */
        player->reload = 8;
    }
    
 void doBullets(void)
    {
        Entity* b, * prev;

        prev = &stage.bulletHead;

        for (b = stage.bulletHead.next; b != NULL; b = b->next)
        {
            b->x += b->dx; //move bullet horizontally
            b->y += b->dy; // move bullet vertically

            if ( b->x < -b->w || b->y < -b->h || b->x > SCREEN_WIDTH || b->y > SCREEN_HEIGHT || bulletHitFighter(b,0.2)) //if off-screen or bullet hit Fighter
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


void doFighters(void)
    {
        Entity* e, * prev;

        prev = &stage.fighterHead;

        for (e = stage.fighterHead.next; e != NULL; e = e->next)
        {
            e->x += e->dx;
            e->y += e->dy;

            if (e != player && e->x < -e->w )
            {
                e->health = 0;
            }

            if (e->health == 0)
            {
                if (e == player)
                {
                    player = NULL;
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

void spawnEnemies(void)
    {
        Entity* enemy;

        if (--enemySpawnTimer <= 0)
        {
            Entity* enemy = new Entity;
            stage.fighterTail->next = enemy;
            stage.fighterTail = enemy;

            enemy->x = SCREEN_WIDTH;
            enemy->y = int(rand()) % (SCREEN_HEIGHT-enemy->h);
            enemy->texture = enemyTexture;
            enemy->side = SIDE_ALIEN;
            enemy->health = int(rand()) % 3;
            
            SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);
            
            enemy->dx = -(2 + (rand() % 10));
            
            enemySpawnTimer = 10 + (rand() % 60);

            enemy->reload = FPS * (1 + (rand() % 3));
        }

    }
void resetStage(void)
{
    Entity* e;

    while (stage.fighterHead.next)
    {
        e = stage.fighterHead.next;
        stage.fighterHead.next = e->next;
        delete e;
    }

    while (stage.bulletHead.next)
    {
        e = stage.bulletHead.next;
        stage.bulletHead.next = e->next;
        delete e;
    }

    Stage stage;
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    initPlayer();

    enemySpawnTimer = 0;

    stageResetTimer = FPS * 2;
}   
void doEnemies(void)
{
    Entity* e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e != player && player != NULL && --e->reload <= 0)
        {
            fireAlienBullet(e);
        }   
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
    bullet->texture = alienBulletTexture;
    bullet->side = SIDE_ALIEN;
    SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

    bullet->x += (e->w / 2) - (bullet->w / 2);
    bullet->y += (e->h / 2) - (bullet->h / 2);

    calcSlope(player->x + (player->w / 2), player->y + (player->h / 2), e->x, e->y, &bullet->dx, &bullet->dy);

    bullet->dx *= ALIEN_BULLET_SPEED;
    bullet->dy *= ALIEN_BULLET_SPEED;

    e->reload = (rand() % FPS * 2);
}

void clipPlayer(void)
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

        if (player->x > SCREEN_WIDTH *4/5)
        {
            player->x = SCREEN_WIDTH *4/5;
        }

        if (player->y > SCREEN_HEIGHT - player->h)
        {
            player->y = SCREEN_HEIGHT - player->h;
        }
    }
}