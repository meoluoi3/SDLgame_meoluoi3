#include "SDL.h"
#include "SDL_image.h"
#include "draw.h"
#include "struct.h"
#include "bits/stdc++.h"

extern App app;
extern Stage stage;
extern Entity* player;
extern SDL_Texture* background;
extern Star stars[MAX_STARS];
extern SDL_Texture* explosionTexture;
extern int backgroundX;

void prepareScene()
{
    SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
    SDL_RenderClear(app.renderer);
}

void presentScene()
{
    SDL_RenderPresent(app.renderer);
}

SDL_Texture* loadTexture(const char* filename)
{
    SDL_Texture* texture = IMG_LoadTexture(app.renderer, filename);
    if (!texture) {
        SDL_Log("Failed to load texture: %s", IMG_GetError());
    }
    return texture;
}

void blit(SDL_Texture* texture, int x, int y)
{
    SDL_Rect dest;
    if (!texture) {
        SDL_Log("blit() Error: Attempted to render a NULL texture");
        return;
    }
    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

    SDL_RenderCopy(app.renderer, texture, NULL, &dest);
}

void blitRect(SDL_Texture* texture, SDL_Rect* src, int x, int y)
{
    SDL_Rect dest;

    dest.x = x;
    dest.y = y;
    dest.w = src->w;
    dest.h = src->h;

    SDL_RenderCopy(app.renderer, texture, src, &dest);
}

void drawPlayer()
{
    blit(player->texture, player->x, player->y);
}

void drawBullets()
{
    Entity* b;
    for (b = stage.bulletHead.next; b != NULL; b = b->next)
    {
        blit(b->texture, b->x, b->y);
    }
}

void drawFighters()
{
    Entity* e;
    for (e = stage.fighterHead.next; e != NULL; e = e->next)
    {
        blit(e->texture, e->x, e->y);
    }
}

void drawBackground()
{
    SDL_Rect dest;
    int x;

    for (x = backgroundX; x < SCREEN_WIDTH; x += SCREEN_WIDTH)
    {
        dest.x = x;
        dest.y = 0;
        dest.w = SCREEN_WIDTH;
        dest.h = SCREEN_HEIGHT;

        SDL_RenderCopy(app.renderer, background, NULL, &dest);
    }
}

void drawStarfield()
{
    int i, c;

    for (i = 0; i < MAX_STARS; i++)
    {
        c = 32 * stars[i].speed;

        SDL_SetRenderDrawColor(app.renderer, c, c, c, 255);
        SDL_RenderDrawLine(app.renderer, stars[i].x, stars[i].y, stars[i].x + 3, stars[i].y);
    }
}

void drawDebris()
{
    Debris* d;

    for (d = stage.debrisHead.next; d != NULL; d = d->next)
    {
        blitRect(d->texture, &d->rect, d->x, d->y);
    }
}

void drawExplosions()
{
    Explosion* e;

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

    for (e = stage.explosionHead.next; e != NULL; e = e->next)
    {
        SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
        SDL_SetTextureAlphaMod(explosionTexture, e->a);

        blit(explosionTexture, e->x, e->y);
    }

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
}
