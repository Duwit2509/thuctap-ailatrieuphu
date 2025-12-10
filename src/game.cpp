#define WIN32_LEAN_AND_MEAN

#ifdef _WIN32
#include <windows.h>
#endif

#include "../include/helpers.h"
#include "../include/game.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <random>
#include <conio.h>
#include <set>
#include <map>
#include <vector>
#include <string>


using namespace std;

// Ham lay ngau nhien cau hoi theo level
Question getRandomQuestionByLevel(const vector<Question>& allQuestions, int level) {
    vector<Question> levelQuestions;
    for (const Question& q : allQuestions) {
        if (q.level == level) {
            levelQuestions.push_back(q);
        }
    }

    if (levelQuestions.empty()) {
        setColor(12); cerr << "LOI: Khong tim thay cau hoi level " << level << endl; resetColor();
        return {level, "Loi", {"A","B","C","D"}, "A"};
    }

    // Dung de luu cac cau hoi da duoc hoi
    static map<int, vector<string>> usedQuestions; 

    // Tao danh sach cac cau hoi chua duoc hoi
    vector<Question> availableQuestions;
    for (const auto& q : levelQuestions) {
        bool isUsed = false;
        for (const string& usedText : usedQuestions[level]) {
            if (q.questionText == usedText) {
                isUsed = true;
                break;
            }
        }
        if (!isUsed) {
            availableQuestions.push_back(q);
        }
    }

    // Neu tat ca cau hoi da duoc hoi, reset lai danh sach
    if (availableQuestions.empty()) {
        usedQuestions[level].clear();
        availableQuestions = levelQuestions; 
    }

    // Chon ngau nhien 1 cau hoi trong danh sach cau hoi con lai
    static random_device rd; 
    static mt19937 gen(rd()); 
    uniform_int_distribution<> dis(0, availableQuestions.size() - 1);
    
    int randomIndex = dis(gen);
    Question selectedQ = availableQuestions[randomIndex];

    // Luu cau hoi da duoc hoi
    usedQuestions[level].push_back(selectedQ.questionText);

    return selectedQ;
}


