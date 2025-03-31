#include "Enemy_tank.h"
#include <SDL2/SDL_image.h>
#include <cstdlib>

EnemyTank::EnemyTank(int startX, int startY, SDL_Renderer* renderer)
    : x(startX), y(startY), direction(rand() % 4), renderer(renderer), delayCounter(0) {

    SDL_Surface* tempSurface = IMG_Load("assets/enemy_tank.png");
    if (!tempSurface) {
        SDL_Log("Lỗi load ảnh xe tăng địch: %s", IMG_GetError());
        texture = nullptr;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);
    }
}

EnemyTank::~EnemyTank() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void EnemyTank::move(const Map& map) {
    int dx = 0, dy = 0;
    switch (direction) {
        case 0: dy = -1; break;  // Lên
        case 1: dx = 1; break;   // Phải
        case 2: dy = 1; break;   // Xuống
        case 3: dx = -1; break;  // Trái
    }

    int tileX = x / Map::TILE_SIZE;
    int tileY = y / Map::TILE_SIZE;
    int nextTileX = tileX + dx;
    int nextTileY = tileY + dy;

    if (nextTileX < 0 || nextTileX >= map.getWidth() || nextTileY < 0 || nextTileY >= map.getHeight()) {
        direction = rand() % 4; // Đổi hướng ngẫu nhiên
        return;
    }

    int tileType = map.getTile(nextTileX, nextTileY);
    if (tileType == Map::WALL || tileType == Map::GRASS) {
        direction = rand() % 4; // Đổi hướng nếu gặp vật cản
        return;
    }

    x = nextTileX * Map::TILE_SIZE;
    y = nextTileY * Map::TILE_SIZE;
}
void EnemyTank::shoot(std::vector<Bullet*>& bullets) {
    // Tạo đạn mới và thêm vào danh sách bullets
    Bullet* newBullet = new Bullet(x, y, direction, renderer, false);
    bullets.push_back(newBullet);
}

void EnemyTank::update(const Map& map, std::vector<Bullet*>& bullets) {
    delayCounter++;
    if (delayCounter >= moveDelay) { // Chỉ di chuyển sau mỗi `moveDelay` khung hình
    move(map);
    delayCounter = 0; // Reset biến đếm
    }
    shootDelayCounter++;
    if (shootDelayCounter >= this->shootDelay) {
        shoot(bullets); // Bắn đạn
        shootDelayCounter = 0;
    }
}


void EnemyTank::render() {
    SDL_Rect rect = { x, y, Map::TILE_SIZE, Map::TILE_SIZE };
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
    }
}
