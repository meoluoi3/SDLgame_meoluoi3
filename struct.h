#pragma once
#include "defs.h"
#include "SDL.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
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
    void (*logic)();
    void (*draw)();

    Delegate() : logic(nullptr), draw(nullptr) {}
};

class App {
public:
    SDL_Renderer* renderer;
    SDL_Window* window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
    int mouseButtons[MAX_MOUSE_BUTTONS];
    int prevKeyboard[MAX_KEYBOARD_KEYS];
    App() : renderer(nullptr), window(nullptr) {
        memset(keyboard, 0, sizeof(keyboard));
        memset(mouseButtons, 0, sizeof(mouseButtons));
        memset(prevKeyboard, 0, sizeof(prevKeyboard));
    }
};

class Entity {
public:
    MovePattern pattern;
    double baseX, baseY, timer; 
    int px, py;
    
    double x, y, dx, dy;
    int w, h, health, maxHealth, reload, side, angle;

    SDL_Texture* texture;

    Entity* next;

    Entity()
        : x(0), y(0), dx(0), dy(0),
        w(0), h(0), health(0), maxHealth(0),
        reload(0), side(0), angle(0),
    pattern(PATTERN_LINEAR), baseX(0), baseY(0), timer(0), 
        texture(nullptr), next(nullptr)
    {}
};
extern Entity* player;
struct Map {
    int tileVisual[MAP_WIDTH][MAP_HEIGHT];
    int data[MAP_WIDTH][MAP_HEIGHT];
};

struct Dungeon {
    SDL_Point renderOffset;
    SDL_Point camera;
    
    Map map;
};

class Stage {
public:
    int highscore;
    int score;
    Entity fighterHead;
    Entity* fighterTail;

    Entity bulletHead;
    Entity* bulletTail;

    Explosion explosionHead;
    Explosion* explosionTail;

    Debris debrisHead;
    Debris* debrisTail;
    
    Dungeon dungeon;
    Stage() : fighterTail(&fighterHead), bulletTail(&bulletHead), explosionTail(&explosionHead), debrisTail(&debrisHead) {}
};




class Star {
public:
    int x, y, speed;

    Star() : x(0), y(0), speed(0) {}
};

struct Settings {
    int sfxVolume;    // 0–100
    int musicVolume;  // 0–100
    int bgIndex;
    int musicIndex;
};

extern Settings settings; 
void logic();
void draw();

