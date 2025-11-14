Game Ai Là Triệu Phú (Console C++)
Đây là project môn Thực tập Cơ sở, mô phỏng lại gameshow "Ai Là Triệu Phú" kinh điển trên nền tảng console C++. Ứng dụng được xây dựng với giao diện ASCII, màu sắc, và đầy đủ các chức năng chính.



* Tính Năng Nổi Bật
Chương trình đáp ứng đầy đủ các yêu cầu của Mức 1 và Mức 2, đồng thời bổ sung nhiều tính năng nâng cao:

Logic Game Hoàn Chỉnh:

Vượt qua 15 câu hỏi với độ khó tăng dần.

Hiển thị các mốc tiền thưởng quan trọng (5, 10, 15).

Cho phép người chơi [S] Dừng cuộc chơi bất cứ lúc nào để bảo toàn tiền thưởng.

Đầy Đủ 3 Quyền Trợ Giúp:

[5] 50:50: Loại bỏ 2 đáp án sai.

[6] Hỏi ý kiến khán giả: Hiển thị biểu đồ bình chọn (dạng thanh [=== ]).

[7] Gọi điện thoại cho người thân: "Nhà thông thái" đưa ra gợi ý (80% đúng, 20% sai).

Phân Hệ Quản Trị (Admin):

Đăng nhập bằng mật khẩu (adminLogin).

Xem toàn bộ ngân hàng câu hỏi (viewAllQuestions).

Thêm câu hỏi mới (addQuestion) (Hỗ trợ Tiếng Việt có dấu).

Xóa câu hỏi (deleteQuestion).

Tự động sắp xếp file questions.json theo level (1-15) mỗi khi thêm câu hỏi.

Lưu Trữ & Bảng Xếp Hạng:

Người chơi nhập tên trước khi chơi.

Tự động lưu điểm số vào scores.json.

Hiển thị Bảng Xếp Hạng Top 10 người chơi (viewHighScores).

Giao Diện Console (TUI):

Sử dụng Màu sắc (Windows API) để làm nổi bật các chức năng.

Dùng ký tự ASCII mở rộng (UTF-8) ╔═╗ để vẽ khung thân thiện.

Sử dụng sleep để tạo hiệu ứng chờ, tăng kịch tính.

* Công Nghệ Sử Dụng
Ngôn ngữ: C++ (Sử dụng chuẩn C++17).

Trình biên dịch: g++ (MinGW-w64).

Thư viện:

JSON for Modern C++ (json.hpp): Dùng để đọc và ghi dữ liệu câu hỏi/điểm số từ file .json.

Standard Library (STL): vector, string, fstream, random, algorithm...

Windows API: Sử dụng SetConsoleOutputCP(CP_UTF8), SetConsoleCP(CP_UTF8) và SetConsoleTextAttribute để xử lý Tiếng Việt, màu sắc, và ký tự đặc biệt.

* Cách Chạy (Hướng Dẫn)
Cách 1: Chơi Ngay (Cho Người Dùng)
Tải file Game_Ai_La_Trieu_Phu.zip từ mục Releases.

Giải nén file .zip.

Chạy file game.exe để chơi.

(Lưu ý: Đảm bảo 2 file questions.json và scores.json luôn nằm cạnh file game.exe).

Cách 2: Tự Biên Dịch (Cho Lập Trình Viên)
Clone repo:

Bash

git clone https://github.com/Duwit2509/thuctap-ailatrieuphu.git
Yêu cầu:

Trình biên dịch g++ hỗ trợ C++17.

Thư viện json.hpp (đã có sẵn trong repo).

Biên dịch: Mở Terminal và gõ lệnh:

Bash

g++ main.cpp -o game.exe -std=c++17
Chạy:

Bash

./game.exe
* Cấu Trúc File
.
├── main.cpp          # File code C++ chinh, chua toan bo logic
├── json.hpp          # Thu vien JSON (khong can cai dat)
├── questions.json    # Ngan hang cau hoi (Co the sua truc tiep)
├── scores.json       # Luu bang xep hang (Tu dong tao)
├── game.exe          # File chay (sau khi bien dich)
└── README.md         # File gioi thieu 
* Tác Giả
Trần Đức Việt - 65134312
