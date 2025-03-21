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
extern Star stars[MAX_STARS];
SDL_Texture* bulletTexture = NULL;
SDL_Texture* enemyTexture = NULL;
SDL_Texture* alienBullet = NULL;
SDL_Texture* background = NULL;
SDL_Texture* explosionTexture = NULL;
int enemySpawnTimer = 0;
int stageResetTimer{};
int backgroundX{};

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
    SDL_ShowCursor(0);
}
void initStage(void) // Initialize the stage
{
    
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    stage.fighterHead.next = NULL;
    stage.bulletHead.next = NULL;
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    stage.explosionHead.next = NULL;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    initPlayer();

    bulletTexture = loadTexture("img/bullet_klee.png");
    enemyTexture = loadTexture("img/enemy.png");
    alienBullet = loadTexture("img/alienBullet.png");
    background = loadTexture("img/background.png");
    explosionTexture = loadTexture("img/explosion.png");
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
     doBackground();
     doStarfield();
     doPlayer();
     doFighters();
     doBullets();
     spawnEnemies();
     doEnemies(); 
     clipPlayer();
     doExplosions();
     doDebris();
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
     drawBackground();
     drawStarfield();
     drawFighters();
     drawBullets();
     drawDebris();
     drawExplosions();
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
void initStarfield(void)
{
    int i;

    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].speed = 1 + rand() % 8;
    }
}
void doBackground(void)
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}
void doStarfield(void)
{
    int i;

    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x -= stars[i].speed;

        if (stars[i].x < 0)
        {
            stars[i].x = SCREEN_WIDTH + stars[i].x;
        }
    }
}
void doExplosions(void)
{

    Explosion* e, * prev;

    prev = &stage.explosionHead;

    for (e = stage.explosionHead.next; e != NULL; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if (--e->a <= 0)
        {
            if (e == stage.explosionTail)
            {
                stage.explosionTail = prev;
            }

            prev->next = e->next;
            free(e);
            e = prev;
        }

        prev = e;
        printf("Explosions active: %d\n");
    }
}
void doDebris(void)
{
    Debris* d, * prev;

    prev = &stage.debrisHead;

    for (d = stage.debrisHead.next; d != NULL; d = d->next)
    {
        d->x += d->dx;
        d->y += d->dy;

        d->dy += 0.5;

        if (--d->life <= 0)
        {
            if (d == stage.debrisTail)
            {
                stage.debrisTail = prev;
            }

            prev->next = d->next;
            free(d);
            d = prev;
        }

        prev = d;
    }
}
void addExplosions(int x, int y, int num)
{
    printf("Explosion added\n");
    Explosion* e;
    int i;

    for (i = 0; i < num; i++)
    {
        e = new Explosion;
        memset(e, 0, sizeof(Explosion));
        stage.explosionTail->next = e;
        stage.explosionTail = e;

        e->x = x + (rand() % 32) - (rand() % 32);
        e->y = y + (rand() % 32) - (rand() % 32);
        e->dx = (rand() % 10) - (rand() % 10);
        e->dy = (rand() % 10) - (rand() % 10);

        e->dx /= 10;
        e->dy /= 10;

        switch (rand() % 4)
        {
        case 0:
            e->r = 255;
            break;

        case 1:
            e->r = 255;
            e->g = 128;
            break;

        case 2:
            e->r = 255;
            e->g = 255;
            break;

        default:
            e->r = 255;
            e->g = 255;
            e->b = 255;
            break;
        }

        e->a = rand() % FPS * 3;
    }
}
void addDebris(Entity* e)
{
    Debris* d;
    int x, y, w, h;

    w = e->w / 2;
    h = e->h / 2;

    for (y = 0; y <= h; y += h)
    {
        for (x = 0; x <= w; x += w)
        {
            d = new Debris;
            memset(d, 0, sizeof(Debris));
            stage.debrisTail->next = d;
            stage.debrisTail = d;

            d->x = e->x + e->w / 2;
            d->y = e->y + e->h / 2;
            d->dx = (rand() % 5) - (rand() % 5);
            d->dy = -(5 + (rand() % 12));
            d->life = FPS * 2;
            d->texture = e->texture;

            d->rect.x = x;
            d->rect.y = y;
            d->rect.w = w;
            d->rect.h = h;
        }
    }
}
void drawBackground(void)
{
    SDL_Rect dest;
    int x;

    for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(app.renderer, background, NULL, &dest);
    }
}
void drawStarfield(void)
{
    int i, c;

    for (i = 0; i < MAX_STARS; i++)
    {
        c = 32 * stars[i].speed;

        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);

        SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
    }
}
void drawDebris(void)
{
    Debris* d;

    for (d = stage.debrisHead.next; d != NULL; d = d->next)
    {
        blitRect(d->texture, &d->rect, d->x, d->y);
    }
}
void drawExplosions(void)
{
    Explosion* e;

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

    for (e = stage.explosionHead.next; e != NULL; e = e->next)
    {
        SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
        SDL_SetTextureAlphaMod(explosionTexture, e->a);

        blit(explosionTexture, e->x, e->y);
    }

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}