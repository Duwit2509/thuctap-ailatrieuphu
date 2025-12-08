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
    cout << "╔═══════════════ QUYEN TRO GIUP ═══════════════ ║ ═══════ LUA CHON KHAC ═══════╗" << endl;
    resetColor();

    // 50:50
    if (used5050) {
    setColor(7); 
    cout << "║       [5] 50:50 (Da su dung)                  ║";
    } else {
    setColor(10); 
    cout << "║       [5] 50:50                               ║";
    }
    resetColor(); 
    // Dung cuoc choi
    setColor(12); 
    cout << "      [S] Dung cuoc choi     " << endl;
    resetColor();

    // Hoi khan gia
    if (usedAskAudience) {
    setColor(7); 
    cout << "║       [6] Hoi khan gia (Da su dung)           ║                " << endl;
    } else {
    setColor(10); 
    cout << "║       [6] Hoi khan gia                        ║                " << endl;
    }
    resetColor();

    // Goi dien thoai
    if (usedPhoneAFriend) {
    setColor(7); 
    cout << "║       [7] Goi dien thoai (Da su dung)         ║                " << endl;
    } else {
    setColor(10); 
    cout << "║       [7] Goi dien thoai                      ║                " << endl;
    }
    resetColor();

    cout << "╚══════════════════════════════════════════════ ╩ ═════════════════════════════╝" << endl << endl;
                                                              
    setColor(14); 
    cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << endl;

     // Tieu de cau hoi va tri gia hien tai
    string header = "CAU SO " + to_string(q.level) + "  -  TRI GIA: " + formatMoney(currentPrize) + " VND";

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
    int maxWidth = 70; // Chieu rong toi da cho cau hoi
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
    cout << "Dang lay y kien khan gia..." << endl;
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
    cout << "╔════════ KET QUA KHAO SAT KHAN GIA ════════╗" << endl;
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
    cout << "Dang ket noi voi 'Nha Thong Thai'..." << endl;
    resetColor();

    playApplyPhoneAFriendSound();

    cout << "..." << endl;
    sleep(1000);
    setColor(8);
    cout << "Nha Thong Thai: 'Alo, toi nghe!'" << endl;
    sleep(1500);
    setColor(11);
    cout << "Ban: (Doc cau hoi...)" << endl;
    sleep(2000);
    setColor(8);
    cout << "Nha Thong Thai: 'Hmm... cau nay...'" << endl;
    sleep(1500);
    cout << "Nha Thong Thai: 'Toi nghi... dap an la... ";
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
    cout << "Nha Thong Thai: 'Toi kha chac chan... nhung quyet dinh la cua ban. Chuc may man!'" << endl;
    resetColor();
    cout << "Tit... tit... (Ket thuc cuoc goi)" << endl;

    pressEnterToContinue();
}

//Ham nhan dien phim bam co gioi han thoi gian
char getUserInputWithTimer(int &timeLeft) {
    // Xoa bo nho dem
    FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));

    cout << endl; 

    while (timeLeft > 0) {
        setColor(12); 
        cout << "\r   >> Thoi gian con lai: " << setw(2) << timeLeft << " giay <<   ";
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