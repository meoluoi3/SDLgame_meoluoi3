#include <SDL.h>
#include "input.h"
#include "struct.h"
#include "sound.h"
#include "tutorial.h"

extern App app;

void doInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            exit(0);
            break;

        case SDL_KEYDOWN:
            doKeyDown(&event.key);
            break;

        case SDL_KEYUP:
            doKeyUp(&event.key);
            break;
        case SDL_MOUSEBUTTONDOWN:  // Mouse button pressed
            handleMouseDown(&event.button);
            break;

        case SDL_MOUSEBUTTONUP:  // Mouse button released
            handleMouseUp(&event.button);
            break;
        }
    }
}

void doKeyDown(SDL_KeyboardEvent* event)
{
    if (event->keysym.scancode >= 0 &&
        event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[event->keysym.scancode] = 1;
    }
}

void doKeyUp(SDL_KeyboardEvent* event)
{
    if (event->keysym.scancode >= 0 &&
        event->keysym.scancode < MAX_KEYBOARD_KEYS)
    {
        app.keyboard[event->keysym.scancode] = 0;
    }
}

void handleMouseDown(SDL_MouseButtonEvent* button)
{
    if (button->button < MAX_MOUSE_BUTTONS )
    {
       
        app.mouseButtons[button->button] = 1;
    }
}

void handleMouseUp(SDL_MouseButtonEvent* button)
{
    if (button->button < MAX_MOUSE_BUTTONS)
    {
        app.mouseButtons[button->button] = 0;
    }
}

void handleInputEvent(SDL_Event& e) {
    switch (e.type) {
    case SDL_QUIT:
        exit(0);
        break;

    case SDL_KEYDOWN:
        doKeyDown(&e.key);
        
        break;

    case SDL_KEYUP:
        doKeyUp(&e.key);
        break;

    case SDL_MOUSEBUTTONDOWN:
        handleMouseDown(&e.button);
        break;

    case SDL_MOUSEBUTTONUP:
        handleMouseUp(&e.button);
        break;

    default:
        break;
    }
}