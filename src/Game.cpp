#include "Game.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm> // Để dùng std::random_shuffle
#include <algorithm> // Để dùng std::random_shuffle
#include <random>    // Để tạo số ngẫu nhiên
#include <SDL_mixer.h>
Mix_Chunk* shootSound = nullptr;

std::vector<EnemyTank*> enemyTanks;
Game::Game() : window(nullptr), renderer(nullptr), running(false), player(nullptr), map("map/map.csv"){}


Game::~Game() {
    // Giải phóng âm thanh
    if (shootSound) {
        Mix_FreeChunk(shootSound);
        shootSound = nullptr;
    }
    Mix_CloseAudio(); // Đóng SDL_mixer
    delete player;
    for (Bullet* bullet : bullets) {
        delete bullet;
    }
    clean();
}


bool Game::init(const char* title, int width, int height) {
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL không thể khởi tạo: " << SDL_GetError() << std::endl;
        return false;
    }

    // Tạo cửa sổ
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Không thể tạo cửa sổ: " << SDL_GetError() << std::endl;
        return false;
    }

    // Tạo renderer với VSync
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Không thể tạo renderer: " << SDL_GetError() << std::endl;
        return false;
    }
    // Thiết lập màu vẽ mặc định
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    // Khởi tạo SDL_image để load ảnh PNG
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image không thể khởi tạo: " << IMG_GetError() << std::endl;
        return false;
    }
    // Khởi tạo SDL_mixer
if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    std::cerr << "SDL_mixer không thể khởi tạo: " << Mix_GetError() << std::endl;
    return false;
}

// Load âm thanh bắn đạn
shootSound = Mix_LoadWAV("assets/shoot.wav");
if (!shootSound) {
    std::cerr << "Lỗi load âm thanh bắn: " << Mix_GetError() << std::endl;
    return false;
}
Bullet::loadSound();

    // Load texture cho map
    if (!map.loadTextures(renderer)) {
        std::cerr << "Không thể load texture cho map!" << std::endl;
        return false;
    }
    // Tạo xe tăng người chơi (phải làm trước khi load texture)
    spawnPlayer();  // Hàm này sẽ khởi tạo player

    // Load texture cho xe tăng người chơi
    if (!player || !player->loadTexture(renderer)) {
        std::cerr << "Không thể load texture cho xe tăng người chơi!" << std::endl;
        return false;
    }
    // Tạo xe tăng địch
    spawnEnemies();
    if (!gameScreens.loadTextures(renderer)) {
        std::cerr << "Không thể load texture cho GameScreens!" << std::endl;
        return false;
    }
    // Thiết lập chế độ blend cho texture (nếu cần)
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    running = true;
    gameState = GameState::START_SCREEN;
    return true;
}
void Game::loadMap(const std::string& filename) {
    map.loadMap(filename.c_str());
    spawnPlayer(); // Tìm vị trí trống để sinh xe tăng
}

void Game::spawnPlayer() {
    // Tìm một ô trống để đặt xe tăng
    for (int i = 0; i < map.getHeight(); i++) {
        for (int j = 0; j < map.getWidth(); j++) {
            if (map.getTile(j, i) == Map::EMPTY) { // Ô trống
                player = new PlayerTank(j * Map::TILE_SIZE, i * Map::TILE_SIZE, renderer, map);
                return;
            }
        }
    }
    std::cerr << "Không tìm thấy vị trí trống để đặt xe tăng!" << std::endl;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_RETURN: // Nhấn Enter
                    if (gameState == GameState::START_SCREEN) {
                        gameState = GameState::PLAYING;
                    }
                    break;
                case SDLK_UP:
                    player->move(0, map);  // Di chuyển lên
                    SDL_Log("Hướng: Lên (0)");
                    break;
                case SDLK_RIGHT:
                    player->move(1, map);  // Di chuyển phải
                    SDL_Log("Hướng: Phải (1)");
                    break;
                case SDLK_DOWN:
                    player->move(2, map);  // Di chuyển xuống
                    SDL_Log("Hướng: Xuống (2)");
                    break;
                case SDLK_LEFT:
                    player->move(3, map);  // Di chuyển trái
                    SDL_Log("Hướng: Trái (3)");
                    break;
                case SDLK_SPACE:
                    player->shoot(bullets, renderer);
                    SDL_Log("Bắn đạn");
                    break;
            }
        }
    }
}


