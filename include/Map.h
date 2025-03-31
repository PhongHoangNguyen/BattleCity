#ifndef MAP_H
#define MAP_H

#include <vector>
#include <SDL.h>

class Map {
public:
    static const int TILE_SIZE = 32;
    enum TileType { EMPTY = 0, WALL = 1, WATER = 2, GRASS = 3 };

    Map(const char* filename);
    ~Map();

    void loadMap(const char* filename);
    void render(SDL_Renderer* renderer);
    void setTile(int x, int y, int value);
    int getTile(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    bool loadTextures(SDL_Renderer* renderer);
private:
    std::vector<std::vector<int>> grid;
    int width, height;
    SDL_Texture* wallTexture;
    SDL_Texture* waterTexture;
    SDL_Texture* grassTexture;
};

#endif
