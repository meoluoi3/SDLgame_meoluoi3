#include "dungeon.h"
#include "struct.h"
#include "init.h"
#include "draw.h"
#include "effect.h"
#include "player.h"
#include "weapon.h"
#include "bullet.h"
#include "enemy.h"
#include "math.h"
#include "stage.h"

extern Stage stage;
extern int enemySpawnTimer;
extern int stageResetTimer;
extern App app;
extern std::vector<SDL_Texture*> tiles;

int moveDelay = 0;

void resetDungeon() {
    loadTiles();
    initMap();

    stage.dungeon.camera.x = 0;
    stage.dungeon.camera.y = 0;

    app.delegate.logic = &logicDungeon;
    app.delegate.draw = &drawDungeon;
}


void loadTiles()
{
    tiles.clear();
    tiles.resize(48, nullptr); // max index = 47

    for (int i = 1; i <= 10; ++i)
    {
        std::ostringstream ss;
        ss << "kenney_top-down-shooter/PNG/Tiles/tile_"
            << std::setw(2) << std::setfill('0') << i << ".png";
        SDL_Texture* tex = loadTexture(ss.str().c_str());
        if (!tex) SDL_Log("Failed to load %s", ss.str().c_str());
        tiles[i] = tex;
    }

    for (int i = 42; i <= 47; ++i)
    {
        std::ostringstream ss;
        ss << "kenney_top-down-shooter/PNG/Tiles/tile_"
            << std::setw(2) << std::setfill('0') << i << ".png";
        SDL_Texture* tex = loadTexture(ss.str().c_str());
        if (!tex) SDL_Log("Failed to load %s", ss.str().c_str());
        tiles[i] = tex;
    }
}

#define PLAYER_WIDTH  24
#define PLAYER_HEIGHT 24


void logicDungeon()
{
    std::cerr << "px: " << player->px << " py: " << player->py
        << " tile: (" << player->x << "," << player->y << ")\n";

    updateCameraDelay();
    doPlayerMovement(wpnList);
    doFighters();
    doBullets();
    doEnemies();
    updateWeaponReloads(wpnList, SDL_GetTicks());
    doExplosions();
    doDebris();

    if (stage.score > stage.highscore) {
        stage.highscore = stage.score;
    }

    if (player == nullptr && --stageResetTimer <= 0) {
        SDL_Log("Calling resetStage()...\n");
        resetStage();
    }
}

bool mapdraw = false;

void drawDungeon()
{
    drawMapTiles();  
    drawFighters();
    drawBullets();
    drawDebris();
    drawExplosions();

    playerAndWeaponTexture(wpnList);

    drawHud(app.renderer, stage.score, stage.highscore);

    int idx = wpnList.currentIndex;
    if (idx >= 0 && idx < (int)wpnList.list.size()) {
        drawAmmoHUD(app.renderer, wpnList.list[idx]);
    }
}

// Hàm đọc map từ file CSV
bool loadMapFromCSV(const std::string& filename, Map& map) {
    std::ifstream file(filename); // Mở file

    if (!file.is_open()) { // Kiểm tra xem file có mở được không
        std::cerr << "Failed to open map file: " << filename << std::endl;
        return false; // Trả về false nếu mở thất bại
    }

    std::string line;
    int row = 0; // Dòng hiện tại trong map

    // Dùng cho việc chọn ngẫu nhiên tile hình ảnh
    std::srand(static_cast<unsigned int>(std::time(0))); // Khởi tạo seed random

    while (std::getline(file, line) && row < MAP_HEIGHT) {
        std::stringstream ss(line); // Dùng stringstream để phân tích dòng
        std::string segment;
        int col = 0; // Cột hiện tại trong map

        // Đọc từng giá trị trong dòng, phân cách bởi dấu phẩy
        while (std::getline(ss, segment, ',') && col < MAP_WIDTH) {
            try {
                // Chuyển chuỗi thành số nguyên
                int tileType = std::stoi(segment);

                // Lưu loại tile logic
                map.data[col][row] = tileType;

                // Chọn ngẫu nhiên tile hình ảnh dựa trên loại tile logic
                if (tileType == TILE_GROUND) {
                    // Chọn ngẫu nhiên trong khoảng 1-10 cho đất
                    map.tileVisual[col][row] = 1 + std::rand() % 10;
                }
                else if (tileType == TILE_WALL) {
                    // Chọn ngẫu nhiên trong khoảng 42-47 cho tường
                    map.tileVisual[col][row] = 42 + std::rand() % 6;
                }
                else {
                    // Xử lý các loại tile khác nếu có, hoặc gán một giá trị mặc định
                    map.tileVisual[col][row] = 0; // Hoặc một tile "trống"
                }

                col++; // Chuyển sang cột tiếp theo

            }
            catch (const std::invalid_argument& ia) {
                std::cerr << "Invalid number in map file: " << segment << " at line " << row + 1 << ", col " << col + 1 << std::endl;
                // Có thể chọn cách bỏ qua tile này hoặc báo lỗi nặng hơn
                map.data[col][row] = -1; // Giá trị lỗi
                map.tileVisual[col][row] = 0;
                col++;
            }
            catch (const std::out_of_range& oor) {
                std::cerr << "Number out of range in map file: " << segment << " at line " << row + 1 << ", col " << col + 1 << std::endl;
                // Xử lý lỗi tương tự invalid_argument
                map.data[col][row] = -1;
                map.tileVisual[col][row] = 0;
                col++;
            }
        }

        // Kiểm tra xem số cột trong dòng có khớp với MAP_WIDTH không (tùy chọn)
        if (col != MAP_WIDTH) {
            std::cerr << "Warning: Line " << row + 1 << " has " << col << " columns, expected " << MAP_WIDTH << std::endl;
            // Có thể cần xử lý lỗi nếu không khớp kích thước
        }

        row++; // Chuyển sang dòng tiếp theo
    }

    // Kiểm tra xem số dòng trong file có khớp với MAP_HEIGHT không (tùy chọn)
    if (row != MAP_HEIGHT) {
        std::cerr << "Warning: Map file has " << row << " rows, expected " << MAP_HEIGHT << std::endl;
        // Có thể cần xử lý lỗi nếu không khớp kích thước
    }


    file.close(); // Đóng file
    return true; // Trả về true nếu đọc thành công (sau khi xử lý lỗi)
}