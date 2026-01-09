#include "../include/helpers.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

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

void playApplyCallWiseMan() {
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



// Hàm nhập chuỗi Tiếng Việt có dấu trên Console Windows
string inputUTF8() {
#ifdef _WIN32
    // Cấp phát bộ đệm để đọc ký tự Wide (Unicode 16-bit)
    const int MAX_BUFFER = 1024;
    wchar_t wBuffer[MAX_BUFFER];
    DWORD charsRead;
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);

    // Dùng API Windows đọc trực tiếp Unicode từ bàn phím
    ReadConsoleW(hStdIn, wBuffer, MAX_BUFFER - 1, &charsRead, NULL);

    if (charsRead > 0) {
        // Loại bỏ ký tự xuống dòng (\r\n) ở cuối do phím Enter tạo ra
        // Windows thường trả về \r\n, ta cần xóa nó đi để chuỗi sạch
        if (charsRead >= 2 && wBuffer[charsRead - 2] == L'\r') {
            wBuffer[charsRead - 2] = L'\0';
        } else if (charsRead >= 1 && (wBuffer[charsRead - 1] == L'\n' || wBuffer[charsRead - 1] == L'\r')) {
            wBuffer[charsRead - 1] = L'\0';
        } else {
            wBuffer[charsRead] = L'\0'; // Đảm bảo kết thúc chuỗi
        }

        // --- Chuyển đổi từ wstring (Unicode) sang string (UTF-8) ---
        
        // B1: Tính độ dài cần thiết cho chuỗi UTF-8
        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wBuffer, -1, NULL, 0, NULL, NULL);
        
        // B2: Tạo chuỗi string đệm
        string strTo(size_needed, 0);
        
        // B3: Thực hiện chuyển đổi
        WideCharToMultiByte(CP_UTF8, 0, wBuffer, -1, &strTo[0], size_needed, NULL, NULL);
        
        // Loại bỏ ký tự null thừa ở cuối string do quá trình convert (rất quan trọng)
        if (!strTo.empty() && strTo.back() == '\0') strTo.pop_back();
        
        return strTo;
    }
    return "";
#else
    // Nếu chạy trên Linux/Mac thì getline bình thường vẫn ổn
    string s;
    getline(cin, s);
    return s;
#endif
}