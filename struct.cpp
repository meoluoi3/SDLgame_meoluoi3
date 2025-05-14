#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "struct.h"
#include <stdio.h>
#include "defs.h"
#include "draw.h"
#include "stage.h"
#include "bits/stdc++.h"
#include "enemy.h"
#include "effect.h"
#include "player.h"
#include "bullet.h"
#include "sound.h"
#include <SDL_ttf.h>
#include "text.h"
#include "SDL_video.h"
#include "math.h"
#include "weapon.h"

extern App app;
extern Stage stage;
extern Entity* player;
extern Star stars[MAX_STARS];
extern int stageResetTimer;
extern PlayerWeapons wpnList;

void logicSurvivor()
{
   
    doBackground();
    doStarfield();

    doPlayerMovement(wpnList);
    doFighters();
    doBullets();
   
   
    
    spawnEnemies();
    doEnemies();
    updateWeaponReloads(wpnList, SDL_GetTicks());
    

    clipPlayer();
    doExplosions();
    doDebris();

    if (stage.score > stage.highscore) {
        stage.highscore = stage.score;
    }

    if (player == nullptr && --stageResetTimer <= 0) {
        SDL_Log("Calling resetStage()...\n");
        resetStage();
    }
}

void drawSurvivor()
{
    drawBackground();
    drawStarfield();
    drawFighters();
    drawBullets();
    drawDebris();
    drawExplosions();

    // Safe weapon texture assignment
    playerAndWeaponTexture(wpnList);
    drawReloadText(app.renderer, wpnList);
    drawHud(app.renderer, stage.score, stage.highscore);

    // Safe ammo HUD
    int idx = wpnList.currentIndex;
    if (idx >= 0 && idx < (int)wpnList.list.size()) {
        drawAmmoHUD(app.renderer, wpnList.list[idx]);
    }
}
