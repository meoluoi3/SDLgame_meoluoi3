#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
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
#include "sound.h"
#include <SDL_ttf.h>
#include "text.h"
#include "SDL_video.h"
extern App app;
extern Stage stage;
extern Entity* player;
extern Star stars[MAX_STARS];

SDL_Texture* bulletTexture = nullptr;
SDL_Texture* enemyTexture = nullptr;
SDL_Texture* alienBullet = nullptr;
SDL_Texture* background = nullptr;
SDL_Texture* explosionTexture = nullptr;
SDL_Texture* fontTexture = nullptr;
int backgroundX{};
int stageResetTimer{};





void initSDL() // Initialize SDL
{

    int rendererFlags, windowFlags;

    rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
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

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
    {
        SDL_Log("Couldn't initialize SDL Mixer\n");
        exit(1);
    }

    if (TTF_Init() == -1) {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf Error: ", TTF_GetError());
    }

    initFont("font/HYWenHei_Extended.ttf", 28);
    Mix_AllocateChannels(MAX_SND_CHANNELS);
    SDL_ShowCursor(1);
}

void initStage() // Initialize the stage
{
    app.delegate.logic = logic;
    app.delegate.draw = draw;

    stage.fighterHead.next = nullptr;
    stage.bulletHead.next = nullptr;
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    stage.explosionHead.next = nullptr;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    //initPlayer(); // already initialize player in resetStage
    initStarfield();
    resetStage();
    initSounds();
    loadMusic("sound/backgroundsound.mp3");
    bulletTexture = loadTexture("img/bullet_klee.png");
    enemyTexture = loadTexture("img/enemy.png");
    alienBullet = loadTexture("img/alienBullet.png");
    background = loadTexture("img/background.png");
    explosionTexture = loadTexture("img/explosion.png");
    //resetStage();
}

void initPlayer()
{
    player = new Entity();
    player->next = nullptr;  // Ensure proper termination

    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x = SCREEN_WIDTH/2 + rand() % 20 - rand() % 20;
    player->y = SCREEN_HEIGHT/2 + rand() % 20 - rand() % 20;
    player->side = SIDE_PLAYER;
    player->health = 3;
    player->maxHealth = 3;
    player->texture = loadTexture("img/survivor1_stand.png");
    SDL_QueryTexture(player->texture, nullptr, nullptr, &player->w, &player->h);
}

void initStarfield()
{
    int i;

    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x = rand() % SCREEN_WIDTH;
        stars[i].y = rand() % SCREEN_HEIGHT;
        stars[i].speed = 1 + rand() % 8;
    }
}

void logic()
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
    if (stage.score > stage.highscore) {
        stage.highscore = stage.score;
    }
    if (player == nullptr && --stageResetTimer <= 0)
    {
        SDL_Log("Calling resetStage()...\n");
        resetStage();
    }
}

void draw()
{
    drawBackground();
    drawStarfield();
    drawFighters();
    drawBullets();
    drawDebris();
    drawExplosions();
    drawHud(app.renderer, stage.score, stage.highscore);
    
}
