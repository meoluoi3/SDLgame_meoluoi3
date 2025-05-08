#include "SDL.h"
#include "SDL_image.h"
#include "draw.h"
#include "struct.h"
#include "bits/stdc++.h"
#include "text.h"
#include "stage.h"
#include "math.h"

extern App app;
extern Stage stage;
extern Entity* player;
extern SDL_Texture* background;
extern Star stars[MAX_STARS];
extern SDL_Texture* explosionTexture;
extern int backgroundX;

std::vector<SDL_Texture*> playerTextures;
std::vector<SDL_Texture*> enemyTextures;

int playerCurrentFrame = 0;
int enemyCurrentFrame = 0;

int frameSpeed = 10;
int frameTimer = 0;

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
        SDL_Log("blit() Error: Attempted to render a nullptr texture");
        return;
    }
    dest.x = x;
    dest.y = y;

    SDL_QueryTexture(texture, nullptr, nullptr, &dest.w, &dest.h);

    SDL_RenderCopy(app.renderer, texture, nullptr, &dest);
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
void blitRotated(SDL_Texture* texture, int x, int y, double angle)
{
    SDL_Rect dstRect;

    dstRect.x = x;
    dstRect.y = y;
    SDL_QueryTexture(texture, nullptr, nullptr, &dstRect.w, &dstRect.h);
    dstRect.x -= (dstRect.w / 2);
    dstRect.y -= (dstRect.h / 2);

    SDL_RenderCopyEx(app.renderer, texture, nullptr, &dstRect, angle, nullptr, SDL_FLIP_NONE);
}

void drawBullets()
{
    Entity* b;
    for (b = stage.bulletHead.next; b != nullptr; b = b->next)
    {
        blit(b->texture, b->x, b->y);
    }
}
void drawFighters()
{
    Entity* e;
   
    for (e = stage.fighterHead.next; e != nullptr; e = e->next)
    {
        // Draw the entity (player or enemy)
        
        if (e == player) {
            int mouseX, mouseY;
            // Get mouse position
            SDL_GetMouseState(&mouseX, &mouseY);
            player->angle = getAngle(player->x, player->y, mouseX, mouseY);
            blitRotated(player->texture, player->x, player->y, player->angle);
        }
        else if (player != nullptr and e!= player) {
            e->angle = getAngle(e->x, e->y, player->x, player->y);
            blitRotated(e->texture, e->x, e->y, e->angle);
        }
        // Draw the health bar above the entity
        drawHealthBar( app.renderer,e);
        drawHealthText (app.renderer,e);
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
void drawHealthBar(SDL_Renderer* renderer,Entity* entity) {
    int barWidth = 16 * entity->maxHealth;
    int barHeight = 10;

    int healthWidth = (entity->health * barWidth) / entity->maxHealth;

    SDL_Rect healthBar = { entity->x, entity->y - entity->h / 2 - 10, healthWidth, barHeight };

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &healthBar);

    SDL_Rect remainingHealthBar = { entity->x + healthWidth,
                                    entity->y - entity->h / 2 - 10,
                                    barWidth - healthWidth, barHeight };

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &remainingHealthBar);
}
void drawHud(SDL_Renderer* renderer, int score, int highscore) {
    SDL_Color white = { 255, 255, 255 };
    SDL_Color green = { 0, 255, 0 };

    // Draw score at top-left
    renderText(renderer, "SCORE: " + std::to_string(score), 10, 10, white);

    // Draw high score at top-right
    if (score == highscore and score != 0) {
        
        renderText(renderer, "HIGH SCORE: " + std::to_string(highscore), 960, 10, green);
    }
    else {
        renderText(renderer, "HIGH SCORE: " + std::to_string(highscore), 960, 10, white);
    }
}
void drawHealthText(SDL_Renderer* renderer,Entity* entity ) {
    std::string healthText = std::to_string(entity->health) + "/" + std::to_string(entity->maxHealth);
    SDL_Color textColor = { 255, 255, 255 };
    int textX = entity->x ;
    int textY = entity->y - entity->h - 2;
    renderText(renderer, healthText, textX, textY, textColor,0.5);
}

