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
void resetDungeon() {
    stage.fighterHead.next = nullptr;
    stage.fighterTail = &stage.fighterHead;
    stage.bulletHead.next = nullptr;
    stage.bulletTail = &stage.bulletHead;

    for (int x = 0; x < MAP_WIDTH; ++x) {
        for (int y = 0; y < MAP_HEIGHT; ++y) {
            stage.dungeon.map.data[x][y] = TILE_GROUND;
            stage.dungeon.map.tileVisual[x][y] = 1 + (std::rand() % 10);
        }
    }

    stage.dungeon.camera.x = 0;
    stage.dungeon.camera.y = 0;
    stage.dungeon.renderOffset.x = (SCREEN_WIDTH - (MAP_RENDER_WIDTH * TILE_SIZE)) / 2;
    stage.dungeon.renderOffset.y = (SCREEN_HEIGHT - (MAP_RENDER_HEIGHT * TILE_SIZE)) / 2;

    cameraghost.x = 0;
    cameraghost.y = 0;
    cameraghost.smoothSpeed = 0.2f;
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
                int px = x * TILE_SIZE - stage.dungeon.camera.x;
                int py = y * TILE_SIZE - stage.dungeon.camera.y;
                blit(tiles[t], px, py);
            }
        }
    }
}

extern int frameCount;
void updateCamera() {
    if (!player) return;

    // 1) Build look‑ahead target
    double tx = player->x + ((player->dx || player->dy) ? player->dx * 2.0 : 0.0);
    double ty = player->y + ((player->dx || player->dy) ? player->dy * 2.0 : 0.0);

    // 2) Smooth the ghost toward that target
    cameraghost.x += (tx - cameraghost.x) * cameraghost.smoothSpeed;
    cameraghost.y += (ty - cameraghost.y) * cameraghost.smoothSpeed;

    // 3) Clamp the GHOST so centering never shows outside the world
    const int halfW = SCREEN_WIDTH / 2 - player->w / 2;
    const int halfH = SCREEN_HEIGHT / 2 - player->h / 2;
    const int worldW = MAP_WIDTH * TILE_SIZE;
    const int worldH = MAP_HEIGHT * TILE_SIZE;

    cameraghost.x = clamp(cameraghost.x, double(halfW), double(worldW - halfW));
    cameraghost.y = clamp(cameraghost.y, double(halfH), double(worldH - halfH));

    // 4) Center the camera on the ghost — no final clamp here!
    stage.dungeon.camera.x = int(cameraghost.x) - SCREEN_WIDTH / 2 + player->w / 2;
    stage.dungeon.camera.y = int(cameraghost.y) - SCREEN_HEIGHT / 2 + player->h / 2;
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
    drawFightersDungeon();
    drawBulletDungeon();
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
void drawBulletDungeon() {
    Entity* bullet = stage.bulletHead.next;
    while (bullet) {
        // In Dungeon mode, subtract camera position to get the correct on-screen position
        int renderX = bullet->x - stage.dungeon.camera.x;
        int renderY = bullet->y - stage.dungeon.camera.y;

        // Draw the bullet at the calculated position
        SDL_Rect dstRect = { renderX, renderY, bullet->w, bullet->h };
        SDL_RenderCopy(app.renderer, bullet->texture, nullptr, &dstRect);

        bullet = bullet->next;
    }
}

void drawFightersDungeon() {
    for (Entity* e = stage.fighterHead.next; e != nullptr; e = e->next) {
        std::cerr << "using drawFightersDungeon\n";
        // Convert world (pixel) coords into screen coords:
        int sx = int(e->x) - stage.dungeon.camera.x - e->w / 2;
        int sy = int(e->y) - stage.dungeon.camera.y - e->h / 2;

        int screenX = int(e->x) - stage.dungeon.camera.x;
        int screenY = int(e->y) - stage.dungeon.camera.y;

        e->screenX = screenX;
        e->screenY = screenY;

        // Rotate & blit
        if (e == player) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Correct angle calculation: Use world coordinates (e->x, e->y) and mouse position
            player->angle = getAngle(e->x, e->y, mouseX + stage.dungeon.camera.x, mouseY + stage.dungeon.camera.y);
            blitRotated(player->texture, sx, sy, player->angle);
        }
        else {
            // For other entities, you can still use their world coordinates for angle calculation
            e->angle = getAngle(e->x, e->y, screenX, screenY);
            blitRotated(e->texture, sx, sy, e->angle);
        }

        // Draw health bars/text
        drawHealthBar(app.renderer, e, screenX, screenY);
        drawHealthText(app.renderer, e, screenX, screenY);
    }
}



void initCameraGhost() {
    if (player) {
        cameraghost.x = player->x;
        cameraghost.y = player->y;
    }
    else {
        cameraghost.x = 0;
        cameraghost.y = 0;
    }
    cameraghost.smoothSpeed = 0.2f;      // tweak for your desired feel

    // zero any lingering offsets
    stage.dungeon.renderOffset.x = 0;
    stage.dungeon.renderOffset.y = 0;

    // instantly snap camera into place
    updateCamera();
}