#include "Map.h"
#include <SDL2/SDL_image.h>
#include <fstream>
#include <iostream>

Map::Map(const char* filename) : wallTexture(nullptr), waterTexture(nullptr), grassTexture(nullptr) {
    loadMap(filename);
}

Map::~Map() {
    if (wallTexture) SDL_DestroyTexture(wallTexture);
    if (waterTexture) SDL_DestroyTexture(waterTexture);
    if (grassTexture) SDL_DestroyTexture(grassTexture);
}

bool Map::loadTextures(SDL_Renderer* renderer) {
    // Load texture cho tường
    SDL_Surface* surface = IMG_Load("assets/wall.png");
    if (!surface) {
        SDL_Log("Không thể load texture tường: %s", IMG_GetError());
        return false;
    }
    wallTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Load texture cho nước
    surface = IMG_Load("assets/water.png");
    if (!surface) {
        SDL_Log("Không thể load texture nước: %s", IMG_GetError());
        return false;
    }
    waterTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Load texture cho cỏ
    surface = IMG_Load("assets/grass.png");
    if (!surface) {
        SDL_Log("Không thể load texture cỏ: %s", IMG_GetError());
        return false;
    }
    grassTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return true;
}

void Map::render(SDL_Renderer* renderer) {
    for (size_t i = 0; i < grid.size(); i++) {
        for (size_t j = 0; j < grid[i].size(); j++) {
            SDL_Rect rect = { (int)j * TILE_SIZE, (int)i * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            switch (grid[i][j]) {
                case WALL:
                    if (wallTexture)
                        SDL_RenderCopy(renderer, wallTexture, nullptr, &rect);
                    break;
                case WATER:
                    if (waterTexture)
                        SDL_RenderCopy(renderer, waterTexture, nullptr, &rect);
                    break;
                case GRASS:
                    if (grassTexture)
                        SDL_RenderCopy(renderer, grassTexture, nullptr, &rect);
                    break;
                case EMPTY:
                    // Không render gì cho ô trống
                    break;
            }
        }
    }
}
void Map::loadMap(const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Không thể mở file map: " << filename << std::endl;
        return;
    }

    grid.clear(); // Xóa dữ liệu cũ trước khi tải mới
    std::string line;

    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char c : line) {
            if (c >= '0' && c <= '9') {
                row.push_back(c - '0'); // Chuyển ký tự số thành số nguyên
            }
        }
        grid.push_back(row);
    }

    file.close();
}

void Map::setTile(int x, int y, int value) {
    int width = getWidth();
    int height = getHeight();

    if (x < 0 || x >= width || y < 0 || y >= height) {
        SDL_Log("Lỗi: Ghi vào ô ngoài phạm vi (%d, %d) (Map: %dx%d)", x, y, width, height);
        return;
    }
    SDL_Log("Thay đổi ô (%d, %d) từ %d -> %d", x, y, grid[y][x], value);
    grid[y][x] = value;
}


int Map::getTile(int x, int y) const {
    int width = getWidth();
    int height = getHeight();

    if (x < 0 || x >= width || y < 0 || y >= height) {
        SDL_Log("Lỗi: Truy cập ô ngoài phạm vi (%d, %d) (Map: %dx%d)", x, y, width, height);
        return -1;
    }
    return grid[y][x];
}

int Map::getWidth() const {
    return grid.empty() ? 0 : grid[0].size();
}

int Map::getHeight() const {
    return grid.size();
}
