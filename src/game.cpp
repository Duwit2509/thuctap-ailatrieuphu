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


using namespace std;

//Lay cau hoi ngau nhien theo level
Question getRandomQuestionByLevel(const vector<Question>& allQuestions, int level) {
    vector<Question> levelQuestions;
    for (const Question& q : allQuestions) {
        if (q.level == level) {
            levelQuestions.push_back(q);
        }
    }

    if (!levelQuestions.empty()) {
        static random_device rd; 
        static mt19937 gen(rd()); 
        uniform_int_distribution<> dis(0, levelQuestions.size() - 1);
        int randomIndex = dis(gen);
        return levelQuestions[randomIndex];
    }

    setColor(12);
    cerr << "LOI: Khong tim thay cau hoi cho level " << level << endl;
    resetColor();
    return {level, "Cau hoi loi", {"A. Loi", "B. Loi", "C. Loi", "D. Loi"}, "A"};
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
            cout << "Nhap lua chon (A, B, C, D) hoac (5, 6, 7) de tro giup, [S] stop: ";
            resetColor();
            //Goi ham nhan dien phim bam co gioi han thoi gian
            playerAnswer = getUserInputWithTimer(remainingTime);

            //Kiem tra neu het thoi gian
            if (playerAnswer == 'X') {
                playWrongSound(); 
                setColor(12);
                cout << "\nHET GIO!!! Ban da khong dua ra cau tra loi kip thoi." << endl;
                resetColor();
                sleep(1500);
                    cout << "Dap an dung la: ";
                setColor(10);
                cout << currentQuestion.answer << endl;
                resetColor();
                sleep(2000);
                if (currentLevel > 10) money = prizeMoney[10];
                else if (currentLevel > 5) money = prizeMoney[5];
                else money = 0;
                
                cout << "Ban ra ve voi so tien: ";
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
                    cout << "Ban da su dung tro giup nay roi! Nhan Enter de chon lai." << endl;
                    resetColor();
                    pressEnterToContinue();
                } else {
                    cout << "Ban da chon su dung tro giup 50:50!" << endl;
                    used5050 = true; 
                    apply5050(currentQuestion); 
                    setColor(14);
                    cout << "May tinh dang loai bo 2 dap an sai..." << endl;
                    sleep(2000);
                    resetColor();
                }
            } 
            else if (playerAnswer == '6') {
                if (usedAskAudience) {
                    setColor(12);
                    cout << "Ban da su dung tro giup nay roi! Nhan Enter de chon lai." << endl;
                    resetColor();
                    pressEnterToContinue();
                } else {
                    cout << "Ban da chon su dung tro giup Hoi y kien khan gia!" << endl;
                    usedAskAudience = true; 
                    applyAskAudience(currentQuestion); 
                }
            }
            else if (playerAnswer == '7') {
                if (usedPhoneAFriend) {
                    setColor(12);
                    cout << "Ban da su dung tro giup nay roi! Nhan Enter de chon lai." << endl;
                    resetColor();
                    pressEnterToContinue();
                } else {
                    cout << "Ban da chon su dung tro giup Goi dien thoai!" << endl;
                    usedPhoneAFriend = true; 
                    applyPhoneAFriend(currentQuestion); 
                }
            }
            else if (playerAnswer == 'S') {
                playerWantsToStop = true;
                answerSubmitted = true; 
            }
            else if (playerAnswer >= 'A' && playerAnswer <= 'D') {
                playPalpitatingSound();
                // Hien thi cau hoi voi dap an da chon
                displayQuestion(currentQuestion, prizeMoney[currentLevel], used5050, usedAskAudience, usedPhoneAFriend, playerAnswer);
                cout << "Dap an cuoi cung cua ban la ";
                setColor(11);
                cout << playerAnswer << endl;
                resetColor();
                sleep(2000); 
                answerSubmitted = true;
            } else {
                setColor(12);
                cout << "Lua chon khong hop le! Nhan Enter de chon lai." << endl;
                resetColor();
                pressEnterToContinue(); 
            }
        } 

        
        // Neu nguoi choi dung cuoc choi
        if (playerWantsToStop) {
            // Nguoi choi duoc so tien cua cau truoc do
            money = prizeMoney[currentLevel - 1]; 
            setColor(14); 
            cout << "\nBan da quyet dinh dung cuoc choi!" << endl;
            playEndGameSound();
            cout << "Ban ra ve voi so tien thuong la: ";
            setColor(10);
            cout << formatMoney(money) << " VND" << endl;
            resetColor();
            pressEnterToContinue();
            finalLevel = currentLevel - 1;
            return money;
        }

        // Xu ly cau tra loi
        cout << "Va dap an dung la..." << endl;
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
            cout << "\nCHINH XAC!!! (" << currentQuestion.answer << ")" << endl;
            cout << "Tien thuong hien tai: " << formatMoney(money) << " VND" << endl;
            resetColor();
            if (currentLevel == 5 || currentLevel == 10 || currentLevel == 15) {
                setColor(13);
                cout << "Ban da vuot qua moc quan trong so " << currentLevel << "!" << endl;
                resetColor();
            }
            if (currentLevel == 15) {
                sleep(8000);
                playEndGameSound();

                setColor(14);
                cout << "========================================" << endl;
                cout << "CHUC MUNG! BAN DA TRO THANH TRIEU PHU!" << endl;
                cout << "BAN CHIEN THANG 150,000,000 VND!" << endl;
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
            cout << "\nRat tiec! Do la cau tra loi sai." << endl;
            resetColor();
            sleep(1500); 
            cout << "Dap an dung la: ";
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
            cout << "Ban ra ve voi so tien thuong la: " ;
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
    return a.score > b.score;
}

// Ham hien thi bang xep hang
void viewHighScores(vector<Player>& allPlayers) {
    clearScreen();
    playSelectSound();
    setColor(14); 
    cout << "╔══════════════════════════════════════╗" << endl;
    cout << "║            BANG XEP HANG             ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
    resetColor();

    if (allPlayers.empty()) {
        cout << "Chua co ai choi!" << endl;
    } else {
        sort(allPlayers.begin(), allPlayers.end(), compareScores);

        setColor(15); 
        cout << left << setw(5) << "TOP" 
             << setw(20) << "TEN NGUOI CHOI" 
             << setw(10) << "CAU SO"  
             << "TIEN THUONG" << endl;
        resetColor();
        cout << "----------------------------------------" << endl;

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
