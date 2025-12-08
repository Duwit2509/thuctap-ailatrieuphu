Game Ai Là Triệu Phú (Console C++)
Project Thực tập Cơ sở - Mô phỏng gameshow "Ai Là Triệu Phú" kinh điển với giao diện Console sống động, âm thanh thực tế và logic chuẩn xác.

Tính Năng Nổi Bật
Phiên bản nâng cấp này mang đến trải nghiệm người dùng vượt trội so với các ứng dụng console thông thường:

1. Gameplay & Logic Chặt Chẽ
Hệ thống 15 câu hỏi: Độ khó tăng dần, bao gồm các mốc quan trọng (Câu 5, 10, 15).

Tiền thưởng thực tế: Mô phỏng thang tiền thưởng từ 200.000 VNĐ đến 150.000.000 VNĐ.

Áp lực thời gian: Đồng hồ đếm ngược 30 giây cho mỗi câu hỏi. Hết giờ tự động xử lý thua cuộc.

Dừng cuộc chơi: Người chơi có thể chọn [S] để dừng lại và bảo toàn số tiền hiện có.

2. Hệ Thống Trợ Giúp
[5] 50:50: Loại bỏ ngay lập tức 2 phương án sai.

[6] Hỏi ý kiến khán giả: Hiển thị biểu đồ phần trăm bình chọn trực quan.

[7] Gọi điện thoại cho người thân: Kết nối với "Nhà thông thái" (có tỷ lệ đúng/sai logic).

3. Trải Nghiệm Nghe Nhìn (Audio & Visual)
Giao diện Neon: Sử dụng bảng màu nâng cao của Windows API.

Top 1: Vàng Gold (14)

Top 2: Xanh Ngọc Neon (11)

Top 3: Tím Hồng (13)

Hiệu ứng chọn đáp án: Ô đáp án chuyển màu khi được chọn -> Tạo cảm giác hồi hộp trước khi công bố kết quả.

Âm thanh sống động: Tích hợp hệ thống âm thanh WinMM:

Nhạc nền hồi hộp, tiếng tim đập khi đếm ngược.

Hiệu ứng âm thanh riêng cho từng quyền trợ giúp.

Nhạc chiến thắng, nhạc thua cuộc, nhạc hiệu chương trình.

4. Hệ Thống Quản Trị (Admin) & Dữ Liệu
Đăng nhập bảo mật: Cần mật khẩu để truy cập quyền Admin.

Quản lý câu hỏi:

Xem danh sách, Thêm mới, Xóa câu hỏi.

Tự động sắp xếp câu hỏi theo Level.

Lưu trữ JSON: Dữ liệu câu hỏi và bảng xếp hạng được lưu vĩnh viễn trong file .json.

 Bảng Xếp Hạng: Lưu trữ Tên, Số tiền và Level câu hỏi cao nhất đạt được.

Công Nghệ Sử Dụng
Ngôn ngữ: C++ (Chuẩn C++17).

Trình biên dịch: MinGW-w64 (g++).

Thư viện:

nlohmann/json: Xử lý dữ liệu JSON.

Windows API: Xử lý màu sắc, âm thanh (PlaySound, Beep), input (FlushConsoleInputBuffer).

Standard Library: vector, thread, chrono, fstream, algorithm...

Cấu Trúc Dự Án
Dự án được tổ chức theo mô hình tách biệt mã nguồn (Separation of Concerns):

```text
THUCTAP-AILATRIEUPHU/
│
├── include/                # Chứa các file header (.h)
│   ├── game.h              # Struct Player, Question & khai báo hàm game
│   ├── admin.h             # Các hàm quản lý Admin
│   ├── helpers.h           # Hàm hỗ trợ (màu sắc, âm thanh, UI)
│   ├── json_handler.h      # Xử lý đọc/ghi file JSON
│   └── json.hpp            # Thư viện JSON (nlohmann)
│
├── src/                    # Chứa mã nguồn (.cpp)
│   ├── main.cpp            # Hàm main, Menu chính
│   ├── game.cpp            # Logic luồng game chính
│   ├── game_ui.cpp         # Xử lý hiển thị, vẽ khung, hiệu ứng
│   ├── admin.cpp           # Logic quản trị viên
│   ├── helpers.cpp         # Triển khai các hàm tiện ích
│   └── json_handler.cpp    # Triển khai đọc/ghi file
│
├── sounds/                 # Thư mục chứa file âm thanh .wav
│   ├── intro.wav, select.wav, correct.wav...
│
├── questions.json          # Cơ sở dữ liệu câu hỏi
├── scores.json             # Dữ liệu bảng xếp hạng
├── build.bat               # Script biên dịch tự động
└── README.md

Hướng Dẫn Cài Đặt & Chạy
Cách 1: Chơi Ngay (User)
Tải file .zip từ mục Releases.

Giải nén ra thư mục.

Quan trọng: Đảm bảo thư mục sounds/ và file questions.json nằm cùng cấp với file .exe.

Chạy ailatrieuphu.exe và thưởng thức!

Cách 2: Tự Biên Dịch (Developer)
Yêu cầu: Đã cài đặt MinGW (g++) hỗ trợ C++17.

Clone repository:

Bash

git clone https://github.com/Duwit2509/thuctap-ailatrieuphu.git
Chạy script biên dịch (Windows): Nhấn đúp vào file build.bat hoặc chạy lệnh trong terminal:

DOS

.\build.bat
Nếu báo Build successful!, game sẽ tự động chạy.

Hướng Dẫn Chơi
Sử dụng bàn phím để tương tác:

A, B, C, D: Chọn đáp án tương ứng.

5, 6, 7: Sử dụng quyền trợ giúp.

S: Dừng cuộc chơi (Stop).

Admin Password mặc định: 2509

Tác Giả
Trần Đức Việt
MSSV: 65134312
Github: Duwit2509