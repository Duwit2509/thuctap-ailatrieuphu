#define WIN32_LEAN_AND_MEAN

#ifdef _WIN32
#include <windows.h>
#endif

#include "../include/game.h"
#include "../include/helpers.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <random>
#include <conio.h>
#include <thread>
#include <vector>

using namespace std;

// Ham hien thi cau hoi
void displayQuestion(const Question& q, long long currentPrize, bool used5050, bool usedAskAudience, bool usedCallWiseMan, char selectedOption) {
    clearScreen();
    setColor(14);
    cout << "╔═══════════════ QUYỀN TRỢ GIÚP ═══════════════ ║ ═══════ LỰA CHỌN KHÁC ═══════╗" << endl;
    resetColor();

    // 50:50
    if (used5050) {
    setColor(7); 
    cout << "║       [5] 50:50 (Đã sử dụng)                  ║";
    } else {
    setColor(10); 
    cout << "║       [5] 50:50                               ║";
    }
    resetColor(); 
    // Dung cuoc choi
    setColor(12); 
    cout << "      [S] Dừng cuộc chơi     " << endl;
    resetColor();

    // Hoi khan gia
    if (usedAskAudience) {
    setColor(7); 
    cout << "║       [6] Hỏi khán giả (Đã sử dụng)           ║                " << endl;
    } else {
    setColor(10); 
    cout << "║       [6] Hỏi khán giả                        ║                " << endl;
    }
    resetColor();

    // Goi dien thoai
    if (usedCallWiseMan) {
    setColor(7); 
    cout << "║       [7] Gọi điện thoại (Đã sử dụng)         ║                " << endl;
    } else {
    setColor(10); 
    cout << "║       [7] Gọi điện thoại                      ║                " << endl;
    }
    resetColor();

    cout << "╚══════════════════════════════════════════════ ╩ ═════════════════════════════╝" << endl << endl;
                                                              
    setColor(14); 
    cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << endl;

     // Tieu de cau hoi va tri gia hien tai
    string header = "CÂU SỐ " + to_string(q.level) + "  -  TRỊ GIÁ: " + formatMoney(currentPrize) + " VND";

    // Tinh toan can le giua
    int boxWidth = 78;
    int paddingLeft = (boxWidth - header.length()) / 2;
    int paddingRight = boxWidth - header.length() - paddingLeft;
                
    setColor(14);
    cout << "║ " << string(paddingLeft, ' ');
    setColor(11);
    cout << header;
    setColor(14);
    cout << string(paddingRight, ' ')  << endl;
    cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << endl;
    

    // Hien thi cau hoi voi ngat dong
    int maxWidth = 90; // Chieu rong toi da cho cau hoi
    string wrapped = wrapText(q.questionText, maxWidth);

    stringstream ss(wrapped);
    string line;
    while (getline(ss, line)) {
        cout << "║ " << line;
        
        //Tinh khoang trang de can le phai
        int pad = maxWidth - line.length();
        while (pad-- > 0) cout << " ";

        cout << " " << endl;
    }

    cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << endl;
    resetColor(); 

    // Hien thi cac dap an
    for (int i = 0; i < 4; ++i) {
        char currentOptionChar = 'A' + i; 
        
        //logic danh dau dap an da chon
        if (currentOptionChar == selectedOption) {
            setColor(13); 
        } else {
            resetColor(); 
        }

        cout << "║ " << q.options[i]; 
        
        resetColor(); // Reset lại ngay để không bị lem màu ra ngoài
        cout << endl;
    }

    setColor(14);
    cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << endl;
    resetColor();
}

//Ham thuc hien tro giup 50:50
void apply5050(Question& currentQuestion) {
    // In ra de nguoi choi biet chuc nang dang chay
    setColor(14); // Mau vang
    cout << "   Đang loại bỏ 2 phương án sai..." << endl;
    resetColor();
    
    // Phat am thanh va doi 1.5 giay
    playApply5050Sound();
    sleep(1500);

    char correctAnswerChar = currentQuestion.answer[0]; 
    vector<int> incorrectIndices; 
    
    for (int i = 0; i < 4; ++i) {
        char optionChar = 'A' + i; 
        if (optionChar != correctAnswerChar) {
            incorrectIndices.push_back(i); 
        }
    }

    // Xao tron danh sach dap an sai
    static random_device rd;
    static mt19937 gen(rd());
    shuffle(incorrectIndices.begin(), incorrectIndices.end(), gen);

    // In ra 2 dap an sai dau tien
    int indexToHide1 = incorrectIndices[0];
    int indexToHide2 = incorrectIndices[1];

    currentQuestion.options[indexToHide1] = string(1, 'A' + indexToHide1) + ". ______";
    currentQuestion.options[indexToHide2] = string(1, 'A' + indexToHide2) + ". ______";
}

