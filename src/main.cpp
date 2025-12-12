#define WIN32_LEAN_AND_MEAN
#ifdef _WIN32
#include <windows.h> 
#endif

#include <iostream>  
#include <vector>
#include <limits>
#include <string>

#include "../include/game.h"
#include "../include/admin.h"
#include "../include/json_handler.h"
#include "../include/helpers.h"

using namespace std;

// Ham lay xep hang theo tien
string getRankTitle(long long money) {
    if (money < 1000000) return "Tân binh nghèo khổ";
    if (money < 10000000) return "Người chơi tiềm năng";
    if (money < 50000000) return "Triệu phú";     
    if (money < 200000000) return "Đại gia";      
    if (money < 1000000000) return "Ty phú";      
    return "VUA TIEN TE";                         
}

int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); 
        SetConsoleCP(CP_UTF8);
    #endif

    // Tai cau hoi va diem so tu file JSON
    vector<Question> allQuestions = loadQuestions();
    vector<Player> allScores = loadScores();

    if (allQuestions.empty()) {
        setColor(12);
        cout << "LỖI NGHIÊM TRỌNG: Không có câu hỏi nào được tải. Kết thúc." << endl;
        resetColor();
        pressEnterToContinue();
    }

    cout << "Nhấn Enter để bắt đầu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    bool isRunning = true;
    
    while (isRunning) {
        playWelcomeSound();
        clearScreen(); 
        setColor(14);
        cout << "╔═══════════════════════════════════════════════╗" << endl;
        cout << "║                ĐĂNG NHẬP TÀI KHOẢN            ║" << endl;
        cout << "╚═══════════════════════════════════════════════╝" << endl;
        resetColor(); 

        string playerName;
        setColor(11);
        cout << "\nNhập tên của bạn để bắt đầu (hoặc 'X' để thoát): ";
        resetColor();
        getline(cin, playerName);
        if (playerName == "x" || playerName == "X") {
            setColor(14);
            cout << "Tạm biệt! Hẹn gặp lại." << endl;
            resetColor();
            break;
        }
        if (playerName.empty()) playerName = "Noname";
        long long myMoney = loadUserMoney(playerName);
        bool isUserSession = true;
        while (isUserSession) {
            clearScreen();
            playIntroSound();
            setColor(14); 
            cout << "╔═════════════════════════════════════════════╗" << endl;
            cout << "║      CHÀO MỪNG ĐẾN VỚI AI LÀ TRIỆU PHÚ      ║" << endl;
            cout << "╚═════════════════════════════════════════════╝" << endl;

            //Thong tin nguoi choi
            setColor(10);
            cout << "\nXin chào: " << playerName << endl;
            setColor(11);
            cout << "Tài sản: " << formatMoney(myMoney) << "VND" << endl;
            cout << "Danh hiệu: ";
            setColor(13);
            cout << getRankTitle(myMoney) << endl;
            resetColor();
            cout << "\n----------------------------------------" << endl;

            cout << "  1. Chơi game" << endl;
            cout << "  2. Xem Bảng Xếp Hạng" << endl;
            cout << "  3. Quản lý câu hỏi (Admin)" << endl;
            cout << "  4. Hướng dẫn & luật chơi" << endl;
            cout << "  5. Đăng xuất" << endl;
            cout << "----------------------------------------" << endl;
            cout << "Nhập lựa chọn của bạn (1-5): ";

            int choice;
            cin >> choice;
            
            while (cin.fail()) {
                cin.clear(); 
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                setColor(12);
                cout << "Lựa chọn không hợp lệ. Vui lòng nhập số (1-5): ";
                resetColor();
                cin >> choice;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            switch (choice) {
                case 1: { 
                    if (allQuestions.empty()) {
                        setColor(12);
                        cout << "Chưa có dữ liệu câu hỏi! Vui lòng vào Admin để thêm." << endl;
                        resetColor();
                        pressEnterToContinue();
                        break;
                    }
                    clearScreen();
                    setColor(14);
                    cout << "\n   Chuẩn bị vào trò chơi..." << endl;
                    cout << "   Chúc bạn may mắn!" << endl;
                    resetColor();
                    
                    sleep(3000);
                    
                    int finalLevel = 0;
                    long long finalScore = playGame(allQuestions, finalLevel);

                    // Cap nhat so tien nguoi choi
                    myMoney += finalScore;
                    saveUserMoney(playerName, myMoney);
                    // Luu diem so
                    Player newScoreEntry = {playerName, finalScore, finalLevel};
                    allScores.push_back(newScoreEntry); // Them diem moi vao vector
                    saveScores(allScores); // Luu vector vao file

                    setColor(10);
                    cout << "\nĐã lưu kết quả vào Bảng Xếp Hạng!" << endl;
                    resetColor();
                    pressEnterToContinue(); 
                    break;
                }
                case 2:
                    allScores = loadScores();
                    viewHighScores(allScores);
                    break;
                case 3:
                    adminLogin(allQuestions);
                    break;
                case 4:
                    viewGameRules();
                    break;
                case 5:
                    isUserSession = false; 
                    setColor(14);
                    cout << "Đang đăng xuất..." << endl;
                    resetColor();
                    sleep(1000);
                    break;
                default:
                    setColor(12);
                    cout << "Lựa chọn không hợp lệ. Vui lòng nhập lại (1-5)." << endl;
                    resetColor();
                    pressEnterToContinue(); 
                    break;
            }
        }
    }

    return 0;
}

    
