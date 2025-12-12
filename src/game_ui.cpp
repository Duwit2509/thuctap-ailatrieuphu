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
void displayQuestion(const Question& q, long long currentPrize, bool used5050, bool usedAskAudience, bool usedPhoneAFriend, char selectedOption) {
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
    if (usedPhoneAFriend) {
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
    char correctAnswerChar = currentQuestion.answer[0]; 
    vector<int> incorrectIndices; 
    
    for (int i = 0; i < 4; ++i) {
        char optionChar = 'A' + i; 
        if (optionChar != correctAnswerChar) {
            incorrectIndices.push_back(i); 
        }
    }

    static random_device rd;
    static mt19937 gen(rd());
    shuffle(incorrectIndices.begin(), incorrectIndices.end(), gen);

    int indexToHide1 = incorrectIndices[0];
    int indexToHide2 = incorrectIndices[1];

    playApply5050Sound();

    currentQuestion.options[indexToHide1] = string(1, 'A' + indexToHide1) + ". ...";
    currentQuestion.options[indexToHide2] = string(1, 'A' + indexToHide2) + ". ...";
}

// Ham thuc hien tro giup Hoi y kien khan gia 
void applyAskAudience(const Question& currentQuestion) {
    clearScreen();
    setColor(14); 
    cout << "Đang lấy ý kiến khán giả...\n" << endl;
    resetColor();

    playApplyAskAudienceSound();
    sleep(2000); 

    int totalPercent = 100;
    vector<int> percents(4); 
    char correct = currentQuestion.answer[0]; 
    int correctIndex = correct - 'A'; 

    static random_device rd;
    static mt19937 gen(rd());

    uniform_int_distribution<> dis(40, 70);
    percents[correctIndex] = dis(gen);
    
    int remaining = totalPercent - percents[correctIndex];
    
    int wrong1 = gen() % remaining;
    int wrong2 = gen() % (remaining - wrong1);
    int wrong3 = remaining - wrong1 - wrong2;
    
    vector<int> wrongPercents = {wrong1, wrong2, wrong3};
    
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

    const int MAX_BAR_WIDTH = 20; 

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
    
    Beep(1000, 200);
    pressEnterToContinue(); 
}

// Ham thuc hien tro giup Goi dien thoai 
void applyPhoneAFriend(const Question& currentQuestion) {
    clearScreen();
    setColor(14); 
    cout << "Đang kết nối với  'Nhà Thông Thái'..." << endl;
    resetColor();

    playApplyPhoneAFriendSound();

    cout << "..." << endl;
    sleep(1000);
    setColor(8);
    cout << "Nhà Thông Thái: 'Alo, tôi nghe!'" << endl;
    sleep(1500);
    setColor(11);
    cout << "Ban: (Đọc câu hỏi...)" << endl;
    sleep(2000);
    setColor(8);
    cout << "Nhà Thông Thái: 'Hmm... câu này...'" << endl;
    sleep(1500);
    cout << "Nhà Thông Thái: 'Tôi nghĩ... Đáp án là... ";
    sleep(2000);

    // Logic 80% dung, 20% sai
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100); 

    char finalAnswer;

    if (dis(gen) <= 80) { 
        finalAnswer = currentQuestion.answer[0];
    } else {
        vector<char> wrongOptions;
        for (int i = 0; i < 4; ++i) {
            char opt = 'A' + i;
            if (opt != currentQuestion.answer[0]) {
                wrongOptions.push_back(opt);
            }
        }
        // Lay ngau nhien 1 trong 3 dap an sai
        uniform_int_distribution<> dis_wrong(0, wrongOptions.size() - 1);
        finalAnswer = wrongOptions[dis_wrong(gen)];
    }

    // In ra dap an
    setColor(10); 
    cout << finalAnswer << " !'" << endl;
    setColor(14);
    cout << "Nhà Thông Thái: 'Tôi khá chắc chắn... nhưng quyết định là ở bạn. GOOD LUCK!'" << endl;
    resetColor();
    cout << "Tít... tít... (Kết thúc cuộc gọi)" << endl;

    pressEnterToContinue();
}

//Ham nhan dien phim bam co gioi han thoi gian
char getUserInputWithTimer(int &timeLeft) {
    // Xoa bo nho dem
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    cout << endl; 

    while (timeLeft > 0) {
        setColor(12); 
        cout << "\r   >> Thời gian còn lại: " << setw(2) << timeLeft << " giây <<   ";
        resetColor();

        //Check phim bam
        for (int i = 0; i < 10; i++) {
            if (_kbhit()) { 
                char ch = _getch(); 
                ch = toupper(ch);  
                
                if ((ch >= 'A' && ch <= 'D') || ch == '5' || ch == '6' || ch == '7' || ch == 'S') {
                    cout << "\n"; 
                    return ch;    
                }
            }
            Sleep(100); 
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