// Ham thuc hien tro giup Hoi y kien khan gia 
void applyAskAudience(const Question& currentQuestion) {
    clearScreen();
    setColor(14); 
    cout << "\n==============================================" << endl;
    cout << "       ĐANG LẤY Ý KIẾN KHÁN GIẢ..." << endl;
    cout << "==============================================\n" << endl;
    resetColor();

    playApplyAskAudienceSound();
    sleep(2000); 

    //Logic tính toán phần trăm
    int totalPercent = 100;
    vector<int> percents(4); 

   // Xác định index đáp án đúng
    // Giả sử answer là string "A" -> lấy ký tự đầu tiên
    char correct = currentQuestion.answer[0]; 
    int correctIndex = correct - 'A'; 

    static random_device rd;
    static mt19937 gen(rd());

    // Phân bổ phần trăm
    uniform_int_distribution<> dis(40, 60);
    percents[correctIndex] = dis(gen);
    
    int remaining = totalPercent - percents[correctIndex];

    // Chia phần trăm còn lại cho 3 đáp án sai
    int wrong1 = gen() % remaining;
    int wrong2 = gen() % (remaining - wrong1);
    int wrong3 = remaining - wrong1 - wrong2;

    // Lưu phần trăm vào vector
    vector<int> wrongPercents = {wrong1, wrong2, wrong3};

    // Gán phần trăm cho các đáp án sai
    int j = 0;
    for (int i = 0; i < 4; ++i) {
        if (i != correctIndex) {
            percents[i] = wrongPercents[j];
            j++;
        }
    }
    
    //Hien thi ket qua khan gia
    setColor(14);
    cout << "╔════════ KẾT QUẢ KHẢO SÁT KHÁN GIẢ ════════╗" << endl;
    resetColor();
    // Dinh nghia chieu rong cua thanh do
    const int MAX_BAR_WIDTH = 20;

    // In ra tung phan tram voi thanh do
    for (int i = 0; i < 4; ++i) {
        cout << "║ " << (char)('A' + i) << ": " << setw(3) << percents[i] << "% ";
        setColor(10);
        cout << "[";
        // (Lay % * 20) / 100
        int barWidth = (percents[i] * MAX_BAR_WIDTH) / 100;
        for (int k = 0; k < barWidth; ++k) {
            cout << "=";
        }
        for (int k = 0; k < MAX_BAR_WIDTH - barWidth; ++k) {
            cout << " ";
        }
        cout << "]";
        resetColor();
        cout << endl;
    }
    cout << "╚═══════════════════════════════════════════╝" << endl;
    
    // In ra đáp án được chọn nhiều nhất
    int maxP = 0;
    int maxIdx = 0;
    for(int i=0; i<4; i++) {
        if(percents[i] > maxP) { maxP = percents[i]; maxIdx = i; }
    }
    cout << "\nĐa số khán giả chọn phương án " << (char)('A' + maxIdx) << " (" << maxP << "%)" << endl;
    pressEnterToContinue(); 
}

