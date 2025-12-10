#include "../include/helpers.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <limits>
#include <sstream>

#ifdef _WIN32
    #include <windows.h>
    #include <mmsystem.h>
    #include <conio.h> 
    #pragma comment(lib, "winmm.lib") 
#endif

using namespace std;

// Ham tro giup de doi mau chu tren Console (chi danh cho Windows)
void setColor(int color) {
    #ifdef _WIN32
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    #endif
}

// Ham tro giup de reset mau ve mac dinh (mau trang)
void resetColor() {
    setColor(7);
}

/* --- BANG MA MAU (de ban tra cuu) ---
7: Trang trang
8: Xam
10: Xanh la (cho dap an dung)
11: Xanh duong (cho tieu de)
13: Tim 
12: Do (cho dap an sai / thong bao loi)
14: Vang (cho cau hoi / tieu de)
15: Trang sang (noi bat)
*/

// Ham tro giup de dung chuong trinh trong X mili-giay
void sleep(int milliseconds) {
    this_thread::sleep_for(chrono::milliseconds(milliseconds));
}

// Ham xoa man hinh (hoat dong tren ca Windows va Linux)
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // Lenh cho Windows
    #else
        system("clear"); // Lenh cho Linux/macOS
    #endif
}

// Ham bat nguoi dung an Enter de tiep tuc 
void pressEnterToContinue() {
    setColor(7);
    cout << "\nNhấn Enter để tiếp tục...";
    
    // Xoa bo nho dem de tranh van de bam phim
    #ifdef _WIN32
        FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
    #else
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    #endif
    cin.clear();
    cin.get();
}


// Am thanh cho cac su kien khac nhau
void playSelectSound() {
    PlaySoundA("sounds/select.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playCorrectSound() {
    PlaySoundA("sounds/correct.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playWrongSound() {
    PlaySoundA("sounds/wrong.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playNextQuestionSound() {
    PlaySoundA("sounds/next.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playWinsStepSound() {
    PlaySoundA("sounds/winsstep.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playIntroSound() {
    PlaySoundA("sounds/intro.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playWelcomeSound() {
    PlaySoundA("sounds/welcome.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playEndGameSound() {
    PlaySoundA("sounds/endgame.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playPalpitatingSound() {
    PlaySoundA("sounds/palpitate.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void playApply5050Sound() {
    #ifdef _WIN32
    Beep(400, 300); 
    Sleep(200);     
    Beep(300, 400); 
    #endif
}

void playApplyAskAudienceSound() {
    #ifdef _WIN32
    for (int i = 200; i <= 800; i += 100) {
        Beep(i, 100); 
    }
    #endif
}

void playApplyPhoneAFriendSound() {
   #ifdef _WIN32
    for (int i = 0; i < 2; i++) {
        Beep(1200, 50); Beep(1500, 50); Beep(1200, 50); Beep(1500, 50);
        Beep(1200, 50); Beep(1500, 50); Beep(1200, 50); Beep(1500, 50);
        Beep(1200, 50); Beep(1500, 50); Beep(1200, 50); Beep(1500, 50);
        cout << "Reng... ";
        Sleep(1000); 
    }
    cout << endl;
    #endif
}

// Ham dinh dang tien te VND
string formatMoney(long long money) {
    string s = to_string(money);
    int n = s.length();

    // Duyệt ngược và chèn dấu chấm
    for (int i = n - 3; i > 0; i -= 3) {
        s.insert(i, ".");
    }

    return s;
}

// Ham ngat dong cho van ban dai
string wrapText(const string& text, int width) {
    stringstream ss(text);
    string word;
    string line;
    string result;

    while (ss >> word) {
        if (line.length() + word.length() + 1 > width) {
            result += line + "\n";
            line = word;
        } else {
            if (!line.empty()) line += " ";
            line += word;
        }
    }

    if (!line.empty()) 
        result += line;

    return result;
}