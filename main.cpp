#include "Game.h"

int main(int argc, char* argv[]) {
    Game game;

    if (!game.init("Battle City", SCREEN_WIDTH, SCREEN_HEIGHT)) {
        return -1;
    }

    game.loadMap("map/map.csv"); // Load bản đồ từ file CSV

    while (game.isRunning()) {
        game.handleEvents();
        game.update();
        game.render();
        SDL_Delay(16); // Giới hạn FPS ~60
    }

    game.clean();
    return 0;
}
