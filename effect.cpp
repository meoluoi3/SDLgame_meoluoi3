#include "effect.h"
#include "struct.h"
#include "bits/stdc++.h"
extern App app;
extern Stage stage;
extern Entity* player;
extern Star stars[MAX_STARS]; 
extern int backgroundX;


void doBackground(void)
{
    
    if (--backgroundX < -SCREEN_WIDTH)
    {
        backgroundX = 0;
    }
}
void doStarfield(void)
{
    int i;

    for (i = 0; i < MAX_STARS; i++)
    {
        stars[i].x -= stars[i].speed;

        if (stars[i].x < 0)
        {
            stars[i].x = SCREEN_WIDTH + stars[i].x;
        }
    }
}
void doExplosions(void)
{

    Explosion* e, * prev;

    prev = &stage.explosionHead;

    for (e = stage.explosionHead.next; e != NULL; e = e->next)
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
    printf("Explosion added\n");
    Explosion* e;
    int i;

    for (i = 0; i < num; i++)
    {
        e = new Explosion();
        stage.explosionTail->next = e;
        stage.explosionTail = e;

        e->x = x + (rand() % 32)+30;// -(rand() % 32);
        e->y = y + (rand() % 32)+30;// -(rand() % 32);
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
void doDebris(void)
{
    Debris* d, * prev;

    prev = &stage.debrisHead;

    for (d = stage.debrisHead.next; d != NULL; d = d->next)
    {
        d->x += d->dx;
        d->y += d->dy;

        
        d->life--;
        
        

        d->rect.w --;
        d->rect.h --;
        if (d->life <= 0 or (d->rect.w == 0 and d->rect.h == 0))
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
    Debris* d;
    int x, y, w, h;

    w = e->w / (rand() % 5 + 1);
    h = e->h / (rand() % 5 + 1);

    for (y = 0; y < e->h; y += h)
    {
        for (x = 0; x < e->w; x += w)
        {
            d = new Debris();
            stage.debrisTail->next = d;
            stage.debrisTail = d;

            w = rand() % (e->w / 5) + 1;
            h = rand() % (e->h / 5) + 1;

            d->x = e->x + e->w / 2;
            d->y = e->y + e->h / 2;

            d->dx = (rand() % 20) - (rand() % 20); // small horizontal movement
            d->dy = (rand() % 20) - (rand() % 20);        // flies upward

            d->life = FPS * 3;
            d->texture = e->texture;

            d->rect.x = x;
            d->rect.y = y;
            d->rect.w = w;
            d->rect.h = h;
        }
    }
}
    