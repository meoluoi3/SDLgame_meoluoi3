#pragma once
#include "defs.h"

struct App {
    SDL_Renderer* renderer;
    SDL_Window* window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
};

struct Entity {
    float x;
    float y;
    int w;
    int h;
    float dx;
    float dy;
    int health;
    int reload;
    SDL_Texture* texture;
    Entity* next;
};

struct Delegate{
    void (*logic)(void);
    void (*draw)(void);
};

struct Stage {
    Entity fighterHead, * fighterTail;
    Entity bulletHead, * bulletTail;
};

void initSDL(void);
void initStage(void);
static void initPlayer();
static void logic(void);
static void draw(void);
static void doPlayer(void);
static void doBullets(void);
static void fireBullet(void);



