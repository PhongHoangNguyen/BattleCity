#include "Bullet.h"
#include"Map.h"
#include <SDL2/SDL_image.h>
Mix_Chunk* Bullet::shootSound = nullptr;

Bullet::Bullet(int startX, int startY, int dir, SDL_Renderer* renderer, bool isPlayerBullet)
        : x(startX), y(startY), speed(BULLET_SPEED), direction(dir), renderer(renderer), active(true), isPlayerBullet(isPlayerBullet) {

        // Load texture khác nhau cho đạn của người chơi và đạn của kẻ địch
        const char* bulletImage = isPlayerBullet ? "assets/player_bullet.png" : "assets/enemy_bullet.png";
        SDL_Surface* tempSurface = IMG_Load(bulletImage);
        if (!tempSurface) {
            SDL_Log("Lỗi tải ảnh đạn: %s", IMG_GetError());
            texture = nullptr;
        } else {
            texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
        }
        playShootSound();
    }

Bullet::~Bullet() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Bullet::update(Map& map) {
    switch (direction) {
        case 0: y -= speed; break;
        case 1: x += speed; break;
        case 2: y += speed; break;
        case 3: x -= speed; break;
    }

    int tileX = x / Map::TILE_SIZE;
    int tileY = y / Map::TILE_SIZE;

    if (tileX < 0 || tileX >= map.getWidth() || tileY < 0 || tileY >= map.getHeight()) {
        active = false;
        return;
    }

    int tileType = map.getTile(tileX, tileY);

    // Nếu là tường thì hủy đạn luôn
    if (tileType == Map::WALL) {
        active = false;
        return;
    }

    // Nếu là cỏ, chỉ đạn của người chơi mới phá hủy cỏ
    if (tileType == Map::GRASS) {
        map.setTile(tileX, tileY, Map::EMPTY); // Xóa cỏ
        active = false;
    }
}


void Bullet::render() {
    if (!active) return;
    SDL_Rect rect = { x, y, BULLET_SIZE, BULLET_SIZE };
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    } else {
        SDL_SetRenderDrawColor(renderer, isPlayerBullet ? 255 : 255, isPlayerBullet ? 255 : 0, 0, 255); // Màu khác nhau cho đạn
        SDL_RenderFillRect(renderer, &rect);
    }
}
bool Bullet::loadSound() {
    SDL_Log("Đang tải âm thanh bắn đạn...");
    shootSound = Mix_LoadWAV("assets/shoot.wav");
    if (!shootSound) {
        SDL_Log("Không thể load âm thanh bắn đạn: %s", Mix_GetError());
        return false;
    }
    SDL_Log("Âm thanh bắn đạn tải thành công!");
    return true;
}


bool Bullet::checkCollision(int targetX, int targetY, int targetWidth, int targetHeight) const {
    // Kiểm tra va chạm giữa đạn và đối tượng (xe tăng)
    if (x + BULLET_SIZE > targetX && x < targetX + targetWidth &&
        y + BULLET_SIZE > targetY && y < targetY + targetHeight) {
        return true; // Có va chạm
    }
    return false; // Không va chạm
}
bool Bullet::isActive() const {
    return active;
}
void Bullet::playShootSound() {
    if (shootSound) {
        Mix_PlayChannel(-1, shootSound, 0); // Phát âm thanh
    }
}

void Bullet::cleanSound() {
    if (shootSound) {
        Mix_FreeChunk(shootSound);
        shootSound = nullptr;
    }
}