//Ham choi game
long long playGame(const vector<Question>& allQuestions, int &finalLevel) {
    int currentLevel = 1;
    long long money = 0;
    finalLevel = 0;
    bool used5050 = false; 
    bool usedAskAudience = false;
    bool usedPhoneAFriend = false;
    bool playerWantsToStop = false;

    const long long prizeMoney[] = {
        0, 200000, 400000, 600000, 1000000, 2000000,
        3000000, 6000000, 10000000, 14000000, 22000000,
        30000000, 40000000, 60000000, 85000000, 150000000
    };

    while (currentLevel <= 15) {
        Question currentQuestion = getRandomQuestionByLevel(allQuestions, currentLevel);
        bool answerSubmitted = false;
        char playerAnswer;

        int remainingTime = 30;
        while (!answerSubmitted) {
            displayQuestion(currentQuestion, prizeMoney[currentLevel], used5050, usedAskAudience, usedPhoneAFriend, ' '); 
            setColor(11);
            cout << "Nhập lựa chọn (A, B, C, D) hoặc (5, 6, 7) để trợ giúp, [S] stop: ";
            resetColor();
            //Goi ham nhan dien phim bam co gioi han thoi gian
            playerAnswer = getUserInputWithTimer(remainingTime);

            //Kiem tra neu het thoi gian
            if (playerAnswer == 'X') {
                playWrongSound(); 
                setColor(12);
                cout << "\nHẾT GIỜ!!! Bạn đã không đưa ra câu trả lời kịp thời." << endl;
                resetColor();
                sleep(1500);
                    cout << "Đáp án đúng là: ";
                setColor(10);
                cout << currentQuestion.answer << endl;
                resetColor();
                sleep(2000);
                if (currentLevel > 10) money = prizeMoney[10];
                else if (currentLevel > 5) money = prizeMoney[5];
                else money = 0;
                
                cout << "Bạn ra về với số tiền: ";
                setColor(10);
                cout << formatMoney(money) << " VND" << endl;
                resetColor();
                pressEnterToContinue();
                finalLevel = currentLevel - 1;
                return money; 
            }

            if (playerAnswer == '5') {
                if (used5050) {
                    setColor(12);
                    cout << "Bạn đã sử dụng quy trợ giúp này rồi! Nhấn Enter để chọn lại." << endl;
                    resetColor();
                    pressEnterToContinue();
                } else {
                    cout << "\nBạn đã chọn sử dụng trợ giúp 50:50!" << endl;
                    used5050 = true; 
                    apply5050(currentQuestion); 
                    setColor(14);
                    cout << "Máy tinh đang loại bỏ 2 đáp án sai..." << endl;
                    sleep(2000);
                    resetColor();
                }
            } 
            else if (playerAnswer == '6') {
                if (usedAskAudience) {
                    setColor(12);
                    cout << "Bạn đã sử dụng quy trợ giúp này rồi! Nhấn Enter để chọn lại." << endl;
                    resetColor();
                    pressEnterToContinue();
                } else {
                    cout << "\nBạn đã chọn sử dụng trợ giúp Hỏi ý kiến khán giả!" << endl;
                    usedAskAudience = true; 
                    applyAskAudience(currentQuestion); 
                }
            }
            else if (playerAnswer == '7') {
                if (usedPhoneAFriend) {
                    setColor(12);
                    cout << "Bạn đã sử dụng quy trợ giúp này rồi! Nhấn Enter để chọn lại." << endl;
                    resetColor();
                    pressEnterToContinue();
                } else {
                    cout << "\nBạn đã chọn sử dụng trợ giúp Gọi điện thoại!" << endl;
                    usedPhoneAFriend = true; 
                    applyPhoneAFriend(currentQuestion); 
                }
            }
            else if (playerAnswer == 'S') {
               playSelectSound();
                
                setColor(13); 
                cout << "\nBẠN CÓ CHẮC MUỐN DỪNG CUỘC CHƠI? (Y/N): ";
                resetColor();

                // Dùng _getch() để bắt phím Y hoặc N ngay lập tức
                char confirm = _getch();
                confirm = toupper(confirm); 
                cout << confirm << endl; 

                if (confirm == 'Y') {
                    playerWantsToStop = true;
                    answerSubmitted = true; 
                } else {
                    setColor(10);
                    cout << "Quyết định đúng đắn! Hãy tiếp tục chiến đấu ..." << endl;
                    resetColor();
                    sleep(1000); 
                }
            }
            else if (playerAnswer >= 'A' && playerAnswer <= 'D') {
                playPalpitatingSound();
                // Hien thi cau hoi voi dap an da chon
                displayQuestion(currentQuestion, prizeMoney[currentLevel], used5050, usedAskAudience, usedPhoneAFriend, playerAnswer);
                cout << "Đáp án cuối cùng của bạn là: ";
                setColor(11);
                cout << playerAnswer << endl;
                resetColor();
                sleep(2000); 
                answerSubmitted = true;
            } else {
                setColor(12);
                cout << "Lựa chọn không hợp lệ! Nhấn Enter để chọn lại" << endl;
                resetColor();
                pressEnterToContinue(); 
            }
        } 

        
        // Neu nguoi choi dung cuoc choi
        if (playerWantsToStop) {
            // Nguoi choi duoc so tien cua cau truoc do
            money = prizeMoney[currentLevel - 1]; 
            setColor(14); 
            cout << "\nBạn đã quyết định dừng cuộc chơi!" << endl;
            playEndGameSound();
            cout << "Bạn ra về với số tiền thưởng là: ";
            setColor(10);
            cout << formatMoney(money) << " VND" << endl;
            resetColor();
            pressEnterToContinue();
            finalLevel = currentLevel - 1;
            return money;
        }

        // Xu ly cau tra loi
        cout << "Và đáp án đúng là..." << endl;
        sleep(1500); 
        //Neu nguoi choi tra loi
        if (playerAnswer == currentQuestion.answer[0]) {
            //Cap nhat level hien tai
            finalLevel = currentLevel;
            //Kiem tra neu la moc quan trong
            bool isMilestone = (currentLevel == 5 || currentLevel == 10 || currentLevel == 15);

            if (isMilestone) {
                playWinsStepSound(); 
            } else {
                playCorrectSound();  
            }
            money = prizeMoney[currentLevel];
            setColor(10); 
            cout << "\nCHÍNH XÁC!!! (" << currentQuestion.answer << ")" << endl;
            cout << "Tiền thưởng hiện tại: " << formatMoney(money) << " VND" << endl;
            resetColor();
            if (currentLevel == 5 || currentLevel == 10 || currentLevel == 15) {
                setColor(13);
                cout << "Bạn đã vượt qua mốc quan trọng số " << currentLevel << "!" << endl;
                resetColor();
            }
            if (currentLevel == 15) {
                sleep(8000);
                playEndGameSound();

                setColor(14);
                cout << "========================================" << endl;
                cout << "CHÚC MỪNG! BẠN ĐÃ TRỞ THÀNH TRIỆU PHÚ!" << endl;
                cout << "BẠN CHIẾN THẮNG 150,000,000 VND!" << endl;
                cout << "========================================" << endl;
                resetColor();

                pressEnterToContinue(); 
                finalLevel = 15;
                return money;
            }
            sleep(2000);
             
            currentLevel++; 
            pressEnterToContinue();

        } else {
            setColor(12); 
            playWrongSound();  
            cout << "\nRất tiếc! Đó là câu trả lời sai." << endl;
            resetColor();
            sleep(1500); 
            cout << "Đáp án đúng là: ";
            setColor(10); 
            cout << currentQuestion.answer << endl;
            resetColor();
            sleep(2000); 
            if (currentLevel > 10) {
                money = prizeMoney[10]; 
            } else if (currentLevel > 5) {
                money = prizeMoney[5]; 
            } else {
                money = 0; 
            }
            cout << "Bạn ra về với số tiền thưởng là: " ;
            setColor(10);
            cout << formatMoney(money) << " VND" << endl;
            resetColor();
            pressEnterToContinue();
            finalLevel = currentLevel - 1;
            return money; 
        }
    } 
    
    return money; 
}

