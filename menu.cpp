#include "menu.h"
#include "text.h"
#include <SDL_ttf.h>
#include <SDL.h>
#include "defs.h"

extern TTF_Font* gFont;

static int menuIndex;
static const char* mainMenuLabels[] = {
	"Play",
	"Tutorial",
	"Settings",
	"Quit"
};
static const int MAIN_MENU_COUNT = 4;
static const int MAIN_MENU_START_X = 700;
static const int MAIN_MENU_START_Y = 200;
static const int MAIN_MENU_SPACING_Y = 100;


static int pauseIndex = 0;
static const char* pauseMenuLabels[] = {
	"Resume",
	"Settings",
	"Exit to Main Menu",
	"Quit Game"
};
static const int PAUSE_MENU_COUNT = 4;
static const int PAUSE_MENU_START_X = SCREEN_WIDTH / 2 - 100;
static const int PAUSE_MENU_START_Y = 200;
static const int PAUSE_MENU_SPACING_Y = 60;


bool initMenu(SDL_Renderer* renderer) {
	menuIndex = 0;
	if (!gFont) {
		SDL_Log("Font not loaded! Mouse menu click functionality disabled.");
	}
	return true;
}
void cleanupMenu() {
}
void updateMainMenu(const SDL_Event& e) {
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_DOWN:
		case SDLK_s:
			menuIndex = (menuIndex + 1) % MAIN_MENU_COUNT;
			break;
		case SDLK_UP:
		case SDLK_w:
			menuIndex = (menuIndex + MAIN_MENU_COUNT - 1) % MAIN_MENU_COUNT;
			break;
		case SDLK_RETURN:
			switch (menuIndex) {
			case 0: gameState = GS_PLAYING;  break;
			case 1: gameState = GS_TUTORIAL; break;
			case 2:
				previousState = GS_MENU;
				gameState = GS_SETTINGS;
				break;

			case 3: gameState = GS_EXIT;     break;
			}
			break;
		}
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			int mouseX = e.button.x;
			int mouseY = e.button.y;
			int textW = 0, textH = 0;

			for (int i = 0; i < MAIN_MENU_COUNT; ++i) {
				if (gFont) {
					TTF_SizeText(gFont, mainMenuLabels[i], &textW, &textH);
				}
				else {
					continue;
				}

				SDL_Rect menuItemRect = {
					MAIN_MENU_START_X,
					MAIN_MENU_START_Y + i * MAIN_MENU_SPACING_Y,
					textW,
					textH
				};

				if (mouseX >= menuItemRect.x && mouseX < menuItemRect.x + menuItemRect.w &&
					mouseY >= menuItemRect.y && mouseY < menuItemRect.y + menuItemRect.h)
				{
					switch (i) {
					case 0: gameState = GS_PLAYING;  break;
					case 1: gameState = GS_TUTORIAL; break;
					case 2:
						previousState = GS_MENU;
						gameState = GS_SETTINGS;
						break;

					case 3: gameState = GS_EXIT;     break;
					}
					break;
				}
			}
		}
	}
}
void drawMainMenu(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	SDL_Color white = { 255, 255, 255, 255 };

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	int hoveredIndex = -1;

	// Determine which menu item is hovered
	for (int i = 0; i < MAIN_MENU_COUNT; ++i) {
		int textW = 0, textH = 0;
		if (gFont) {
			TTF_SizeText(gFont, mainMenuLabels[i], &textW, &textH);
		}

		SDL_Rect menuItemRect = {
			MAIN_MENU_START_X,
			MAIN_MENU_START_Y + i * MAIN_MENU_SPACING_Y,
			textW,
			textH
		};

		if (mouseX >= menuItemRect.x && mouseX <= menuItemRect.x + menuItemRect.w &&
			mouseY >= menuItemRect.y && mouseY <= menuItemRect.y + menuItemRect.h) {
			hoveredIndex = i;
			break;
		}
	}

	// Render each item with correct color
	for (int i = 0; i < MAIN_MENU_COUNT; ++i) {
		SDL_Color color;

		if (i == hoveredIndex) {
			color = SDL_Color{ 0, 255, 0, 255 }; // Hovered
		}
		else if (hoveredIndex == -1 && i == menuIndex) {
			color = SDL_Color{ 0, 255, 0, 255 }; // Selected via keyboard
		}
		else {
			color = white; // Normal
		}

		renderText(renderer,
			mainMenuLabels[i],
			MAIN_MENU_START_X,
			MAIN_MENU_START_Y + i * MAIN_MENU_SPACING_Y,
			color);
	}

	SDL_RenderPresent(renderer);
}
void updatePauseMenu(const SDL_Event& e) {
	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_DOWN:
		case SDLK_s:
			pauseIndex = (pauseIndex + 1) % PAUSE_MENU_COUNT;
			break;
		case SDLK_UP:
		case SDLK_w:
			pauseIndex = (pauseIndex + PAUSE_MENU_COUNT - 1) % PAUSE_MENU_COUNT;
			break;
		case SDLK_RETURN:
			switch (pauseIndex) {
			case 0:
				gameState = GS_PLAYING;
				break;
			case 1:
				previousState = GS_PAUSED;
				gameState = GS_SETTINGS;
				break;
			case 2:
				gameState = GS_MENU;
				break;
			case 3:
				gameState = GS_EXIT;
				break;
			}
			break;
		case SDLK_p:
			gameState = GS_PLAYING;
			break;
		default:
			break;
		}
	}
	else if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT)
		{
			int mouseX = e.button.x;
			int mouseY = e.button.y;
			int textW = 0, textH = 0;

			for (int i = 0; i < PAUSE_MENU_COUNT; ++i) {
				if (gFont) {
					TTF_SizeText(gFont, pauseMenuLabels[i], &textW, &textH);
				}
				else {
					continue;
				}

				SDL_Rect menuItemRect = {
					PAUSE_MENU_START_X,
					PAUSE_MENU_START_Y + i * PAUSE_MENU_SPACING_Y,
					textW,
					textH
				};

				if (mouseX >= menuItemRect.x && mouseX < menuItemRect.x + menuItemRect.w &&
					mouseY >= menuItemRect.y && mouseY < menuItemRect.y + menuItemRect.h)
				{
					switch (i) {
					case 0:
						gameState = GS_PLAYING;
						break;
					case 1:
						previousState = GS_PAUSED;
						gameState = GS_SETTINGS;
						break;
					case 2:
						gameState = GS_MENU;
						break;
					case 3:
						gameState = GS_EXIT;
						break;
					}
					break;
				}
			}
		}
	}
}
void drawPauseMenu(SDL_Renderer* renderer) {
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
	SDL_Rect full = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_RenderFillRect(renderer, &full);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);

	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	int hoveredIndex = -1;

	// First check if mouse is hovering over any button
	for (int i = 0; i < PAUSE_MENU_COUNT; ++i) {
		int textW = 0, textH = 0;
		if (gFont) {
			TTF_SizeText(gFont, pauseMenuLabels[i], &textW, &textH);
		}

		SDL_Rect menuItemRect = {
			PAUSE_MENU_START_X,
			PAUSE_MENU_START_Y + i * PAUSE_MENU_SPACING_Y,
			textW,
			textH
		};

		if (mouseX >= menuItemRect.x && mouseX <= menuItemRect.x + menuItemRect.w &&
			mouseY >= menuItemRect.y && mouseY <= menuItemRect.y + menuItemRect.h) {
			hoveredIndex = i;
			break;
		}
	}

	// Then draw all buttons with proper highlight
	for (int i = 0; i < PAUSE_MENU_COUNT; ++i) {
		SDL_Color color;

		if (i == hoveredIndex) {
			color = SDL_Color{ 0, 255, 0, 255 }; // Hovered (green)
		}
		else if (hoveredIndex == -1 && i == pauseIndex) {
			color = SDL_Color{ 0, 255, 0, 255 }; // No hover: highlight selected
		}
		else {
			color = SDL_Color{ 255, 255, 255, 255 }; // Normal (white)
		}

		renderText(renderer,
			pauseMenuLabels[i],
			PAUSE_MENU_START_X,
			PAUSE_MENU_START_Y + i * PAUSE_MENU_SPACING_Y,
			color);
	}

	SDL_RenderPresent(renderer);
}

