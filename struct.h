#pragma once
#include "defs.h"
#include "SDL.h"

struct Explosion {
    float x;
    float y;
    float dx;
    float dy;
    int r, g, b, a;
    Explosion* next;
};

struct Debris {
    float x;
    float y;
    float dx;
    float dy;
    SDL_Rect rect;
    SDL_Texture* texture;
    int life;
    Debris* next;
};

struct Delegate {
    void (*logic)(void);
    void (*draw)(void);

    Delegate() : logic(nullptr), draw(nullptr) {}
};

struct App {
    SDL_Renderer* renderer;
    SDL_Window* window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];

    App() : renderer(nullptr), window(nullptr), keyboard{} {}
};

struct Entity {
    float x, y, dx, dy;
    int w, h, health, reload, side;
    SDL_Texture* texture;
    Entity* next;

    Entity() : x(0), y(0), dx(0), dy(0), w(0), h(0), health(1), reload(0), side(0), texture(nullptr), next(nullptr) {}
};

struct Stage {
    Entity fighterHead;
    Entity* fighterTail;
    Entity bulletHead;
    Entity* bulletTail;
    Explosion explosionHead, * explosionTail;
    Debris debrisHead, * debrisTail;
    //Stage() : fighterTail(&fighterHead), bulletTail(&bulletHead) {}
};

struct Star {
    int x;
    int y;
    int speed;
};

void initPlayer();
void initSDL(void);
void initStage(void);
void logic(void);
void draw(void);
void doPlayer(void);
void doBullets(void);
void fireBullet(void);
void drawPlayer(void);

void doFighters(void);

int bulletHitFighter(Entity* b);

void fireAlienBullet(Entity* e);
void clipPlayer(void);
void initStarfield(void);
void doBackground(void);
void doStarfield(void);
void doExplosions(void);
void doDebris(void);
void addExplosions(int x, int y, int num);
void addDebris(Entity* e);

