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
    cout << "║            ĐĂNG NHẬP ADMIN           ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
    resetColor();

    string pass = "";
    cout << "Vui lòng nhập mật khẩu Admin: ";
    getline(cin, pass);


    if (pass == ADMIN_PASSWORD) {
        setColor(10); 
        cout << "Đăng nhập thành công! Đang tải..." << endl;
        resetColor();
        sleep(1500); 
        adminMenu(allQuestions); 
    } else {
        setColor(12); 
        cout << "Mật khẩu sai! Nhấn Enter để quay lại." << endl;
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
        cout << "║        CHỨC NĂNG QUẢN TRỊ VIÊN       ║" << endl;
        cout << "╚══════════════════════════════════════╝" << endl;
        resetColor();
        cout << "  1. Xem tất cả câu hỏi" << endl;
        cout << "  2. Thêm câu hỏi mới" << endl;
        cout << "  3. Xóa câu hỏi" << endl;
        cout << "  4. Quay lại Menu chính" << endl;
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
                cout << "Lựa chọn không hợp lệ." << endl;
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
    cout << "        DANH SÁCH TẤT CẢ CÂU HỎI (" << allQuestions.size() << " câu)" << endl;
    cout << "========================================" << endl;
    resetColor();

    if (allQuestions.empty()) {
        cout << "Không có câu hỏi nào." << endl;
    }

    int pageSize = 5; // So cau hoi hien thi tren 1 trang
    int totalQuestions = allQuestions.size();
    int totalPages = (totalQuestions + pageSize - 1) / pageSize; // Tinh tong so trang

   for (int page = 0; page < totalPages; ++page) {
        clearScreen();
        setColor(14);
        cout << "========================================" << endl;
        cout << " DANH SÁCH CÂU HỎI (Trang " << (page + 1) << "/" << totalPages << ")" << endl;
        cout << " Tổng số câu: " << totalQuestions << endl;
        cout << "========================================" << endl;
        resetColor();

        int start = page * pageSize;
        int end = min(start + pageSize, totalQuestions);

        for (int i = start; i < end; ++i) {
            const Question& q = allQuestions[i]; 
            
            setColor(11);
            cout << "\n--- Câu " << (i + 1) << " (Level: " << q.level << ") ---" << endl;
            resetColor();
            cout << "Nội dung: " << q.questionText << endl;
            
            // In cac dap an
            for (const string& opt : q.options) {
                cout << "  " << opt << endl;
            }
            
            setColor(10);
            cout << "Đáp án đúng: " << q.answer << endl;
            resetColor();
            cout << "----------------------------------------" << endl;
        }

        // Hoi nguoi dung muon xem tiep khong
        if (page < totalPages - 1) {
            setColor(14);
            cout << "\n>> Nhấn Enter để xem trang tiếp theo (hoặc nhập 'Q' để thoát)...";
            resetColor();
            
            string input;
            getline(cin, input);
            if (input == "q" || input == "Q") break;
        } else {
            setColor(14);
            cout << "\n>> Đã hết danh sách.";
            resetColor();
            pressEnterToContinue();
        }
    }
}

//Them cau hoi moi
void addQuestion(vector<Question>& allQuestions) {
    clearScreen();
    setColor(14);
    cout << "========================================" << endl;
    cout << "             THÊM CÂU HỎI MỚI" << endl;
    cout << "========================================" << endl;
    resetColor();
    
    Question newQ; 
    
    cout << "Nhập level (1-15): ";
    cin >> newQ.level;
    
    // Kiem tra loi nhap
    while (cin.fail() || newQ.level < 1 || newQ.level > 15) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        setColor(12);
        cout << "Level không hợp lệ. Vui lòng nhập số từ 1-15: ";
        resetColor();
        cin >> newQ.level;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    cout << "Nhập nội dung câu hỏi: ";
    getline(cin, newQ.questionText);

    newQ.options.resize(4); 
    string tempOpt;

    cout << "Nhập nội dung đáp án A: ";
    getline(cin, tempOpt);
    newQ.options[0] = "A. " + tempOpt;

    cout << "Nhập nội dung đáp án B: ";
    getline(cin, tempOpt);
    newQ.options[1] = "B. " + tempOpt;

    cout << "Nhập nội dung đáp án C: ";
    getline(cin, tempOpt);
    newQ.options[2] = "C. " + tempOpt;

    cout << "Nhập nội dung đáp án D: ";
    getline(cin, tempOpt);
    newQ.options[3] = "D. " + tempOpt;

    cout << "Nhập đáp án đúng (A, B, C hay D): ";
    string answerInput;
    cin >> answerInput;
    newQ.answer = string(1, toupper(answerInput[0]));
    while (newQ.answer != "A" && newQ.answer != "B" && newQ.answer != "C" && newQ.answer != "D") {
        setColor(12);
        cout << "Đáp án phải là A, B, C hoặc D. Nhập lại: ";
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
    cout << "\nĐã thêm và lưu câu hỏi mới thành công!" << endl;
    resetColor();
    pressEnterToContinue();
}

//Xoa cau hoi
void deleteQuestion(vector<Question>& allQuestions) {
    clearScreen();
    setColor(14);
    cout << "========================================" << endl;
    cout << "              XÓA CÂU HỎI" << endl;
    cout << "========================================" << endl;
    resetColor();

    if (allQuestions.empty()) {
        cout << "Không có câu hỏi nào để xóa." << endl;
        pressEnterToContinue();
        return;
    }

    setColor(8);
    cout << "Danh sách câu hỏi:" << endl;
    resetColor();
    for (size_t i = 0; i < allQuestions.size(); ++i) {
        //Chi hien thi phan dau cau hoi de de nhan biet
        string shortText = allQuestions[i].questionText.substr(0, 50);
        if (allQuestions[i].questionText.length() > 50) shortText += "...";
        
        cout << "[" << (i + 1) << "] Lv" << allQuestions[i].level << ": " << shortText << endl;
    }
    
    setColor(14);
    cout << "\nNhập STT câu hỏi muốn XÓA (Nhập 0 để hủy): ";
    resetColor();
    int indexToDelete;
    cin >> indexToDelete;

    while (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        setColor(12);
        cout << "Nhập sai. Vui lòng nhập số: ";
        resetColor();
        cin >> indexToDelete;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

    if (indexToDelete == 0) {
        setColor(10);
        cout << "Đã hủy xóa." << endl;
        resetColor();
        pressEnterToContinue();
        return;
    }

    if (indexToDelete > 0 && indexToDelete <= allQuestions.size()) {
        int realIndex = indexToDelete - 1;

        setColor(12);
        cout << "\nBạn có chắc muốn xóa câu hỏi: \"" << allQuestions[realIndex].questionText << "\" ? (y/n): ";
        resetColor();
        char confirm;
        cin >> confirm;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        if (confirm == 'y' || confirm == 'Y') {
            allQuestions.erase(allQuestions.begin() + realIndex);
            saveQuestions(allQuestions);
            setColor(10);
            cout << "\nĐã xóa và lưu lại thành công!" << endl;
            resetColor();
        } else {
            cout << "\nHủy xóa." << endl;
        }
    } else {
        setColor(12);
        cout << "\nSố thứ tự không hợp lệ. Không có gì được xóa." << endl;
        resetColor();
    }
    pressEnterToContinue();
}