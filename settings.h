#pragma once
// Settings
#include <vector>
#include <string>
extern const std::vector<int> fpsCapOptions;
void applySettings();
void updateSettings(const SDL_Event& e);
void drawSettings(SDL_Renderer* renderer);