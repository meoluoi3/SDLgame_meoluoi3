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

extern App app;
extern Stage stage;
extern Entity* player;
extern Star stars[MAX_STARS];
extern int stageResetTimer;

void logic()
{
  
    doBackground();
    doStarfield();

    doPlayerMovement();
    doFighters();
    doBullets();

    spawnEnemies();
    doEnemies();

    clipPlayer();
    doExplosions();
    doDebris();
    if (stage.score > stage.highscore) {
        stage.highscore = stage.score;
    }
    if (player == nullptr && --stageResetTimer <= 0)
    {
        SDL_Log("Calling resetStage()...\n");
        resetStage();
    }
}

void draw()
{
    drawBackground();
    drawStarfield();
    drawFighters();
    drawBullets();
    drawDebris();
    drawExplosions();
    drawHud(app.renderer, stage.score, stage.highscore);
    
}
