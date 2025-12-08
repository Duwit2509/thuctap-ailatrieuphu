#include "../include/helpers.h"
#include "../include/admin.h"
#include <iostream>
using namespace std;

const string ADMIN_PASSWORD = "2509";

//Dang nhap Admin
void adminLogin(vector<Question>& allQuestions) {
    clearScreen();
    setColor(14); 
    cout << "╔══════════════════════════════════════╗" << endl;
    cout << "║            DANG NHAP ADMIN           ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
    resetColor();

    string pass = "";
    cout << "Vui long nhap mat khau Admin: ";
    getline(cin, pass);


    if (pass == ADMIN_PASSWORD) {
        setColor(10); 
        cout << "Dang nhap thanh cong! Dang tai..." << endl;
        resetColor();
        sleep(1500); 
        adminMenu(allQuestions); 
    } else {
        setColor(12); 
        cout << "Mat khau sai! Nhan Enter de quay lai." << endl;
        resetColor();
        pressEnterToContinue();
    }
}

//Menu Admin
void adminMenu(vector<Question>& allQuestions) {
    //giu cho menu admin hoat dong
    bool isAdminRunning = true;
    while (isAdminRunning) {
        clearScreen();
        setColor(14);
        cout << "╔══════════════════════════════════════╗" << endl;
        cout << "║        CHUC NANG QUAN TRI VIEN       ║" << endl;
        cout << "╚══════════════════════════════════════╝" << endl;
        resetColor();
        cout << "  1. Xem tat ca cau hoi" << endl;
        cout << "  2. Them cau hoi moi" << endl;
        cout << "  3. Xoa cau hoi" << endl;
        cout << "  4. Quay lai Menu chinh" << endl;
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
            case 1:
                viewAllQuestions(allQuestions);
                break;
            case 2:
                addQuestion(allQuestions);
                break;
            case 3:
                deleteQuestion(allQuestions);
                break;
            case 4:
                isAdminRunning = false;
                break;
            default:
                setColor(12);
                cout << "Lua chon khong hop le." << endl;
                resetColor();
                pressEnterToContinue();
                break;
        }
    }
}

//Hien thi tat ca cau hoi
void viewAllQuestions(const vector<Question>& allQuestions) {
    clearScreen();
    setColor(14);
    cout << "========================================" << endl;
    cout << "        DANH SACH TAT CA CAU HOI (" << allQuestions.size() << " cau)" << endl;
    cout << "========================================" << endl;
    resetColor();

    if (allQuestions.empty()) {
        cout << "Khong co cau hoi nao." << endl;
    }

    for (int i = 0; i < allQuestions.size(); ++i) {
        const Question& q = allQuestions[i]; 
        
        setColor(11);
        cout << "\n--- Cau hoi " << (i + 1) << " (Level: " << q.level << ") ---" << endl;
        resetColor();
        cout << "Noi dung: " << q.questionText << endl;
        for (const string& opt : q.options) {
            cout << opt << endl;
        }
        setColor(10);
        cout << "Dap an dung: " << q.answer << endl;
        resetColor();
    }
    pressEnterToContinue();
}

//Them cau hoi moi
void addQuestion(vector<Question>& allQuestions) {
    clearScreen();
    setColor(14);
    cout << "========================================" << endl;
    cout << "             THEM CAU HOI MOI" << endl;
    cout << "========================================" << endl;
    resetColor();
    
    Question newQ; 
    
    cout << "Nhap level (1-15): ";
    cin >> newQ.level;
    
    // Kiem tra loi nhap
    while (cin.fail() || newQ.level < 1 || newQ.level > 15) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        setColor(12);
        cout << "Level khong hop le. Vui long nhap so tu 1-15: ";
        resetColor();
        cin >> newQ.level;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "Nhap noi dung cau hoi: ";
    getline(cin, newQ.questionText);

    newQ.options.resize(4); 
    string tempOpt;

    cout << "Nhap noi dung dap an A: ";
    getline(cin, tempOpt);
    newQ.options[0] = "A. " + tempOpt;

    cout << "Nhap noi dung dap an B: ";
    getline(cin, tempOpt);
    newQ.options[1] = "B. " + tempOpt;

    cout << "Nhap noi dung dap an C: ";
    getline(cin, tempOpt);
    newQ.options[2] = "C. " + tempOpt;

    cout << "Nhap noi dung dap an D: ";
    getline(cin, tempOpt);
    newQ.options[3] = "D. " + tempOpt;

    cout << "Nhap dap an dung (A, B, C hay D): ";
    string answerInput;
    cin >> answerInput;
    newQ.answer = string(1, toupper(answerInput[0]));
    while (newQ.answer != "A" && newQ.answer != "B" && newQ.answer != "C" && newQ.answer != "D") {
        setColor(12);
        cout << "Dap an phai la A, B, C hoac D. Nhap lai: ";
        resetColor();
        cin >> answerInput;
        newQ.answer = string(1, toupper(answerInput[0]));
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    newQ.answer = toupper(newQ.answer[0]);

    allQuestions.push_back(newQ);
    sort(allQuestions.begin(), allQuestions.end(), [](const Question& a, const Question& b) {
        return a.level < b.level;
    });
    saveQuestions(allQuestions);

    setColor(10);
    cout << "\nDa them va luu cau hoi moi thanh cong!" << endl;
    resetColor();
    pressEnterToContinue();
}

//Xoa cau hoi
void deleteQuestion(vector<Question>& allQuestions) {
    clearScreen();
    setColor(14);
    cout << "========================================" << endl;
    cout << "              XOA CAU HOI" << endl;
    cout << "========================================" << endl;
    resetColor();

    if (allQuestions.empty()) {
        cout << "Khong co cau hoi nao de xoa." << endl;
        pressEnterToContinue();
        return;
    }

    setColor(8);
    cout << "Danh sach cau hoi (Nhap 0 de huy):" << endl;
    resetColor();
    for (size_t i = 0; i < allQuestions.size(); ++i) {
        //Chi hien thi phan dau cau hoi de de nhan biet
        string shortText = allQuestions[i].questionText.substr(0, 50);
        if (allQuestions[i].questionText.length() > 50) shortText += "...";
        
        cout << "[" << (i + 1) << "] Lv" << allQuestions[i].level << ": " << shortText << endl;
    }
    
    setColor(14);
    cout << "\nNhap STT cau hoi muon XOA: ";
    resetColor();
    int indexToDelete;
    cin >> indexToDelete;

    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        setColor(12);
        cout << "Nhap sai. Vui long nhap so: ";
        resetColor();
        cin >> indexToDelete;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    if (indexToDelete == 0) {
        setColor(10);
        cout << "Da huy xoa." << endl;
        resetColor();
        pressEnterToContinue();
        return;
    }

    if (indexToDelete > 0 && indexToDelete <= allQuestions.size()) {
        int realIndex = indexToDelete - 1;

        setColor(12);
        cout << "\nBan co chac muon xoa cau hoi: \"" << allQuestions[realIndex].questionText << "\" ? (y/n): ";
        resetColor();
        char confirm;
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        if (confirm == 'y' || confirm == 'Y') {
            allQuestions.erase(allQuestions.begin() + realIndex);
            saveQuestions(allQuestions);
            setColor(10);
            cout << "\nDa xoa va luu lai thanh cong!" << endl;
            resetColor();
        } else {
            cout << "\nHuy xoa." << endl;
        }
    } else {
        setColor(12);
        cout << "\nSo thu tu khong hop le. Khong co gi duoc xoa." << endl;
        resetColor();
    }
    pressEnterToContinue();
}