#include "Allsource.h"



void Graphics::logErrorAndExit(const char* msg, const char* error)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
		"%s: %s", msg, error);
	SDL_Quit();
}
//Initialize window
void Graphics::init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logErrorAndExit("SDL_Init", SDL_GetError());

    window = SDL_CreateWindow("SDL Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (window == nullptr) logErrorAndExit("CreateWindow", SDL_GetError());

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) logErrorAndExit("SDL_image error:", IMG_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logErrorAndExit("CreateRenderer", SDL_GetError());

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, 800, 600);
}


void Graphics::prepareScene(SDL_Texture* background)
{
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, NULL);
}

void Graphics::presentScene()
{
	SDL_RenderPresent(renderer);
}

SDL_Texture* Graphics::loadTexture(const char* filename)
{
	if (!renderer) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Renderer is not initialized");
		return nullptr;
	}

	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "Loading %s", filename);
	SDL_Texture* texture = IMG_LoadTexture(renderer, filename);
	if (!texture) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", IMG_GetError());
	}
	return texture;
}


void Graphics::quit()
{
	IMG_Quit();
	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Graphics::blit(SDL_Texture* texture, int x, int y)
{
	SDL_Rect dest;

	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}