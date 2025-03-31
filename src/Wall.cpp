#ifndef WALL_H
#define WALL_H

#include <SDL.h>

class Wall {
public:
    int x, y;
    SDL_Rect rect;
    bool active;

    Wall(int x, int y) {
        this->x = x;
        this->y = y;
        rect = {x, y, 32, 32}; // Kích thước tường 32x32
        active = true;
    }

    void render(SDL_Renderer* renderer) {
        if (active) {
            SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Màu nâu gạch
            SDL_RenderFillRect(renderer, &rect);
        }
    }
};

#endif // WALL_H

