#ifndef PLAYERTANK_H
#define PLAYERTANK_H

#include <SDL.h>
#include <vector>
#include "Bullet.h"
#include "Map.h"

class PlayerTank {
private:
    int x, y;                // Vị trí xe tăng
    int speed;               // Tốc độ di chuyển
    int direction;           // Hướng (0: lên, 1: phải, 2: xuống, 3: trái)
    SDL_Renderer* renderer;  // Renderer để vẽ
    SDL_Texture* texture;    // Ảnh xe tăng
    bool hasMoved;           // Xe tăng đã di chuyển chưa
    bool movedThisFrame;

public:
    static const int SIZE = 32; // Kích thước xe tăng
    static const int SPEED = 4; // Tốc độ mặc định
    bool loadTexture(SDL_Renderer* renderer);
    PlayerTank(int startX, int startY, SDL_Renderer* renderer, const Map& map);
    ~PlayerTank();
    void resetMoveFlag();
    void move(int newDirection, const Map& map);
    void render();
    void update(const Map& map, std::vector<Bullet*>& bullets);
    void shoot(std::vector<Bullet*>& bullets, SDL_Renderer* renderer);
    void setDirection(int dir);

    int getX() const { return x; }
    int getY() const { return y; }

};

#endif
