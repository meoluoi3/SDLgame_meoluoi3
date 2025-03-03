
#include "Allsource.h"


int main(int argc, char* args[])
{

	app.init();
	
	player.x = 200;
	player.y = 200;
	player.texture = app.loadTexture("img/4.png");

	Entities bullet;
	bullet.texture = app.loadTexture("img/bullet_klee.png");
	bool running = true;
	while (running)
	{
		SDL_SetRenderDrawColor(app.renderer, 96, 128, 255, 255);
		SDL_RenderClear(app.renderer);
		doInput();
		if (app.up)
		{
			player.y -= 4;
		}

		if (app.down)
		{
			player.y += 4;
		}

		if (app.left)
		{
			player.x -= 4;
		}

		if (app.right)
		{
			player.x += 4;
		}

		if (player.x < 0)
		{
			player.x = 0;
		}
		if (player.y < 0) {
			player.y = 0;
		}
		if (player.x > SCREEN_WIDTH - 64)
		{
			player.x = SCREEN_WIDTH - 64;
		}	
		if (player.y > SCREEN_HEIGHT - 64)
		{
			player.y = SCREEN_HEIGHT - 64;
		}


		app.blit(player.texture, player.x, player.y);
		app.presentScene();
		SDL_Delay(16);
	}

	app.quit();
	return 0;
}