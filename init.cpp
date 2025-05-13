#include "init.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "struct.h"
#include "stage.h"
#include "sound.h"
#include "draw.h"
#include "text.h"
#include "settings.h"
#include "weapon.h"
#include "dungeon.h"
extern App app;
extern Stage stage;
extern Entity* player;
extern Star stars[MAX_STARS];
extern PlayerWeapons wpnList;
SDL_Texture* bulletTexture = nullptr;
SDL_Texture* enemyTexture = nullptr;
SDL_Texture* alienBullet = nullptr;
SDL_Texture* background = nullptr;
SDL_Texture* explosionTexture = nullptr;
SDL_Texture* fontTexture = nullptr;
 SDL_Texture* playerStanding;
 SDL_Texture* playerHoldingAK;
 SDL_Texture* playerHoldingPistol;
 SDL_Texture* playerPunching;
 SDL_Texture* debrisTexture;
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

    //initMap();            
    //initEntities();       
    //initPlayer();         
   // initStarfield();
    resetStage();
    initSounds();
    initWeapons(wpnList);
    bulletTexture = loadTexture("img/bullet_klee.png");
    enemyTexture = loadTexture("kenney_top-down-shooter/PNG/Zombie 1/zoimbie1_gun.png");
    alienBullet = loadTexture("img/alienBullet.png");
    explosionTexture = loadTexture("img/explosion.png");
    debrisTexture = loadTexture("img/debris.png");  
    
    applySettings(); // loads background and music
}
void initSurvivor() {
    stage.fighterHead.next = nullptr;
    stage.bulletHead.next = nullptr;
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    stage.explosionHead.next = nullptr;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    initPlayer();
    initStarfield();

    app.delegate.logic = logic;
    app.delegate.draw = draw;
}

void initDungeon()
{
    // 1) Reset all your entity lists exactly as before
    stage.fighterHead.next = nullptr;
    stage.bulletHead.next = nullptr;
    stage.fighterTail = &stage.fighterHead;
    stage.bulletTail = &stage.bulletHead;

    stage.explosionHead.next = nullptr;
    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    // 2) Spawn the player
    initPlayer();

    // ────────────────────────────────────────────────────────────
    // 3) Load *and* initialize your map here
    initMap();  // ← calls loadTiles() and fills map.data[][] with TILE_GROUND
    // ────────────────────────────────────────────────────────────

    // 4) Center your view in the middle of the map
    stage.dungeon.renderOffset.x = (SCREEN_WIDTH - (MAP_RENDER_WIDTH * TILE_SIZE)) / 2;
    stage.dungeon.renderOffset.y = (SCREEN_HEIGHT - (MAP_RENDER_HEIGHT * TILE_SIZE)) / 2;

    // 5) Start your camera at the top‐left of the world
    stage.dungeon.camera = { 0, 0 };

    // 6) Hook up dungeon logic/draw
    app.delegate.logic = logicDungeon;
    app.delegate.draw = drawDungeon;
}


void initPlayer()
{
    player = new Entity();
    player->next = nullptr;  // Ensure proper termination

    stage.fighterTail->next = player;
    stage.fighterTail = player;

    player->x = SCREEN_WIDTH / 2 + rand() % 20 - rand() % 20;
    player->y = SCREEN_HEIGHT / 2 + rand() % 20 - rand() % 20;
    player->side = SIDE_PLAYER;
    player->health = 3;
    player->maxHealth = 3;
    player->texture = loadTexture("kenney_top-down-shooter/PNG/Survivor 1/survivor1_machine.png");
    playerStanding = loadTexture("kenney_top-down-shooter/PNG/Survivor 1/survivor1_stand.png");
    playerHoldingAK = loadTexture("kenney_top-down-shooter/PNG/Survivor 1/survivor1_machine.png");
    playerHoldingPistol = loadTexture("kenney_top-down-shooter/PNG/Survivor 1/survivor1_gun.png");
    playerPunching = loadTexture("kenney_top-down-shooter/PNG/Survivor 1/survivor1_hold.png");
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