void Game::update() {
    if (gameState != GameState::PLAYING) return;
     // 1. Kiểm tra player có đang ở ô nước không
    int playerTileX = player->getX() / Map::TILE_SIZE;
    int playerTileY = player->getY() / Map::TILE_SIZE;
    if (map.getTile(playerTileX, playerTileY) == Map::WATER) {
        gameState = GameState::LOSE_SCREEN;
        SDL_Log("Game Over: Player stepped in water");
        return;
    }
    // Cập nhật xe tăng người chơi
    player->update(map, bullets);

    // Cập nhật đạn của người chơi và xe tăng địch
    for (int i = bullets.size() - 1; i >= 0; i--) {
        bullets[i]->update(map);

        // Kiểm tra va chạm giữa đạn và xe tăng địch (nếu đạn là của người chơi)
        if (bullets[i]->isPlayer()) { // Đạn của người chơi
            for (int j = enemyTanks.size() - 1; j >= 0; j--) {
                if (bullets[i]->checkCollision(enemyTanks[j]->getX(), enemyTanks[j]->getY(),Map::TILE_SIZE, Map::TILE_SIZE)) {
                    // Xóa xe tăng địch
                    delete enemyTanks[j];
                    enemyTanks.erase(enemyTanks.begin() + j);

                    // Xóa đạn
                    delete bullets[i];
                    bullets.erase(bullets.begin() + i);
                    break;
                }
            }
        } else { // Đạn của xe tăng địch
            // Kiểm tra va chạm giữa đạn và xe tăng người chơi
            if (bullets[i]->checkCollision(player->getX(), player->getY(),Map::TILE_SIZE, Map::TILE_SIZE)) {
                // Xử lý khi xe tăng người chơi bị bắn trúng
                gameState = GameState::LOSE_SCREEN;
                delete bullets[i];
                bullets.erase(bullets.begin() + i);
                return;
            }
        }

        // Xóa đạn nếu không còn hoạt động
        if (!bullets[i]->isActive()) {
            delete bullets[i];
            bullets.erase(bullets.begin() + i);
        }
    }

    // Cập nhật xe tăng địch
    for (auto& enemy : enemyTanks) {
        enemy->update(map, bullets);
    }
    // Kiểm tra nếu tất cả xe tăng địch đã bị tiêu diệt
    if (enemyTanks.empty()) {
        gameState = GameState::WIN_SCREEN;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (Bullet* bullet : bullets) {
        bullet->render();
    }
    for (auto& enemy : enemyTanks) {
    enemy->render();
    }
    switch (gameState) {
        case GameState::START_SCREEN:
            gameScreens.render(renderer, 0); // 0 là MENU_SCREEN
            break;
        case GameState::PLAYING:
            map.render(renderer);
            player->render();
            for (Bullet* bullet : bullets) {
                bullet->render();
            }
            for (auto& enemy : enemyTanks) {
                enemy->render();
            }
            break;
        case GameState::WIN_SCREEN:
            gameScreens.render(renderer, 3); // 3 là VICTORY
            break;
        case GameState::LOSE_SCREEN:
            gameScreens.render(renderer, 2); // 2 là GAME_OVER
            break;
    }

    SDL_RenderPresent(renderer);

}
void Game::spawnEnemies() {
    std::vector<std::pair<int, int>> emptyTiles;

    // 1️⃣ Tìm tất cả ô trống trong bản đồ
    for (int y = 0; y < map.getHeight(); y++) {
        for (int x = 0; x < map.getWidth(); x++) {
            if (map.getTile(x, y) == Map::EMPTY) {
                emptyTiles.push_back({x, y});
            }
        }
    }

    // 2️⃣ Kiểm tra nếu không đủ 4 ô trống
    if (emptyTiles.size() < 4) {
        printf("Không đủ ô trống để đặt 4 xe tăng địch!\n");
        return;
    }
    // 3️⃣ Xáo trộn danh sách ô trống để chọn vị trí ngẫu nhiên
    std::shuffle(emptyTiles.begin(), emptyTiles.end(), std::mt19937(std::random_device()()));

    // 4️⃣ Chọn 4 ô đầu tiên để đặt xe tăng địch
    for (int i = 0; i < 4; i++) {
        int tileX = emptyTiles[i].first;
        int tileY = emptyTiles[i].second;
        int spawnX = tileX * Map::TILE_SIZE;
        int spawnY = tileY * Map::TILE_SIZE;

        enemyTanks.push_back(new EnemyTank(spawnX, spawnY, renderer));
        printf("Spawn enemy at (%d, %d) in tile (%d, %d)\n", spawnX, spawnY, tileX, tileY); // Debug
    }
}
void Game::clean() {
    gameScreens.clean();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
