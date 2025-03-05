#include "SDL.h"
#include "SDL_image.h"
#include "draw.h"
#include "struct.h"
extern App app;
extern Stage stage;
extern Entity* player;
void prepareScene(void) // Clear the window and make it all blue
{
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene(void) // Draw everything to the window
{
    SDL_RenderPresent(app.renderer);
}

SDL_Texture* loadTexture(const char* filename) // Load an image from file
{
    SDL_Texture* texture = IMG_LoadTexture(app.renderer, filename);
    if (!texture) {
        SDL_Log("Failed to load texture: %s", IMG_GetError());
    }
    return texture;
}


void blit(SDL_Texture* texture, int x, int y,float scale) //draw an image to the screen (specific coordinate)
{
	SDL_Rect dest;
    if (!texture) {
        SDL_Log("blit() Error: Attempted to render a NULL texture");
        return;
    }
    dest.x = x;
    dest.y = y;
    
    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	dest.w *= scale;
	dest.h *= scale;
    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}
