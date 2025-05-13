#include "dungeon.h"
#include "struct.h"
#include "init.h"
#include "draw.h"
#include "effect.h"
#include "player.h"
#include "weapon.h"
#include "bullet.h"
#include "enemy.h"
#include "stage.h"
#include <SDL.h>
#include <SDL_image.h>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <iostream>
#include "math.h"



extern App app;
extern Stage stage;
extern int stageResetTimer;
extern std::vector<SDL_Texture*> tiles;

void initMap(void)
{
    int x, y;

    loadTiles();

    for (x = 0; x < MAP_WIDTH; x++)
    {
        for (y = 0; y < MAP_HEIGHT; y++)
        {
            stage.dungeon.map.data[x][y] = TILE_GROUND;
        }
    }
}

void loadTiles() {
    tiles.clear();
    tiles.resize(48, nullptr);

    // Load ground tiles 1-10
    for (int i = 1; i <= 10; ++i) {
        std::ostringstream ss;
        ss << "kenney_top-down-shooter/PNG/Tiles/tile_"
            << std::setw(2) << std::setfill('0') << i << ".png";
        tiles[i] = loadTexture(ss.str().c_str());
        if (!tiles[i]) SDL_Log("Failed to load %s", ss.str().c_str());
    }
    // Load wall tiles 42-47
    for (int i = 42; i <= 47; ++i) {
        std::ostringstream ss;
        ss << "kenney_top-down-shooter/PNG/Tiles/tile_"
            << std::setw(2) << std::setfill('0') << i << ".png";
        tiles[i] = loadTexture(ss.str().c_str());
        if (!tiles[i]) SDL_Log("Failed to load %s", ss.str().c_str());
    }
}

void drawMapTiles(void)
{
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int t = stage.dungeon.map.data[x][y];
            if (t >= 0 && t < (int)tiles.size() && tiles[t]) {
                int px = x * TILE_SIZE - stage.dungeon.camera.x + stage.dungeon.renderOffset.x;
                int py = y * TILE_SIZE - stage.dungeon.camera.y + stage.dungeon.renderOffset.y;
                blit(tiles[t], px, py);
            }
        }
    }
}


void updateCamera() {
    // Desired camera origin so player is centered
    int cx = player->x - SCREEN_WIDTH / 2 + player->w / 2;
    int cy = player->y - SCREEN_HEIGHT / 2 + player->h / 2;

    // Clamp to [0, worldWidth−screenWidth]
    int maxCX = MAP_WIDTH * TILE_SIZE - SCREEN_WIDTH;
    int maxCY = MAP_HEIGHT * TILE_SIZE - SCREEN_HEIGHT;

    stage.dungeon.camera.x = clamp(cx, 0, maxCX);
    stage.dungeon.camera.y = clamp(cy, 0, maxCY);
}


bool loadMapFromCSV(const std::string& filename, Map& map) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        SDL_Log("Failed to open map file: %s", filename.c_str());
        return false;
    }

    std::string line;
    int row = 0;
    std::srand((unsigned)std::time(nullptr));
    while (std::getline(file, line) && row < MAP_HEIGHT) {
        std::stringstream ss(line);
        std::string cell;
        int col = 0;
        while (std::getline(ss, cell, ',') && col < MAP_WIDTH) {
            int tileType = std::stoi(cell);
            map.data[col][row] = tileType;
            if (tileType == TILE_GROUND)
                map.tileVisual[col][row] = 1 + std::rand() % 10;
            else if (tileType == TILE_WALL)
                map.tileVisual[col][row] = 42 + std::rand() % 6;
            else
                map.tileVisual[col][row] = 0;
            ++col;
        }
        ++row;
    }
    file.close();
    return true;
}

void logicDungeon() {
    doPlayerMovement(wpnList);
    doFighters();
    doBullets();
    doEnemies();
    updateWeaponReloads(wpnList, SDL_GetTicks());
    doExplosions();
    doDebris();

    // Update camera to follow the player
    if (player) {
        updateCamera();
    }

    if (stage.score > stage.highscore) {
        stage.highscore = stage.score;
    }

    if (!player && --stageResetTimer <= 0) {
        SDL_Log("Resetting stage...");
        resetStage();
    }
}


void drawDungeon() {
    // Draw map
    drawMapTiles();

    // Draw all fighters (player, enemies, etc.)
    drawFighters();
    drawBullets();
    drawDebris();
    drawExplosions();

    // Draw the player and weapon texture
    playerAndWeaponTexture(wpnList);

    // Draw HUD elements
    drawHud(app.renderer, stage.score, stage.highscore);

    // Draw weapon-specific HUD (ammo)
    int idx = wpnList.currentIndex;
    if (idx >= 0 && idx < (int)wpnList.list.size()) {
        drawAmmoHUD(app.renderer, wpnList.list[idx]);
    }
}


void drawFightersDungeon(void)
{
    Entity* e;

    for (e = stage.fighterHead.next; e != nullptr; e = e->next)
    {
        // Calculate the position based on the tile size and render offset
        int x = (e->x * TILE_SIZE) + (TILE_SIZE / 2);
        int y = (e->y * TILE_SIZE) + (TILE_SIZE / 2);

        // Apply the render offset
        x += stage.dungeon.renderOffset.x;
        y += stage.dungeon.renderOffset.y;
        
        x += -stage.dungeon.camera.x;
        y += -stage.dungeon.camera.y;

        // If it's the player, rotate based on the mouse position
        if (e == player) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            player->angle = getAngle(player->x, player->y, mouseX, mouseY);
            blitRotated(player->texture, x, y, player->angle);
        }
        else {
            // If it's an enemy, rotate based on the player's position
            if (player != nullptr) e->angle = getAngle(e->x, e->y, player->x, player->y);
            blitRotated(e->texture, x, y, e->angle);
        }

        // Draw health bar and text (for both player and enemies)
        drawHealthBar(app.renderer, e);
        drawHealthText(app.renderer, e);
    }
}
