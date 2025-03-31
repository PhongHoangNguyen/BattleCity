#include "GameScreens.h"
#include <iostream>

GameScreens::GameScreens() : menuTexture(nullptr), gameOverTexture(nullptr), victoryTexture(nullptr) {}

GameScreens::~GameScreens() {
    clean();
}

bool GameScreens::loadTextures(SDL_Renderer* renderer) {
    menuTexture = IMG_LoadTexture(renderer, "assets/menu.png");
    gameOverTexture = IMG_LoadTexture(renderer, "assets/game_over.png");
    victoryTexture = IMG_LoadTexture(renderer, "assets/victory.png");

    if (!menuTexture || !gameOverTexture || !victoryTexture) {
        std::cerr << "Failed to load screen textures: " << IMG_GetError() << std::endl;
        return false;
    }
    return true;
}

void GameScreens::render(SDL_Renderer* renderer, int gameState) {
    SDL_Texture* currentTexture = nullptr;

    switch (gameState) {
        case 0: // MENU_SCREEN
            currentTexture = menuTexture;
            break;
        case 2: // GAME_OVER
            currentTexture = gameOverTexture;
            break;
        case 3: // VICTORY
            currentTexture = victoryTexture;
            break;
        default:
            return;
    }

    SDL_RenderCopy(renderer, currentTexture, NULL, NULL);
}

void GameScreens::clean() {
    if (menuTexture) SDL_DestroyTexture(menuTexture);
    if (gameOverTexture) SDL_DestroyTexture(gameOverTexture);
    if (victoryTexture) SDL_DestroyTexture(victoryTexture);

    menuTexture = nullptr;
    gameOverTexture = nullptr;
    victoryTexture = nullptr;
}
