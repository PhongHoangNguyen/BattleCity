#ifndef GAME_SCREENS_H
#define GAME_SCREENS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class GameScreens {
public:
    GameScreens();
    ~GameScreens();

    bool loadTextures(SDL_Renderer* renderer);
    void render(SDL_Renderer* renderer, int gameState);
    void clean();

private:
    SDL_Texture* menuTexture;
    SDL_Texture* gameOverTexture;
    SDL_Texture* victoryTexture;
};

#endif
