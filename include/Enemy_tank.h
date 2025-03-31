#ifndef ENEMY_TANK_H
#define ENEMY_TANK_H

#include <SDL2/SDL.h>
#include "Map.h"
#include "Bullet.h"
#include <vector>
class EnemyTank {
private:
    int x, y;
    int direction;
    SDL_Texture* texture;
    SDL_Renderer* renderer;
    int delayCounter; // Biến đếm thời gian
    int shootDelayCounter; // Đếm thời gian giữa các lần bắn
    const int moveDelay = 15; // Độ trễ giữa các lần di chuyển
    const int shootDelay = 30; // ĐỘ trễ giauwx các lần bắn
public:
    EnemyTank(int startX, int startY, SDL_Renderer* renderer);
    ~EnemyTank();

    void move(const Map& map);
    void update(const Map& map,std::vector<Bullet*>& bullets);
    void render();

    int getX() const { return x; }
    int getY() const { return y; }
    int getDirection() const { return direction; }
    void shoot(std::vector<Bullet*>& bullets); // Hàm bắn đạn
};

#endif