// Ham so sanh de sap xep diem cao
bool compareScores(const Player& a, const Player& b) {
    if (a.score != b.score) {
        return a.score > b.score; 
    }
    return a.level > b.level;
}

// Ham hien thi bang xep hang
void viewHighScores(vector<Player>& allPlayers) {
    clearScreen();
    playSelectSound();
    setColor(14); 
    cout << "╔══════════════════════════════════════════════════╗" << endl;
    cout << "║                  BẢNG XẾP HẠNG                   ║" << endl;
    cout << "╚══════════════════════════════════════════════════╝" << endl;
    resetColor();

    if (allPlayers.empty()) {
        cout << "Chưa có ai chơi!" << endl;
    } else {
        sort(allPlayers.begin(), allPlayers.end(), compareScores);

        setColor(15); 
        cout << left << setw(5) << "TOP" 
             << setw(25) << "TÊN NGƯỜI CHƠI" 
             << setw(15) << "CÂU SỐ"  
             << "TIỀN THƯỞNG" << endl;
        resetColor();
        cout << "--------------------------------------------------" << endl;

        int rank = 1;
        for (const Player& p : allPlayers) {
            // Mau sac cho 3 vi tri dau
            if (rank == 1) setColor(14);     
            else if (rank == 2) setColor(11); 
            else if (rank == 3) setColor(13); 
            else setColor(8);

            cout << left << setw(5) << rank 
                 << setw(20) << p.name 
                 << setw(10) << p.level 
                 << formatMoney(p.score) << " VND" << endl;
            
            resetColor();
            if (rank == 10) break; 
            rank++;
        }
    }
    pressEnterToContinue();
}
