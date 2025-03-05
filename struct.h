#pragma once
#include "defs.h"
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
    int w, h, health, reload;
    SDL_Texture* texture;
    Entity* next;

    Entity()
        : x(0), y(0), dx(0), dy(0), w(0), h(0), health(1), reload(0), texture(nullptr), next(nullptr) {
    }
};

struct Stage {
    Entity fighterHead;
    Entity* fighterTail;
    Entity bulletHead;
    Entity* bulletTail;

    Stage() : fighterTail(&fighterHead), bulletTail(&bulletHead) {}
};

void initPlayer();
void initSDL(void);
void initStage(void);

void logic(void);
void draw(void);
void doPlayer(void);
void doBullets(void);
void fireBullet(void);



