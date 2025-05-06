#pragma once

// Loads all the sound files needed for the game
void loadSounds(void);

// Initializes sound system, setting up necessary configurations
void initSounds(void);

// Loads a music file (given its filename) into memory
void loadMusic(const char* filename);

// Plays the loaded music, with the loop parameter determining how many times it should repeat
void playMusic(int loop);

// Plays a sound effect identified by an id on a specific channel
void playSound(int id, int channel);
void stopMusic();