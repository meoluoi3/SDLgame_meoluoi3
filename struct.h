#pragma once
#include "defs.h"
#include "SDL.h"
#include "bits/stdc++.h"

class Explosion {
public:
    float x, y, dx, dy;
    int r, g, b, a;
    Explosion* next;

    Explosion() : x(0), y(0), dx(0), dy(0), r(0), g(0), b(0), a(0), next(nullptr) {}
};

class Debris {
public:
    float x, y, dx, dy;
    SDL_Rect rect;
    SDL_Texture* texture;
    int life;
    Debris* next;


    Debris() : x(0), y(0), dx(0), dy(0), texture(nullptr), life(0), next(nullptr) {
        rect = { 0, 0, 0, 0 };
    }
};

class Delegate {
public:
    void (*logic)(void);
    void (*draw)(void);

    Delegate() : logic(nullptr), draw(nullptr) {}
};

class App {
public:
    SDL_Renderer* renderer;
    SDL_Window* window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];

    App() : renderer(nullptr), window(nullptr) {
        memset(keyboard, 0, sizeof(keyboard));
    }
};

class Entity {
public:
    float x, y, dx, dy;
    int w, h, health, reload, side;
    SDL_Texture* texture;
    Entity* next;

    Entity() : x(0), y(0), dx(0), dy(0), w(0), h(0), health(0), reload(0), side(0), texture(nullptr), next(nullptr) {}
};

class Stage {
public:
    Entity fighterHead;
    Entity* fighterTail;
    Entity bulletHead;
    Entity* bulletTail;
    Explosion explosionHead;
    Explosion* explosionTail;
    Debris debrisHead;
    Debris* debrisTail;

    Stage() : fighterTail(&fighterHead), bulletTail(&bulletHead), explosionTail(&explosionHead), debrisTail(&debrisHead) {}
};

class Star {
public:
    int x, y, speed;

    Star() : x(0), y(0), speed(0) {}
};


void initPlayer();
void initSDL(void);
void initStage(void);
void logic(void);
void draw(void);
void clipPlayer(void);
void initStarfield(void);