// Hàm thực hiện quyền trợ giúp: Gọi Nhà Thông Thái
void applyCallWiseMan(const Question& currentQuestion) {
    clearScreen();
    
    // Tạo hiệu ứng đang kết nối
    setColor(14); // Màu vàng
    cout << "\n==============================================" << endl;
    cout << "   ĐANG KẾT NỐI VỚI TỔ TƯ VẤN TẠI CHỖ..." << endl;
    cout << "==============================================\n" << endl;
    resetColor();

    playApplyCallWiseMan();

    cout << "..." << endl;
    sleep(1000); // Tạm dừng 1s
    cout << "..." << endl;
    sleep(1000);

    // Bắt đầu hội thoại
    setColor(11); // Màu xanh dương nhạt (Nhà thông thái)
    cout << "\n[Nhà Thông Thái]: 'Alo, tôi nghe đây! Có câu hỏi khó cần trợ giúp phải không?'" << endl;
    sleep(2000);

    setColor(15); // Màu trắng (Người chơi)
    cout << "\n[Bạn]: (Đọc to nội dung câu hỏi: " << currentQuestion.questionText << "...)" << endl;
    sleep(3000); // Giả lập thời gian đọc câu hỏi

    setColor(11);
    cout << "\n[Nhà Thông Thái]: 'Hừm... Đợi tôi tra cứu dữ liệu một chút...'" << endl;
    sleep(2000);
    cout << "\n[Nhà Thông Thái]: 'Câu này thuộc lĩnh vực này tôi cũng có nghiên cứu...'" << endl;
    sleep(1500);
    cout << "\n[Nhà Thông Thái]: 'Theo phân tích của tôi thì đáp án là... ";
    sleep(2000); // Tạo hồi hộp

    // --- LOGIC XỬ LÝ TỈ LỆ ĐÚNG/SAI ---
    // Cơ chế: Nhà thông thái không phải lúc nào cũng đúng.
    // - Tỉ lệ đúng: 80%
    // - Tỉ lệ sai: 20% (Để troll người chơi)
    
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100); 

    char finalAnswer; // Đáp án nhà thông thái sẽ chọn

    // Nếu random ra số <= 80 -> Chọn đáp án ĐÚNG
    if (dis(gen) <= 80) { 
        // Lấy ký tự đầu tiên của chuỗi đáp án đúng (Ví dụ "A" -> 'A')
        finalAnswer = currentQuestion.answer[0];
    } 
    // Nếu random ra số > 80 -> Chọn đáp án SAI (Random 1 trong 3 câu sai)
    else {
        vector<char> wrongOptions;
        for (int i = 0; i < 4; ++i) {
            char opt = 'A' + i; // Tạo 'A', 'B', 'C', 'D'
            // Nếu đáp án này khác đáp án đúng -> Thêm vào danh sách sai
            if (opt != currentQuestion.answer[0]) {
                wrongOptions.push_back(opt);
            }
        }
        // Lấy ngẫu nhiên 1 đáp án trong danh sách sai
        uniform_int_distribution<> dis_wrong(0, wrongOptions.size() - 1);
        finalAnswer = wrongOptions[dis_wrong(gen)];
    }

    // In ra đáp án chốt
    setColor(14); 
    cout << finalAnswer << " !'" << endl;
    
    setColor(11);
    cout << "\n[Nhà Thông Thái]: 'Tôi tin chắc 90% là phương án này. Chúc bạn may mắn!'" << endl;
    resetColor();
    
    cout << "\n----------------------------------------------" << endl;
    cout << "Cuộc gọi kết thúc..." << endl;

    pressEnterToContinue();
}

//Hàm nhận diện phím bấm với giới hạn thời gian
char getUserInputWithTimer(int &timeLeft) {
    // Xóa bộ đệm phím trước khi bắt đầu
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    cout << endl; 

    while (timeLeft > 0) {
        setColor(12); 
        // \r de tro ve dau dong
        cout << "\r   >> THỜI GIAN CÒN LẠI: " << setw(2)  << timeLeft << " GIÂY <<   ";
        resetColor();

        // Kiểm tra nếu có phím bấm
        for (int i = 0; i < 10; i++) {
            if (_kbhit()) { // Nếu có phím bấm
                char ch = _getch(); // Lấy ký tự phím bấm
                ch = toupper(ch);  // Chuyển thành chữ hoa để dễ so sánh
                
                // Kiểm tra nếu phím bấm hợp lệ 
                if ((ch >= 'A' && ch <= 'D') || ch == '5' || ch == '6' || ch == '7' || ch == 'S') {
                    cout << "\n"; 
                    return ch;    
                }
            }
            Sleep(95); // Đợi 95ms trước khi kiểm tra lại
        }

        if (timeLeft <= 5) {
             Beep(800, 50); 
        } else {
             Beep(600, 30); 
        }

        timeLeft--;
    }

    cout << "\n";
    return 'X'; 
}

