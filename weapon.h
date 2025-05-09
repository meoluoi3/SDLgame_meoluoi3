#pragma once

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

enum class WeaponType {
    None,
    AK,
    Pistol,
    Knife
};


struct Weapon {
    WeaponType type;
    std::string name;
    int ammoCapacity;
    int ammo;
    double fireRate;
    double lastFireTime;
    Uint32 reloadtime;
    Uint32 reloadDuration;

    Weapon(WeaponType t, const std::string& n,
        int cap, int a,
        double fRate,
        Uint32 rDuration)
        : type(t)
        , name(n)
        , ammoCapacity(cap)
        , ammo(a)
        , fireRate(fRate)
        , lastFireTime(0.0)
        , reloadtime(0)
        , reloadDuration(rDuration)
    {}
};

struct PlayerWeapons {
    std::vector<Weapon> list;
    int currentIndex; // 0 = AK, 1 = Pistol, 2 = Knife
};

// Functions
void initWeapons(PlayerWeapons& wpnList);
void handleWeaponFire(PlayerWeapons& wpnList, Uint32 currentTime);
void updateWeaponReloads(PlayerWeapons& wpnList, Uint32 currentTime);
void switchWeapon(PlayerWeapons& wpnList, int key);

void playerAndWeaponTexture(PlayerWeapons& wpnList);
void drawAmmoHUD(SDL_Renderer* renderer, const Weapon& currentWeapon);
