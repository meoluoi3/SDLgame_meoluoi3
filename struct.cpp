#include <SDL.h>
#include <SDL_image.h>
#include "struct.h"
#include <stdio.h>
#include "defs.h"
#include "draw.h"
#include "bits/stdc++.h"
extern App app;
extern Stage stage;
extern Entity* player;
SDL_Texture* bulletTexture = NULL;
SDL_Texture* enemyTexture = NULL;
int enemySpawnTimer = 0;
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

    bulletTexture = loadTexture("img/bullet_klee.png");
    enemyTexture = loadTexture("img/enemy.png");

    
}

 void initPlayer()
{
    player = new Entity;
    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x = 100;
    player->y = 100;
    player->texture = loadTexture("img/4.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
}

 void logic(void)
 {
     doPlayer();

     doFighters();

     doBullets();

     spawnEnemies();
 }

 void draw(void)
 {
    //drawPlayer();

     drawFighters();

     
     
     drawBullets();
 }

 void doPlayer(void)
{
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

    if (app.keyboard[SDL_SCANCODE_LCTRL] && player->reload == 0)
    {
        fireBullet();
    }

    player->x += player->dx;
    player->y += player->dy;

}   
 void fireBullet(void)
    {
        Entity* bullet = new Entity;

        
        
        stage.bulletTail->next = bullet;
        stage.bulletTail = bullet;

        bullet->x = player->x + 50;
        bullet->y = player->y + 200;
        bullet->dx = PLAYER_BULLET_SPEED;
        bullet->health = 1;
        bullet->texture = bulletTexture;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

        bullet->y += (player->h / 2) - (bullet->h / 2);

        player->reload = 8;
    }
    
 void doBullets(void)
    {
        Entity* b, * prev;

        prev = &stage.bulletHead;

        for (b = stage.bulletHead.next; b != NULL; b = b->next)
        {
            b->x += b->dx;
            b->y += b->dy;

            if (b->x > SCREEN_WIDTH)
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



void drawPlayer(void)
    {
        blit(player->texture, player->x, player->y,1.0);
    }

void drawBullets(void)
    {
        Entity* b;

        for (b = stage.bulletHead.next; b != NULL; b = b->next)
        {
            blit(b->texture, b->x, b->y,0.3);
        }
    }

void drawFighters(void) //drawing player and enemies into the game
    {
        Entity* e;

        for (e = stage.fighterHead.next; e != NULL; e = e->next)
        {
           if(e != player) blit(e->texture, e->x, e->y,0.2); // enemies
           else blit(e->texture, e->x, e->y, 1); //player
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

            if (e != player && e->x < -e->w)
            {
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
            enemy->y = int(rand()) % SCREEN_HEIGHT;
            enemy->texture = enemyTexture;
            SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

            enemy->dx = -(2 + (rand() % 4));

            enemySpawnTimer = 30 + (rand() % 60);
        }
    }