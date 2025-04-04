#include <SDL.h>
#include <SDL_image.h>
#include "struct.h"
#include <stdio.h>
#include "defs.h"
#include "draw.h"
#include "stage.h"
#include "bits/stdc++.h"
#include "enemy.h"
#include "effect.h"
#include "player.h"
#include "bullet.h"
extern App app;
extern Stage stage;
extern Entity* player;
extern Star stars[MAX_STARS];
SDL_Texture* bulletTexture = NULL;
SDL_Texture* enemyTexture = NULL;
SDL_Texture* alienBullet = NULL;
SDL_Texture* background = NULL;
SDL_Texture* explosionTexture = NULL;
int backgroundX{};
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
    initStarfield();
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
 void logic(void)
 {
     doBackground();
     doStarfield();
     doPlayerMovement();
     doFighters();
     doBullets();
     spawnEnemies();
     doEnemies(); 
     clipPlayer();
     doExplosions();
     doDebris();
    
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