// Ham hien thi luat choi
void viewGameRules() {
    clearScreen();
    playPalpitatingSound();
    setColor(14);
    cout << "╔══════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                      HƯỚNG DẪN LUẬT CHƠI                             ║" << endl;
    cout << "╚══════════════════════════════════════════════════════════════════════╝" << endl;
    
    setColor(11);
    cout << "\n[1] MỤC TIÊU:" << endl;
    resetColor();
    cout << "\n   - Bạn phải vượt qua 15 câu hỏi trắc nghiệm từ dễ đến khó." << endl;
    cout << "\n   - Mỗi câu hỏi có 4 đáp án (A, B, C, D) và chỉ có 1 đáp án đúng." << endl;
    cout << "\n   - Thời gian suy nghĩ cho mỗi câu là "; setColor(12); cout << "30 giây"; resetColor(); cout << "." << endl;

    setColor(11);
    cout << "\n[2] MỐC QUAN TRỌNG:" << endl;
    resetColor();
    cout << "\n   - Có 3 mốc quan trọng: "; setColor(14); cout << "Câu 5 - Câu 10 - Câu 15"; resetColor(); cout << "." << endl;
    cout << "\n   - Ý nghĩa: Khi đã vượt qua các mốc này, nếu bạn trả lời sai ở" << endl;
    cout << "\n     các câu tiếp theo, bạn sẽ không bị mất hết tiền mã sẽ ra về" << endl;
    cout << "\n     với số tiền thưởng của mốc quan trọng gần nhất." << endl;
    setColor(8);
    cout << "\n     * Ví dụ: Trả lời sai câu 7 -> Nhận tiền thưởng Câu 5." << endl;
    cout << "     * Ví dụ: Trả lời sai câu 12 -> Nhận tiền thưởng Câu 10." << endl;
    resetColor();

    setColor(11);
    cout << "\n[3] QUYỀN TRỢ GIÚP (Chỉ dùng 1 lần duy nhất):" << endl;
    resetColor();
    cout << "   "; setColor(10); cout << "\n     [5] 50:50"; resetColor(); cout << ": Máy tính sẽ loại bỏ 2 phương án sai." << endl;
    cout << "   "; setColor(10); cout << "\n     [6] Hỏi ý kiến khán giả"; resetColor(); cout << ": Xem biểu đồ khảo sát từ khán giả." << endl;
    cout << "   "; setColor(10); cout << "\n     [7] Gọi điện thoại"; resetColor(); cout << ": Hỏi ý kiến nhà thông thái." << endl;

    setColor(11);
    cout << "\n[4] DỪNG CUỘC CHƠI:" << endl;
    resetColor();
    cout << "\n   - Nhấn phím "; setColor(12); cout << "[S]"; resetColor(); cout << " để dừng cuộc chơi bất cứ lúc nào." << endl;
    cout << "\n   - Khi dừng cuộc chơi, bạn sẽ bảo toàn "; setColor(14); cout << "100%"; resetColor(); cout << " số tiền hiện có." << endl;

    setColor(11);
    cout << "\n[5] THẮNG TIỀN THƯỞNG:" << endl;
    resetColor();
    cout << "\n   Câu 1: 200.000       Câu 6: 3.000.000       Câu 11: 30.000.000" << endl;
    cout << "   Câu 2: 400.000       Câu 7: 6.000.000       Câu 12: 40.000.000" << endl;
    cout << "   Câu 3: 600.000       Câu 8: 10.000.000      Câu 13: 60.000.000" << endl;
    cout << "   Câu 4: 1.000.000     Câu 9: 14.000.000      Câu 14: 85.000.000" << endl;
    setColor(14);
    cout << "   CÂU 5: 2.000.000     CÂU 10: 22.000.000     CÂU 15: 150.000.000" << endl;
    resetColor();

    setColor(11);
    cout << "\n[6] HỆ THỐNG DANH HIỆU (TÍCH LŨY):" << endl;
    resetColor();
    cout << "\n   Số tiền bạn kiếm được sẽ cộng dồn vào tài khoản cá nhân." << endl;
    cout << "\n   Tích lũy càng nhiều, danh hiệu của bạn càng cao:" << endl;
    
    cout << "\n   - Dưới 1 Tr:         "; setColor(8); cout << "Tân binh nghèo khổ" << endl; resetColor();
    cout << "   - 1 Tr - 10 Tr:      "; setColor(7); cout << "Người chơi tiềm năng" << endl; resetColor();
    cout << "   - 10 Tr - 50 Tr:     "; setColor(10); cout << "Triệu phú" << endl; resetColor();
    cout << "   - 50 Tr - 200 Tr:    "; setColor(11); cout << "Đại gia" << endl; resetColor();
    cout << "   - 200 Tr - 1 Ty:     "; setColor(13); cout << "Tỷ phú" << endl; resetColor();
    cout << "   - Trên 1 Ty:         "; setColor(14); cout << "VUA TIỀN TỆ" << endl; resetColor();

    cout << "\n----------------------------------------------------------------------" << endl;
    pressEnterToContinue();
}