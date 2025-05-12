#include "SDL.h"
#include "SDL_image.h"
#include "stdio.h"
#include "defs.h"
#include "stage.h"
#include "bits/stdc++.h"
#include "struct.h"
#include "sound.h"
#include "init.h"
#include "dungeon.h"


using namespace std;

extern App app;
extern Stage stage;
extern Entity* player;
extern int enemySpawnTimer;
extern int stageResetTimer;
extern int backgroundX;



void resetStage(void)
{
    // Free all entities
    Entity* e;
    Explosion* ex;
    Debris* d;

    while ((e = stage.fighterHead.next))
    {
        stage.fighterHead.next = e->next;
        delete e;
    }

    while ((e = stage.bulletHead.next))
    {
        stage.bulletHead.next = e->next;
        delete e;
    }

    while ((ex = stage.explosionHead.next))
    {
        stage.explosionHead.next = ex->next;
        delete ex;
    }

    while ((d = stage.debrisHead.next))
    {
        stage.debrisHead.next = d->next;
        delete d;
    }

    // Reset pointers
    stage.fighterHead.next = nullptr;
    stage.fighterTail = &stage.fighterHead;

    stage.bulletHead.next = nullptr;
    stage.bulletTail = &stage.bulletHead;

    stage.explosionTail = &stage.explosionHead;
    stage.debrisTail = &stage.debrisHead;

    initPlayer();
    //initMap();
    initStarfield();
   // resetDungeon();
    enemySpawnTimer = 0;
    stageResetTimer = FPS * 3;
    
    stage.score = 0;
    

}



