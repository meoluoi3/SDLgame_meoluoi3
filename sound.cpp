#include "sound.h"
#include "struct.h"
#include <SDL_mixer.h>

extern Mix_Chunk* sounds[SND_MAX];
extern Mix_Music* music;

void initSounds()
{
  

    loadSounds();
}

void loadSounds()
{
    sounds[SND_PLAYER_FIRE] = Mix_LoadWAV("sound/bulletsound.mp3");
    sounds[SND_ALIEN_FIRE] = Mix_LoadWAV("sound/bulletsound.mp3");
    sounds[SND_PLAYER_DIE] = Mix_LoadWAV("sound/low.mp3");
    sounds[SND_ALIEN_DIE] = Mix_LoadWAV("sound/medium.mp3");
}

void loadMusic(const char* filename)
{
    if (music != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = nullptr;
    }

    music = Mix_LoadMUS(filename);
}

void playMusic(int loop)
{
    Mix_PlayMusic(music, (loop) ? -1 : 0);
}

void playSound(int id, int channel)
{
    Mix_PlayChannel(channel, sounds[id], 0);
}
