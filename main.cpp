#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "struct.h"
#include "input.h"
#include "draw.h"
#include "defs.h"

App app;
Entity player;
Entity bullet;

int main(int argc, char* argv[])
{
    initSDL();

    player.x = 100;
    player.y = 100;
    player.texture = loadTexture("img/4.png", app.renderer);

	bullet.texture = loadTexture("img/bullet_klee.png", app.renderer);
    while (true)
    {
        prepareScene();

        doInput();

        player.x += player.dx;
        player.y += player.dy;
        if (app.up)
        {
            player.y -= 10;
        }

        if (app.down)
        {
            player.y += 10;
        }

        if (app.left)
        {
            player.x -= 10;
        }

        if (app.right)
        {
            player.x += 10;
        }

        if (app.fire && bullet.health == 0)
        {
            bullet.x = player.x+125;
            bullet.y = player.y+75;
            bullet.dx = 16;
            bullet.dy = 0;
            bullet.health = 1;
        }
        
        bullet.x += bullet.dx;
        bullet.y += bullet.dy;

        if (bullet.x > SCREEN_WIDTH)
        {
            bullet.health = 0;
        }

        blit(player.texture, player.x, player.y,1.0);
        
        if (player.x < 0) player.x = 0;
        if (player.y < 0) player.y = 0;
        if (player.x > SCREEN_WIDTH - 100) player.x = SCREEN_WIDTH - 100;
        if (player.y > SCREEN_HEIGHT - 100) player.y = SCREEN_HEIGHT - 100;

        if (bullet.health > 0)
        {
            blit(bullet.texture, bullet.x, bullet.y, 0.1);
        }

       
        presentScene();
        
        SDL_Delay(16);
    }


	

    // Cleanup before exiting
    SDL_DestroyTexture(player.texture);
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
