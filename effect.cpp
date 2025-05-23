#include "effect.h"
#include "struct.h"
#include "bits/stdc++.h"
#include "draw.h"

extern App app;
extern Stage stage;
extern Entity* player;
extern Star stars[MAX_STARS];
extern int backgroundX;
extern SDL_Texture* debrisTexture;

void doBackground()
{
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}

void doStarfield()
{
    for (int i = 0; i < MAX_STARS; i++)
    {
        stars[i].x -= stars[i].speed;

        if (stars[i].x < 0)
        {
               stars[i].x = SCREEN_WIDTH + stars[i].x;
        }
    }
}

void doExplosions()
{
    Explosion* e;
    Explosion* prev = &stage.explosionHead;

    for (e = stage.explosionHead.next; e != nullptr; e = e->next)
    {
        e->x += e->dx;
        e->y += e->dy;

        if (--e->a <= 0)
        {
            if (e == stage.explosionTail)
            {
                stage.explosionTail = prev;
            }

            prev->next = e->next;
            delete e;
            e = prev;
        }

        prev = e;
    }
}

void addExplosions(int x, int y, int num)
{
    SDL_Log("Explosion added\n");

    for (int i = 0; i < num; i++)
    {
        Explosion* e = new Explosion();

        stage.explosionTail->next = e;
        stage.explosionTail = e;

        e->x = x + (rand() % 32) + 30;
        e->y = y + (rand() % 32) + 30;
        e->dx = (rand() % 10) - (rand() % 10);
        e->dy = (rand() % 10) - (rand() % 10);

        e->dx /= 10;
        e->dy /= 10;

        switch (rand() % 4)
        {
        case 0:
            e->r = 255;
            break;
        case 1:
            e->r = 255;
            e->g = 128;
            break;
        case 2:
            e->r = 255;
            e->g = 255;
            break;
        default:
            e->r = 255;
            e->g = 255;
            e->b = 255;
            break;
        }

        e->a = rand() % FPS * 3;
    }
}

void doDebris()
{
    Debris* d;
    Debris* prev = &stage.debrisHead;

    for (d = stage.debrisHead.next; d != nullptr; d = d->next)
    {
        d->x += d->dx;
        d->y += d->dy;

        d->life--;

        d->rect.w--;
        d->rect.h--;

        if (d->life <= 0 || (d->rect.w == 0 && d->rect.h == 0))
        {
            if (d == stage.debrisTail)
            {
                stage.debrisTail = prev;
            }

            prev->next = d->next;
            delete d;
            d = prev;
        }

        prev = d;
    }
}

void addDebris(Entity* e)
{
    int w = e->w / (rand() % 5 + 1);
    int h = e->h / (rand() % 5 + 1);

    for (int y = 0; y < e->h; y += h)
    {
        for (int x = 0; x < e->w; x += w)
        {
            Debris* d = new Debris();
            stage.debrisTail->next = d;
            stage.debrisTail = d;

            w = rand() % (e->w / 4) + 1;
            h = rand() % (e->h / 4) + 1;

            d->x = e->x + e->w / 2;
            d->y = e->y + e->h / 2;

            d->dx = (rand() % 20) - (rand() % 20);
            d->dy = (rand() % 20) - (rand() % 20);

            d->life = FPS * 3;
            d->texture = debrisTexture  ;

            d->rect.x = x;
            d->rect.y = y;
            d->rect.w = w;
            d->rect.h = h;
        }
    }
}


void drawDebris()
{
    Debris* d;

    for (d = stage.debrisHead.next; d != nullptr; d = d->next)
    {
        blitRect(d->texture, &d->rect, d->x, d->y);
    }
}
void drawExplosions()
{
    Explosion* e;

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_ADD);
    SDL_SetTextureBlendMode(explosionTexture, SDL_BLENDMODE_ADD);

    for (e = stage.explosionHead.next; e != nullptr; e = e->next)
    {
        SDL_SetTextureColorMod(explosionTexture, e->r, e->g, e->b);
        SDL_SetTextureAlphaMod(explosionTexture, e->a);

        blit(explosionTexture, e->x, e->y);
    }

    SDL_SetRenderDrawBlendMode(app.renderer, SDL_BLENDMODE_NONE);
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

        SDL_RenderCopy(app.renderer, background, nullptr, &dest);
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
