# README - Tank battle

## Thông Tin Tác Giả
- Họ và tên: Nguyễn Hoàng Phong 
- Mã sinh viên: 24020270
- ## Giới Thiệu Về Game 
Game dựa trên ý tưởng của game Battle city.Trong game, người chơi điều khiển xe tăng của mình tiêu diệt hết xe tăng kẻ địch thì sẽ giành chiến thắng.
## Ngưỡng Điểm Mong Muốn
- Điểm: ≥8
- **Lý do**:
 - Dành nhiều thời gian tự học và phát triển game.
 - Game tích hợp các tính năng:
   - Có âm thanh, đầy đủ hình ảnh của các nhân vật
   - Game đa dạng về nhân vật
   - Cấu trúc game rõ ràng, dễ hiểu.
## Nguồn Tham Khảo
- **Trang Web**: [Phát Triển Phần Mềm 123AZ](https://phattrienphanmem123az.com/lap-trinh-game-c-p2)
- **Kênh YouTube**: Phát Triển Phần Mềm 123 A-Z
- **Nguồn Ảnh & Âm Thanh**: [Itch.io](https://itch.io/game-assets/free/genre-shooter/tag-space)
                            [FreeSound](https://freesound.org/search/?q=gun+shot)
- **Video Demo Game**: [GoogleDriver]https://drive.google.com/file/d/1VJI-tl3fx8sh9inbUDHaK-PSLPrC_unm/view?usp=sharing
- ## Hỗ Trợ Từ AI
- Debug, tăng tốc game.
- Sửa lỗi chính tả, cú pháp.
- Giải thích chức năng và cách sử dụng lệnh.
- Hướng dẫn GitHub, SDL2.
- Cung cấp build system chạy SDL2 trên Sublime Text.
- Giới thiệu tổ chức file.
## Cách Chơi
Các phím di chuyển của xe tăng : trái, phải, lên và xuống
Chỉ có 1 mạng duy nhất hết mạng sẽ chết(đi vào nước hoặc dính đạn từ xe tăng địch)
Space (Cách): Bắn đạn.
- Mục tiêu:
  - Bắn hết kẻ địch.
## Các Thành Phần Trong Game
- Xe tăng người chơi điều khiển
- Xe tăng kẻ địch : xe tăng AI
- Gạch: không thể bị huỷ,cản đạn và cản xe tăng
- Cỏ : có thể bắn huỷ, cản xe tăng
- Nước : Xe tăng người chơi đi vào sẽ chết
- Ô rỗng : Xe tăng có thể đi qua
Cấu trúc của project game:
Battlecity/  
│   ├── assets/               # Chứa tất cả hình ảnh, âm thanh của game  │  
│── include/                  # Thư mục chứa các file header  
│   ├── Bullet.h         # Xử lý đạn
│   ├── Enemy_tank.h                 # Xử lý xe tăng kẻ địch
│   ├── Game.h                 # Xử lý logic chính của game
│   ├── Gamescreen.h                # Xử lý màn hình trước và sau game 
│   ├── Map.h              # Xử lý bản đồ game
│   ├── Playertank.h             # Xử lý xe tăng của người chơi 
│   ├── Wall.h           # Xử lý tường trong map            
│── src/                      # Thư mục chứa các file định nghĩa  
│   ├── Bullet.cpp  
│   ├── Enemy_tank.cpp  
│   ├── Game.cpp  
│   ├── Gamescreen.cpp  
│   ├── Map.cpp  
│   ├── Playertank.cpp  
│   ├── Wall.cpp  
│   ├── main.cpp  
│──map/ map.csv(nơi vẽ và lưu bản đồ)
│── README.txt                # File giới thiệu game
## Hình Ảnh & Âm Thanh
- **Âm thanh** :có tiếng đạn bắn
- **Đồ hoạ** :2D đơn giản
## Cách Chạy Game
1. Mở Code block
2. Nhấn "Build and run"
