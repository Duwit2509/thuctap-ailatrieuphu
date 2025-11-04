#include <iostream> 
#include <fstream>   
#include <vector>    
#include <string>    
#include <random>   
#include <cctype>    
#include <cstdlib>   
#include "json.hpp"

using namespace std;
using json = nlohmann::json; // Dat ten gon cho thu vien JSON

// XAY DUNG LOI NGUON DU LIEU 

struct Question {
    int level;
    string questionText;
    vector<string> options;
    string answer;
};

vector<Question> loadQuestions() {
    ifstream file_in("questions.json");
    if (!file_in.is_open()) {
        cerr << "LOI: Khong the mo file questions.json. Vui long kiem tra lai!" << endl;
        return {}; 
    }
    json j;
    file_in >> j;
    file_in.close();
    vector<Question> questionList;
    for (auto& item : j) {
        Question q;
        q.level = item["level"];
        q.questionText = item["question"];
        q.answer = item["answer"];
        q.options = item["options"].get<vector<string>>();
        questionList.push_back(q);
    }
    cout << "Da tai " << questionList.size() << " cau hoi tu file JSON." << endl;
    return questionList;
}

//  PHAN HE NGUOI CHOI 

void playGame(const vector<Question>& allQuestions);
void adminMenu(vector<Question>& allQuestions); 

// Ham xoa man hinh 
void clearScreen() {
    #ifdef _WIN32
        system("cls"); 
    #else
        system("clear"); 
    #endif
}

// Ham bat nguoi dung an Enter de tiep tuc
void pressEnterToContinue() {
    cout << "\nNhan Enter de tiep tuc...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xoa buffer truoc khi cho
    cin.get();    // Doi nguoi dung an Enter
}

// Ham hien thi cau hoi
void displayQuestion(const Question& q) {
    clearScreen();
    cout << "========================================" << endl;
    cout << "Cau hoi so " << q.level << endl;
    cout << "========================================" << endl;
    cout << q.questionText << endl;
    cout << "----------------------------------------" << endl;

    for (const string& opt : q.options) {
        cout << opt << endl;
    }
    cout << "========================================" << endl;
}

// Ham lay ngau nhien 1 cau hoi theo muc do (level)
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

    // Neu khong tim thay cau hoi (do file JSON bi loi)
    cerr << "LOI: Khong tim thay cau hoi cho level " << level << endl;
    return {level, "Cau hoi loi", {"A. Loi", "B. Loi", "C. Loi", "D. Loi"}, "A"};
}

//  HAM LOGIC CHINH CUA GAME
void playGame(const vector<Question>& allQuestions) {
    int currentLevel = 1;
    long long money = 0;
    // Mang luu 15 moc tien thuong (phan tu 0 la moc 0 dong)
    const long long prizeMoney[] = {
        0, 200000, 400000, 600000, 1000000, 2000000, // Moc 1-5
        3000000, 6000000, 1000000, 14000000, 22000000, // Moc 6-10
        30000000, 40000000, 60000000, 85000000, 150000000 // Moc 11-15
    };

    while (currentLevel <= 15) {
        Question currentQuestion = getRandomQuestionByLevel(allQuestions, currentLevel);
        displayQuestion(currentQuestion);

        cout << "Nhap cau tra loi (A, B, C, D): ";
        char playerAnswer;
        cin >> playerAnswer;
        
        // Kiem tra neu nguoi dung nhap loi (vi du: nhap chuoi)
        while (cin.fail()) {
            cin.clear(); // Xoa trang thai loi
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xoa buffer
            cout << "Nhap sai, vui long chi nhap A, B, C hoac D: ";
            cin >> playerAnswer;
        }

        playerAnswer = toupper(playerAnswer); // Chuyen ve chu IN HOA 

        // Kiem tra dap an
        if (playerAnswer == currentQuestion.answer[0]) {
            money = prizeMoney[currentLevel];
            cout << "\nChinh xac! Ban nhan duoc " << money << " VND" << endl;
            
            if (currentLevel == 5 || currentLevel == 10 || currentLevel == 15) {
                cout << "Ban da vuot qua moc quan trong so " << currentLevel << "!" << endl;
            }
            
            if (currentLevel == 15) {
                cout << "========================================" << endl;
                cout << "CHUC MUNG! BAN DA TRO THANH TRIEU PHU!" << endl;
                cout << "BAN CHIEN THANG 150,000,000 VND!" << endl;
                cout << "========================================" << endl;
            }

            currentLevel++; // Len cau hoi tiep theo
            pressEnterToContinue();

        } else {
            // Tra loi sai
            cout << "\nRat tiec! Do la cau tra loi sai." << endl;
            cout << "Dap an dung la: " << currentQuestion.answer << endl;

            // Tinh tien thuong khi thua (theo 3 moc 5, 10)
            if (currentLevel > 10) {
                money = prizeMoney[10]; // Moc 10
            } else if (currentLevel > 5) {
                money = prizeMoney[5]; // Moc 5
            } else {
                money = 0; // Moc 0
            }

            cout << "Ban ra ve voi so tien thuong la: " << money << " VND" << endl;
            pressEnterToContinue();
            break; 
        }
    }
}

//  PHAN HE QUAN TRI (ADMIN) 
void adminMenu(vector<Question>& allQuestions) {
    clearScreen();
    cout << "========================================" << endl;
    cout << "          CHUC NANG QUAN TRI VIEN" << endl;
    cout << "========================================" << endl;
    cout << "Chuc nang nay se duoc phat trien sau." << endl;
    pressEnterToContinue();
}


int main() {
    vector<Question> allQuestions = loadQuestions();

    if (allQuestions.empty()) {
        cout << "Khong co cau hoi nao duoc tai. Ket thuc chuong trinh." << endl;
        return 1;
    }

    cout << "Nhan Enter de bat dau...";
    cin.get();

    bool isRunning = true;
    while (isRunning) {
        clearScreen(); 

        cout << "========================================" << endl;
        cout << "     CHAO MUNG DEN VOI AI LA TRIEU PHU" << endl;
        cout << "========================================" << endl;
        cout << "1. Choi game" << endl;
        cout << "2. Quan ly cau hoi (Admin)" << endl;
        cout << "3. Thoat" << endl;
        cout << "========================================" << endl;
        cout << "Nhap lua chon cua ban (1-3): ";

        int choice;
        cin >> choice;
        
        // Kiem tra neu nguoi dung nhap sai (vi du: nhap chu 'a')
        while (cin.fail()) {
            cin.clear(); // Xoa trang thai loi
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Xoa bo nho dem
            cout << "Lua chon khong hop le. Vui long nhap so (1-3): ";
            cin >> choice;
        }

        switch (choice) {
            case 1:
                playGame(allQuestions);
                break;
            case 2:
                adminMenu(allQuestions);
                break;
            case 3:
                isRunning = false;
                cout << "Tam biet! Hen gap lai." << endl;
                break;
            default:
                cout << "Lua chon khong hop le. Vui long nhap lai (1, 2 hoac 3)." << endl;
                pressEnterToContinue(); 
                break;
        }
    }

    return 0;
}