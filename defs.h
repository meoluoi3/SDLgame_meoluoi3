#ifndef DEFS
#define DEFS
#define PI 3.14159265358979323846
#define SCREEN_WIDTH 1600  
#define SCREEN_HEIGHT 900 
#define PLAYER_SPEED  10
#define PLAYER_BULLET_SPEED   16
#define MAX_KEYBOARD_KEYS  350
#define SIDE_PLAYER 0
#define SIDE_ALIEN  1
#define FPS 60
#define MAX_MOUSE_BUTTONS 3 
#define ALIEN_BULLET_SPEED    8
#define MAX_STARS   500
#define MAX_SND_CHANNELS 8
#define MAX_NAME_LENGTH 1000


#define SURVIVOR_MODE 1
#define DUNGEON_MODE 2

#define MAX_TILES 538

#define TILE_SIZE      64


#define MAP_RENDER_WIDTH   (SCREEN_WIDTH  / TILE_SIZE)   // = 1600/64 = 25
#define MAP_RENDER_HEIGHT  (SCREEN_HEIGHT / TILE_SIZE)   // = 900/64  = 14


#define MAP_WIDTH    (MAP_RENDER_WIDTH  * 3)  // 25 * 3 = 75
#define MAP_HEIGHT   (MAP_RENDER_HEIGHT * 3)  // 14 * 3 = 42


#define TILE_GROUND   1
#define TILE_WALL  2


enum
{
    CH_ANY = -1,
    CH_PLAYER,
    CH_ALIEN_FIRE
};

enum
{
    SND_PLAYER_FIRE,
    SND_ALIEN_FIRE,
    SND_PLAYER_DIE,
    SND_ALIEN_DIE,
    SND_MAX
};

enum GameState {
    GS_MENU,
    GS_TUTORIAL,
    GS_SETTINGS,
    GS_PLAYING,
    GS_PAUSED,
    GS_EXIT,
    GS_MODE_SELECTION
};
extern GameState gameState;
extern GameState previousState;
enum MovePattern {
    PATTERN_LINEAR,
    PATTERN_ZIGZAG,
    PATTERN_WAVE,
    PATTERN_SPIRAL,
    PATTERN_RANDOM_DRUNK,
    PATTERN_CIRCLE
};

#endif // DEFS
