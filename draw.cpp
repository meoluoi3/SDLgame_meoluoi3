#include "SDL.h"
#include "SDL_image.h"
#include "draw.h"
#include "struct.h"
#include "bits/stdc++.h"
#include "text.h"
#include "stage.h"
#include "math.h"
#include "weapon.h"

extern App app;
extern Stage stage;
extern Entity* player;

extern PlayerWeapons wpnList;


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
SDL_Texture* loadTexture(const char* filename) {
    if (!app.renderer) {
        SDL_Log("loadTexture: Renderer is NULL! Cannot load texture: %s", filename);
        return nullptr;
    }
    SDL_Texture* texture = IMG_LoadTexture(app.renderer, filename);
    if (!texture) {
        SDL_Log("Failed to load texture '%s': %s", filename, IMG_GetError());
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
        
        if (e != nullptr && e->texture != nullptr) {    
            if (e == player) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                player->angle = getAngle(player->x, player->y, mouseX, mouseY);
                blitRotated(player->texture, player->x, player->y, player->angle);
            }
            else {
                if(player != nullptr) e->angle = getAngle(e->x, e->y, player->x, player->y);
                blitRotated(e->texture, e->x, e->y, e->angle);
            }
            // Health bar and text
            drawHealthBar(app.renderer, e, e->x, e->y);
            drawHealthText(app.renderer, e, e->x, e->y);
        }

    }
    
}

void drawHealthBar(SDL_Renderer* renderer, Entity* entity, int screenX, int screenY) {
    int barWidth = 16 * entity->maxHealth;
    int barHeight = 10;

    int healthWidth = (entity->health * barWidth) / entity->maxHealth;

    SDL_Rect healthBar = { screenX, screenY - entity->h / 2 - 10, healthWidth, barHeight };

    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &healthBar);

    SDL_Rect remainingHealthBar = { screenX + healthWidth,
                                    screenY - entity->h / 2 - 10,
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
void drawHealthText(SDL_Renderer* renderer,Entity* entity, int screenX, int screenY) {
    std::string healthText = std::to_string(entity->health) + "/" + std::to_string(entity->maxHealth);
    SDL_Color textColor = { 255, 255, 255 };
    int textX = screenX ;
    int textY = screenY - entity->h - 2;
    renderText(renderer, healthText, textX, textY, textColor,0.5);
}

