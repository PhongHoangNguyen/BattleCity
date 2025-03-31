    #include "PlayerTank.h"
    #include <SDL2/SDL_image.h>
    #include <cstdio>

    PlayerTank::PlayerTank(int startX, int startY, SDL_Renderer* renderer, const Map& map)
        : x(startX), y(startY), speed(SPEED), direction(0), renderer(renderer), hasMoved(false) {

        SDL_Surface* tempSurface = IMG_Load("assets/player_tank.png");
        if (!tempSurface) {
            SDL_Log("Lỗi load ảnh xe tăng: %s", IMG_GetError());
            texture = nullptr;
        } else {
            texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
            SDL_FreeSurface(tempSurface);
        }
    }
    bool PlayerTank::loadTexture(SDL_Renderer* renderer) {
        SDL_Surface* surface = IMG_Load("assets/player_tank.png");
        if (!surface) {
            SDL_Log("Lỗi load texture xe tăng: %s", IMG_GetError());
            return false;
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            SDL_Log("Lỗi tạo texture từ surface: %s", SDL_GetError());
            return false;
        }

        return true;
    }

    PlayerTank::~PlayerTank() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }


    void PlayerTank::move(int newDirection, const Map& map) {
        // Tính toán vị trí tiếp theo dựa trên hướng mới
        int dx = 0, dy = 0;
        switch (newDirection) {
            case 0: dy = -1; break;  // Lên
            case 1: dx = 1; break;   // Phải
            case 2: dy = 1; break;   // Xuống
            case 3: dx = -1; break;  // Trái
        }

        int tileX = x / Map::TILE_SIZE;
        int tileY = y / Map::TILE_SIZE;
        int nextTileX = tileX + dx;
        int nextTileY = tileY + dy;

        // Kiểm tra xem vị trí tiếp theo có hợp lệ không
        if (nextTileX < 0 || nextTileX >= map.getWidth() || nextTileY < 0 || nextTileY >= map.getHeight()) {
            printf("Xe tăng muốn đi ra ngoài bản đồ! (%d, %d) -> Dừng di chuyển\n", nextTileX, nextTileY);
            return; // Không di chuyển nếu ra khỏi bản đồ
        }

        int tileType = map.getTile(nextTileX, nextTileY);

        if (tileType == Map::WALL || tileType == Map::GRASS) {
            printf("Ô tiếp theo (%d, %d) bị cản bởi tileType = %d -> Đổi hướng tại chỗ\n", nextTileX, nextTileY, tileType);
            direction = newDirection; // Đổi hướng tại chỗ
            return; // Không di chuyển nếu gặp vật cản
        }

        // Nếu không gặp vật cản, di chuyển một ô
        x = nextTileX * Map::TILE_SIZE;
        y = nextTileY * Map::TILE_SIZE;
        direction = newDirection; // Cập nhật hướng sau khi di chuyển
        printf("Xe tăng di chuyển đến (%d, %d)\n", nextTileX, nextTileY);
    }

    void PlayerTank::update(const Map& map,std::vector<Bullet*>& bullets) {
        static bool hasMoved = false; // Biến cờ kiểm tra xe tăng đã di chuyển chưa

        // Nếu xe tăng chưa di chuyển, cho phép di chuyển một lần
        if (!hasMoved) {
            move(direction, map);
            hasMoved = true; // Đánh dấu đã di chuyển
        }
    }


    void PlayerTank::render() {
        SDL_Rect rect = { x, y, SIZE, SIZE };
        if (texture) {
            SDL_RenderCopy(renderer, texture, nullptr, &rect);
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    void PlayerTank::shoot(std::vector<Bullet*>& bullets, SDL_Renderer* renderer) {
        int bulletX = x;
        int bulletY = y;

        // Điều chỉnh vị trí đạn xuất phát để tránh đạn bắn ra từ giữa xe tăng
        const int offset = Map::TILE_SIZE / 2; // Canh giữa xe tăng

        switch (direction) {
            case 0: bulletY -= offset; break; // Bắn lên
            case 1: bulletX += offset; break; // Bắn phải
            case 2: bulletY += offset; break; // Bắn xuống
            case 3: bulletX -= offset; break; // Bắn trái
        }

        bullets.push_back(new Bullet(bulletX, bulletY, direction, renderer, true));
    }

    void PlayerTank::setDirection(int dir) {
        direction = dir;
    }
