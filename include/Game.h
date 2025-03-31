#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Map.h"
#include "PlayerTank.h"
#include "Enemy_tank.h"
#include "Bullet.h"
#include "GameScreens.h"
#include "SDL_mixer.h"
#include "GameScreens.h"

extern Mix_Chunk* shootSound;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void spawnEnemies();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool isRunning() { return running; }
    void loadMap(const std::string& filename);
private:
    std::vector<EnemyTank*> enemies;
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    Map map{"map/map.csv"};
    PlayerTank* player;
    GameScreens gameScreens;
    std::vector<Bullet*> bullets;

    void spawnPlayer();
    void checkGameConditions(); // Kiểm tra điều kiện thắng/thua
        enum GameState {
        START_SCREEN,
        PLAYING,
        LOSE_SCREEN,
        WIN_SCREEN
    };
    GameState gameState;
};

#endif
