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
        resetColor(); 

        setColor(14); 
        cout << "╔══════════════════════════════════════╗" << endl;
        cout << "║   CHÀO MỪNG ĐẾN VỚI AI LÀ TRIỆU PHÚ  ║" << endl;
        cout << "╚══════════════════════════════════════╝" << endl;
        resetColor(); 
        
        cout << "  1. Chơi game" << endl;
        cout << "  2. Xem Bảng Xếp Hạng" << endl;
        cout << "  3. Quản lý câu hỏi (Admin)" << endl;
        cout << "  4. Thoát" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Nhập lựa chọn của bạn (1-4): ";

        int choice;
        cin >> choice;
        
        while (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            setColor(12);
            cout << "Lựa chọn không hợp lệ. Vui lòng nhập số (1-4): ";
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
                playIntroSound();
                setColor(14);
                cout << "Chuẩn bị vào trò chơi..." << endl;
                resetColor();
                sleep(2000);
                
                string playerName;
                cout << "Vui lòng nhập tên của bạn: ";
                getline(cin, playerName); 
                if (playerName.empty()) playerName = "Noname";
                sleep(1000);
                int finalLevel = 0;
                long long finalScore = playGame(allQuestions, finalLevel);
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
                isRunning = false;
                cout << "Tạm biệt! Hẹn gặp lại." << endl;
                sleep(1000);
                break;
            default:
                setColor(12);
                cout << "Lựa chọn không hợp lệ. Vui lòng nhập lại (1-4)." << endl;
                resetColor();
                pressEnterToContinue(); 
                break;
        }
    }

    return 0;
}

    
