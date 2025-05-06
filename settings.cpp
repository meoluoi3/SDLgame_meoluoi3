#include "menu.h"
#include "settings.h"
#include "sound.h"
#include "init.h"
#include "struct.h"
#include <SDL_mixer.h>
#include "draw.h"
#include "text.h"

Settings settings = { 100, 100, 0, 0 };
static const char* bgNames[] = { "background", "Nebula", "Stars" };
static const char* musicNames[] = { "track1.mp3", "track2.mp3" };

int selectedSettingIndex = 0; // 0: SFX, 1: Music, 2: Background, 3: Track
const int settingCount = 4;

void applySettings() {
    // Map 0–100 to SDL_mixer’s 0–128
    Mix_Volume(-1, settings.sfxVolume * 128 / 100);
    Mix_VolumeMusic(settings.musicVolume * 128 / 100);

    extern SDL_Texture* background;
    background = loadTexture(
        ("img/" + std::string(bgNames[settings.bgIndex]) + ".png").c_str()
    );

    loadMusic(
        ("sound/" + std::string(musicNames[settings.musicIndex])).c_str()
    );
    playMusic(-1);
}

void updateSettings(const SDL_Event& e) {
    if (e.type == SDL_KEYDOWN) {
        bool changed = false;

        switch (e.key.keysym.sym) {
        case SDLK_UP:
            selectedSettingIndex = (selectedSettingIndex - 1 + settingCount) % settingCount;
            break;
        case SDLK_DOWN:
            selectedSettingIndex = (selectedSettingIndex + 1) % settingCount;
            break;
        case SDLK_ESCAPE:
            gameState = (gameState == GS_PAUSED ? GS_PAUSED : GS_MENU);
            break;
        default:
            break;
        }

        switch (selectedSettingIndex) {
        case 0: // SFX Volume
            if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
                if (settings.sfxVolume < 100) {
                    settings.sfxVolume += 5;
                    changed = true;
                }
            }
            else if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
                if (settings.sfxVolume > 0) {
                    settings.sfxVolume -= 5;
                    changed = true;
                }
            }
            break;

        case 1: // Music Volume
            if (e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) {
                if (settings.musicVolume < 100) {
                    settings.musicVolume += 5;
                    changed = true;
                }
            }
            else if (e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) {
                if (settings.musicVolume > 0) {
                    settings.musicVolume -= 5;
                    changed = true;
                }
            }
            break;

        case 2: // Background
            if ((e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) && settings.bgIndex < 2) {
                settings.bgIndex++;
                changed = true;
            }
            else if ((e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) && settings.bgIndex > 0) {
                settings.bgIndex--;
                changed = true;
            }
            break;

        case 3: // Music track
            if ((e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) && settings.musicIndex < 1) {
                settings.musicIndex++;
                changed = true;
            }
            else if ((e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) && settings.musicIndex > 0) {
                settings.musicIndex--;
                changed = true;
            }
            break;
        }

        if (changed)
            applySettings();
    }
}


void drawSlider(SDL_Renderer* renderer, int x, int y, int value, int max, SDL_Color color) {
    SDL_Rect bg = { x, y, 100, 10 };
    SDL_Rect fg = { x, y, value * 100 / max, 10 };

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &bg);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &fg);
}

void drawSettings(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Dòng này để đặt màu nền đen
    SDL_RenderClear(renderer);

    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color green = { 0, 255, 0, 255 };

    renderText(renderer, "Settings", 650, 50, white);

    for (int i = 0; i < 4; ++i) {
        SDL_Color color = (selectedSettingIndex == i) ? green : white;
        int y = 150 + i * 50;

        switch (i) {
        case 0:
            renderText(renderer, "SFX Volume", 600, y, color);
            drawSlider(renderer, 820, y + 10, settings.sfxVolume, 100, color);
            renderText(renderer, std::to_string(settings.sfxVolume), 930, y, color);

            break;
        case 1:
            renderText(renderer, "Music Volume", 600, y, color);
            drawSlider(renderer, 820, y + 10, settings.musicVolume, 100, color);
            renderText(renderer, std::to_string(settings.musicVolume), 930, y, color);

            break;
        case 2: {
            std::string label = "Background: ";

            if (settings.bgIndex > 0)
                label += "< ";
            else
                label += "  "; // preserve alignment

            label += bgNames[settings.bgIndex];

            if (settings.bgIndex < (int)(sizeof(bgNames) / sizeof(bgNames[0])) - 1)
                label += " >";
            else
                label += "  "; // preserve alignment

            renderText(renderer, label, 600, y, color);
            break;
        }

        case 3: {
            std::string label = "Music: ";

            if (settings.musicIndex > 0)
                label += "< ";
            else
                label += "  ";

            label += musicNames[settings.musicIndex];

            if (settings.musicIndex < (int)(sizeof(musicNames) / sizeof(musicNames[0])) - 1)
                label += " >";
            else
                label += "  ";

            renderText(renderer, label, 600, y, color);
            break;
        }



        }
    }

    renderText(renderer, "Press ESC to ", 600, 700, white);
    renderText(renderer, (gameState == GS_PAUSED ? "Return" : "Back"), 800, 700, white);

    SDL_RenderPresent(renderer);
}