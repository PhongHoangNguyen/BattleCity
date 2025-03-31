#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>
#include "Map.h"
#include "SDL_mixer.h"

const int BULLET_SIZE = 8;
const int BULLET_SPEED = 8;

class Bullet {
private:
    int x, y;
    int speed;
    int direction;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    bool active;
    bool isPlayerBullet; // Thêm thuộc tính để phân biệt đạn của ai
    static Mix_Chunk* shootSound;  // Âm thanh khi bắn đạn
    void playShootSound();
public:
    Bullet(int startX, int startY, int dir, SDL_Renderer* renderer, bool isPlayerBullet);
    ~Bullet();

    void update(Map& map);
    void render();
    bool isActive() const;
    bool isPlayer() const { return isPlayerBullet; } // Thêm hàm getter
    // Hàm kiểm tra va chạm
    bool checkCollision(int targetX, int targetY, int targetWidth, int targetHeight) const;
    static bool loadSound(); // Hàm load âm thanh
    static void cleanSound(); // Hàm dọn dẹp âm thanh
};

#endif

