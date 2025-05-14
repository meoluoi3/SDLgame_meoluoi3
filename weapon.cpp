#include "weapon.h"
#include "struct.h"
#include "draw.h"
#include "text.h"
extern Entity* player;
extern SDL_Texture* playerStanding;
extern SDL_Texture* playerHoldingAK;
extern SDL_Texture* playerHoldingPistol;
extern SDL_Texture* playerPunching;
extern PlayerWeapons wpnList;
void initWeapons(PlayerWeapons& wpnList) {
    wpnList.list.clear();
    wpnList.currentIndex = 0;

    Uint32 now = SDL_GetTicks();



    Weapon ak = { WeaponType::AK, "AK", 30, 30, 0.13, 2000 };
    ak.lastFireTime = (now - static_cast<Uint32>(ak.fireRate * 1000)) / 1000.0;

    Weapon pistol = { WeaponType::Pistol, "Pistol", 7, 7, 0.30, 1000 };
    pistol.lastFireTime = (now - static_cast<Uint32>(pistol.fireRate * 1000)) / 1000.0;

    Weapon knife = { WeaponType::Knife, "Knife", 0, 0, 0.50, 0 };
    knife.lastFireTime = (now - static_cast<Uint32>(knife.fireRate * 1000)) / 1000.0;

    wpnList.list.push_back(ak);
    wpnList.list.push_back(pistol);
    wpnList.list.push_back(knife);
}


void handleWeaponFire(PlayerWeapons& wpnList, Uint32 currentTime) {
    int idx = wpnList.currentIndex;
    if (idx < 0 || idx >= (int)wpnList.list.size()) return;

    Weapon& w = wpnList.list[idx];

    Uint32 fireDelay = static_cast<Uint32>(w.fireRate * 1000); // convert fireRate to ms

    if (currentTime - w.lastFireTime < fireDelay) return;

    if (w.type == WeaponType::Knife || w.ammo > 0) {
        if (w.type != WeaponType::Knife) {
            std::cerr << "ammo: " << w.ammo << "\n";
        }

        w.lastFireTime = currentTime; 
    }
}
bool forceReload(PlayerWeapons& wpnList, Uint32 currentTime) {
    Weapon& w = wpnList.list[wpnList.currentIndex];

    // Only reload if it's not already reloading and not full
    if (w.ammo < w.ammoCapacity && w.reloadtime == 0) {
        w.reloadtime = currentTime;
        SDL_Log("Force reloading %s...", w.name.c_str());
        return true;
    }
    return false;
}

void updateWeaponReloads(PlayerWeapons& wpnList, Uint32 currentTime) {
    for (auto& w : wpnList.list) {
        if (w.reloadtime > 0 ) {
            if (w.type == WeaponType::AK ) {
                std::cerr << "AK: " << currentTime - w.reloadtime << "\n";
                if (currentTime - w.reloadtime >= w.reloadDuration) {
                    w.ammo = w.ammoCapacity;
                    w.reloadtime = 0;
                }
            }
            else if (w.type == WeaponType::Pistol) {
                std::cerr << "pistol: " << currentTime - w.reloadtime << "\n";
                if (currentTime - w.reloadtime >= w.reloadDuration) {
                    w.ammo = w.ammoCapacity;
                    w.reloadtime = 0;
                }
            }
            

        }
    }
}
void drawReloadText(SDL_Renderer* renderer, PlayerWeapons& wpnList) {
    int idx = wpnList.currentIndex;
    if (idx < 0 || idx >= (int)wpnList.list.size()) return;

    const Weapon& w = wpnList.list[idx];
    if (w.reloadtime == 0) return;  

    
     std::string msg = "Reloading...";
    SDL_Color white = { 255, 255, 255, 255 };


    renderText(renderer, msg, 250, 100, white);
}

void switchWeapon(PlayerWeapons& wpnList, int key)
{
    int newIndex = -1;

    if (key == SDLK_1) newIndex = 0;
    else if (key == SDLK_2) newIndex = 1;
    else if (key == SDLK_3) newIndex = 2;

    if (newIndex != -1)
    {
        
        if (wpnList.currentIndex != newIndex)
        {
          
            if (wpnList.currentIndex >= 0 && wpnList.currentIndex < wpnList.list.size())
            {
                wpnList.list[wpnList.currentIndex].reloadtime = 0;
            }

            
            wpnList.currentIndex = newIndex;

            
            if (wpnList.currentIndex >= 0 && wpnList.currentIndex < wpnList.list.size())
            {
                wpnList.list[wpnList.currentIndex].reloadtime = 0;
            }
        }
        else
        {
            
            wpnList.currentIndex = -1;
            player->texture = playerStanding;
        }
    }
}



void resetWeapons()
{
    for (auto& w : wpnList.list)
    {
        w.ammo = w.ammoCapacity;
        w.reloadtime = 0;
        w.lastFireTime = 0.0;  
    }

    
    wpnList.currentIndex = 0; 
}



void playerAndWeaponTexture(PlayerWeapons& wpnList) {
    if (!player) return;
    int idx = wpnList.currentIndex;
    if (idx < 0 || idx >= (int)wpnList.list.size()) {
        player->texture = playerStanding;
        return;
    }

    switch (wpnList.list[idx].type) {
    case WeaponType::AK:
        player->texture = playerHoldingAK;
        break;
    case WeaponType::Pistol:
        player->texture = playerHoldingPistol;
        break;
    case WeaponType::Knife:
        player->texture = playerPunching;
        break;
    default:
        player->texture = playerStanding;
        break;
    }
}


void drawAmmoHUD(SDL_Renderer* renderer, const Weapon& currentWeapon) {
    std::string ammoText =
        std::to_string(currentWeapon.ammo) + " / " +
        std::to_string(currentWeapon.ammoCapacity);

    SDL_Color white = { 255,255,255,255 };
    int ammoX = 100, ammoY = 100;

    renderText(renderer, ammoText, ammoX, ammoY, white, 1);
}

