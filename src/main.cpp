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
        cout << "LOI NGHIEM TRONG: Khong co cau hoi nao duoc tai. Ket thuc." << endl;
        resetColor();
        pressEnterToContinue();
    }

    cout << "Nhan Enter de bat dau...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');


    bool isRunning = true;
    
    while (isRunning) {
        playWelcomeSound();
        clearScreen(); 
        resetColor(); 

        setColor(14); 
        cout << "╔══════════════════════════════════════╗" << endl;
        cout << "║   CHAO MUNG DEN VOI AI LA TRIEU PHU  ║" << endl;
        cout << "╚══════════════════════════════════════╝" << endl;
        resetColor(); 
        
        cout << "  1. Choi game" << endl;
        cout << "  2. Xem Bang Xep Hang" << endl;
        cout << "  3. Quan ly cau hoi (Admin)" << endl;
        cout << "  4. Thoat" << endl;
        cout << "----------------------------------------" << endl;
        cout << "Nhap lua chon cua ban (1-4): ";

        int choice;
        cin >> choice;
        
        while (cin.fail()) {
            cin.clear(); 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            setColor(12);
            cout << "Lua chon khong hop le. Vui long nhap so (1-4): ";
            resetColor();
            cin >> choice;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: { 
                if (allQuestions.empty()) {
                    setColor(12);
                    cout << "Chua co du lieu cau hoi! Vui long vao Admin de them." << endl;
                    resetColor();
                    pressEnterToContinue();
                    break;
                }
                clearScreen();
                playIntroSound();
                setColor(14);
                cout << "Chuan bi vao tro choi..." << endl;
                resetColor();
                sleep(2000);
                
                string playerName;
                cout << "Vui long nhap ten cua ban: ";
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
                cout << "\nDa luu ket qua cua ban vao Bang Xep Hang!" << endl;
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
                cout << "Tam biet! Hen gap lai." << endl;
                sleep(1000);
                break;
            default:
                setColor(12);
                cout << "Lua chon khong hop le. Vui long nhap lai (1-4)." << endl;
                resetColor();
                pressEnterToContinue(); 
                break;
        }
    }

    return 0;
}

    
