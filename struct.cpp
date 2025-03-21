#include <SDL.h>
#include <SDL_image.h>
#include "struct.h"
#include <stdio.h>
#include "defs.h"
#include "draw.h"
#include "stage.h"
#include "bits/stdc++.h"
extern App app;
extern Stage stage;
extern Entity* player;
SDL_Texture* bulletTexture = NULL;
SDL_Texture* enemyTexture = NULL;
SDL_Texture* alienBullet = NULL;
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

    stage.fighterHead.next = NULL;
    stage.bulletHead.next = NULL;
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    initPlayer();

    bulletTexture = loadTexture("img/bullet_klee.png");
    enemyTexture = loadTexture("img/enemy.png");
    alienBullet = loadTexture("img/alienBullet.png");

    //resetStage();
}

void initPlayer()
{
    player = new Entity;
  

    memset(player, 0, sizeof(Entity));  // Ensures all fields are initialized

    player->next = NULL;  // Ensure proper termination

    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x = 100;
    player->y = 100;
    player->side = SIDE_PLAYER;
    player->health = 3; 
    player->texture = loadTexture("img/4.png");
    SDL_QueryTexture(player->texture, NULL, NULL, &player->w, &player->h);
   
}


 void logic(void)
 {
     doPlayer();
     doFighters();
     doBullets();
     spawnEnemies();
     doEnemies(); 
     clipPlayer();
     if (player == NULL)
     {
         printf("Player is NULL in logic()! Reset timer: %d\n", stageResetTimer);
     }

     if (player == NULL && --stageResetTimer <= 0)
     {
         printf("Calling resetStage()...\n");
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
     if (player != NULL)
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
     }
     /*player->x += player->dx;
     player->y += player->dy;*/

}   
 void fireBullet(void)
    {
        Entity* bullet = new Entity;

        memset(bullet, 0, sizeof(Entity));
        
        

        bullet->x = player->x ;
        bullet->y = player->y ;
        bullet->dx = PLAYER_BULLET_SPEED;
        bullet->health = 1;
        bullet->texture = bulletTexture;
        SDL_QueryTexture(bullet->texture, NULL, NULL, &bullet->w, &bullet->h);

        bullet->y += (player->h / 2) - (bullet->h / 2);

        stage.bulletTail->next = bullet;
        stage.bulletTail = bullet;

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



void drawPlayer(void)
    {
        blit(player->texture, player->x, player->y);
    }

void drawBullets(void)
    {
        Entity* b;

        for (b = stage.bulletHead.next; b != NULL; b = b->next)
        {
            blit(b->texture, b->x, b->y);
        }
    }

void drawFighters(void) //drawing player and enemies into the game
    {
    Entity* e;
   
    

        for (e = stage.fighterHead.next; e != NULL; e = e->next)
        {
            

           blit(e->texture, e->x, e->y); // enemies
          
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
void spawnEnemies(void)
{
    if (--enemySpawnTimer <= 0)
    {
        Entity* enemy = new Entity;
        memset(enemy, 0, sizeof(Entity));  // Ensures all fields, including `next`, are set to 0

        enemy->next = NULL;  // Explicitly set to NULL
        stage.fighterTail->next = enemy;
        stage.fighterTail = enemy;

        enemy->x = SCREEN_WIDTH;
        enemy->y = rand() % SCREEN_HEIGHT;
        enemy->texture = enemyTexture;
        enemy->side = SIDE_ALIEN;
        enemy->health = rand() % 5;
        SDL_QueryTexture(enemy->texture, NULL, NULL, &enemy->w, &enemy->h);

        enemy->dx = -(2 + (rand() % 4));
        enemySpawnTimer = 30 + (rand() % 60);
        enemy->reload = FPS * (1 + (rand() % 3));
    }
}

void fireAlienBullet(Entity* e)
{
    Entity* bullet = new Entity;

    
    memset(bullet, 0, sizeof(Entity));
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

int bulletHitFighter(Entity* b)
{
    Entity* e;

    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        if (e->side != b->side && collision(b->x, b->y, b->w, b->h, e->x, e->y, e->w, e->h))
        {
            b->health--;
            e->health--;

            return 1;
        }
    }

    return 0;
}