#ifndef INPUT_H
#define INPUT_H
#include <SDL.h>
#include "struct.h"

void waitUntilQuitClicked();
void doInput(void);
void doKeyDown(SDL_KeyboardEvent* event);
void doKeyUp(SDL_KeyboardEvent* event);
#endif 