#include "menu.h"
#include "settings.h"
#include "sound.h"
#include "init.h"
#include "struct.h"
#include <SDL_mixer.h>
#include "draw.h"
#include "text.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

Settings settings = { 100, 100, 0, 0 };

static const vector<string> bgNames = { "background", "drthanh", "Stars" };
static const vector<string> musicNames = { "_Hotline Miami Soundtrack_Crystals.mp3" , "_Hotline Miami Soundtrack . Perturbator.mp3", "_Hotline Miami Soundtrack . -Paris-.mp3", "_Hotline Miami Soundtrack . Musik.mp3", "_Hotline Miami Soundtrack . Miami.mp3", "_Hotline Miami Soundtrack . It's Safe Now.mp3", "_Hotline Miami Soundtrack . Hydrogen.mp3", "_Hotline Miami Soundtrack . Daisuke.mp3", "_Hotline Miami Soundtrack . A New Morning.mp3"};

int selectedSettingIndex = 0;
const int settingCount = 6;
int currentMusicIndex = -1;
void applySettings() {
    Mix_Volume(-1, settings.sfxVolume * 128 / 100);
    Mix_VolumeMusic(settings.musicVolume * 128 / 100);

    extern SDL_Texture* background;
    background = loadTexture(("img/" + bgNames[settings.bgIndex] + ".png").c_str());

    if (settings.musicIndex != currentMusicIndex) {
        loadMusic(("sound/" + musicNames[settings.musicIndex]).c_str());
        currentMusicIndex = settings.musicIndex;
    }
}


void updateSettings(const SDL_Event& e) {
    if (e.type != SDL_KEYDOWN) return;
    bool changed = false;

    switch (e.key.keysym.sym) {
    case SDLK_UP: case SDLK_w:
        selectedSettingIndex = (selectedSettingIndex - 1 + settingCount) % settingCount;
        return;
    case SDLK_DOWN: case SDLK_s:
        selectedSettingIndex = (selectedSettingIndex + 1) % settingCount;
        return;
    case SDLK_ESCAPE:
        gameState = previousState;
        return;
    case SDLK_RETURN:
        if (selectedSettingIndex == 4) { playMusic(-1); return; }
        if (selectedSettingIndex == 5) { stopMusic();    return; }
        break;
    default:
        break;
    }

    switch (selectedSettingIndex) {
    case 0:
        if ((e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) && settings.sfxVolume < 100) {
            settings.sfxVolume += 5; changed = true;
        }
        else if ((e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) && settings.sfxVolume > 0) {
            settings.sfxVolume -= 5; changed = true;
        }
        break;

    case 1:
        if ((e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) && settings.musicVolume < 100) {
            settings.musicVolume += 5; changed = true;
        }
        else if ((e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) && settings.musicVolume > 0) {
            settings.musicVolume -= 5; changed = true;
        }
        break;

    case 2:
        if ((e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) && settings.bgIndex < (int)bgNames.size() - 1) {
            settings.bgIndex++; changed = true;
        }
        else if ((e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) && settings.bgIndex > 0) {
            settings.bgIndex--; changed = true;
        }
        break;

    case 3:
        if ((e.key.keysym.sym == SDLK_RIGHT || e.key.keysym.sym == SDLK_d) && settings.musicIndex < (int)musicNames.size() - 1) {
            settings.musicIndex++;
            applySettings();
            playMusic(-1);
        }
        else if ((e.key.keysym.sym == SDLK_LEFT || e.key.keysym.sym == SDLK_a) && settings.musicIndex > 0) {
            settings.musicIndex--;
            applySettings();
            playMusic(-1);
        }
        return;

    default:
        break;
    }

    if (changed) applySettings();
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
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = { 255,255,255,255 };
    SDL_Color green = { 0,255,0,255 };

    renderText(renderer, "Settings", 650, 50, white);

    for (int i = 0; i < settingCount; ++i) {
        SDL_Color color = (selectedSettingIndex == i ? green : white);
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
            string label = "Background: ";
            label += (settings.bgIndex > 0 ? "< " : "  ");
            label += bgNames[settings.bgIndex];
            label += (settings.bgIndex < (int)bgNames.size() - 1 ? " >" : "  ");
            renderText(renderer, label, 600, y, color);
            break;
        }

        case 3: {
            string label = "Music: ";
            label += (settings.musicIndex > 0 ? "< " : "  ");
            label += musicNames[settings.musicIndex];
            label += (settings.musicIndex < (int)musicNames.size() - 1 ? " >" : "  ");
            renderText(renderer, label, 600, y, color);
            break;
        }

        case 4:
            renderText(renderer, "▶ Play Music (Enter)", 600, y, color);
            break;

        case 5:
            renderText(renderer, "■ Stop Music (Enter)", 600, y, color);
            break;
        }
    }

    renderText(renderer, "Press ESC to ", 600, 700, white);
    renderText(renderer, (gameState == GS_PAUSED ? "Return" : "Back"), 800, 700, white);
    SDL_RenderPresent(renderer);
}
