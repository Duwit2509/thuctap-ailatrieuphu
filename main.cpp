#include <iostream>  
#include <fstream>   
#include <vector>    
#include <string>    
#include <random>    
#include <cctype>    
#include <cstdlib>  
#include <algorithm> 
#include <chrono>   
#include <thread>    
#include <limits>    
#include <iomanip>   

// Bao gom header cho Windows de bat UTF-8 va Mau sac
#ifdef _WIN32
#include <windows.h> 
#endif

#include "json.hpp"
using namespace std;
using json = nlohmann::json; 


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
7: Trang (mac dinh)
10: Xanh la (cho dap an dung)
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
    cout << "\nNhan Enter de tiep tuc...";
    cin.clear();
    cin.sync();   
    cin.get();   
}


// KHAI BAO CAU TRUC (STRUCTS)
//  Dinh nghia CAU TRUC  de luu tru 1 cau hoi
struct Question {
    int level;
    string questionText;
    vector<string> options;
    string answer;
};

// Dinh nghia Nguoi choi 
struct Player {
    string name;
    long long score;
};

// KHAI BAO PROTOTYPE CAC HAM LOGIC

// Ham JSON
vector<Question> loadQuestions();
void saveQuestions(const vector<Question>& allQuestions);
vector<Player> loadScores();
void saveScores(const vector<Player>& allPlayers);

// Ham Game
long long playGame(const vector<Question>& allQuestions);
void displayQuestion(const Question& q, bool used5050, bool usedAskAudience, bool usedPhoneAFriend);
Question getRandomQuestionByLevel(const vector<Question>& allQuestions, int level);
void apply5050(Question& currentQuestion);
void applyAskAudience(const Question& currentQuestion);
void applyPhoneAFriend(const Question& currentQuestion);
void viewHighScores(vector<Player>& allPlayers);
bool compareScores(const Player& a, const Player& b);

// Ham Admin
void adminLogin(vector<Question>& allQuestions);
void adminMenu(vector<Question>& allQuestions);
void viewAllQuestions(const vector<Question>& allQuestions);
void addQuestion(vector<Question>& allQuestions);
void deleteQuestion(vector<Question>& allQuestions);


int main() {
    #ifdef _WIN32
        // Thiet lap console Windows de HIEN THI ky tu (UTF-8)
        SetConsoleOutputCP(CP_UTF8); 
        // Thiet lap console Windows de NHAN ky tu (UTF-8)
        SetConsoleCP(CP_UTF8);
    #endif

    // Tai CAU HOI
    vector<Question> allQuestions = loadQuestions();
    // Tai DIEM SO
    vector<Player> allScores = loadScores();

    if (allQuestions.empty()) {
        setColor(12);
        cout << "LOI NGHIEM TRONG: Khong co cau hoi nao duoc tai. Ket thuc." << endl;
        resetColor();
        pressEnterToContinue();
        return 1;
    }

    cout << "Nhan Enter de bat dau...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool isRunning = true;
    while (isRunning) {
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
                clearScreen();
                setColor(14);
                cout << "Chuan bi vao tro choi..." << endl;
                resetColor();
                
                string playerName;
                cout << "Vui long nhap ten cua ban: ";
                getline(cin, playerName); 

                // Goi ham choi game va nhan lai diem
                long long finalScore = playGame(allQuestions);

                // Luu diem so
                Player newScoreEntry = {playerName, finalScore};
                allScores.push_back(newScoreEntry); // Them diem moi vao vector
                saveScores(allScores); // Luu vector vao file

                cout << "Da luu ket qua cua ban!" << endl;
                pressEnterToContinue(); 
                break;
            }
            case 2:
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


// DINH NGHIA CAC HAM LOGIC

// HAM XU LY JSON

vector<Question> loadQuestions() {
    ifstream file_in("questions.json");
    if (!file_in.is_open()) {
        cerr << "LOI: Khong the mo file questions.json. Vui long kiem tra lai!" << endl;
        return {}; 
    }
    json j;
    try {
        file_in >> j;
        if (j.is_null() || j.empty()) { 
             cout << "File questions.json dang rong, bat dau danh sach moi." << endl;
             file_in.close();
             return {};
        }
    } catch (json::parse_error& e) {
        cerr << "LOI: File questions.json bi loi dinh dang! " << e.what() << endl;
        file_in.close();
        return {};
    }
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

void saveQuestions(const vector<Question>& allQuestions) {
    json j_array = json::array();
    for (const Question& q : allQuestions) {
        json j_item;
        j_item["level"] = q.level;
        j_item["question"] = q.questionText;
        j_item["options"] = q.options;
        j_item["answer"] = q.answer;
        j_array.push_back(j_item);
    }
    ofstream file_out("questions.json");
    if (!file_out.is_open()) {
        cerr << "LOI: Khong the mo file questions.json de LUU!" << endl;
        return;
    }
    file_out << j_array.dump(4);
    file_out.close();
    cout << "Da luu thanh cong " << allQuestions.size() << " cau hoi." << endl;
}

vector<Player> loadScores() {
    ifstream file_in("scores.json");
    vector<Player> allPlayers;
    if (!file_in.is_open()) {
        cout << "File scores.json khong tim thay, se tao file moi." << endl;
        return allPlayers; 
    }
    json j;
    try {
        file_in >> j;
        if (j.is_null() || j.empty()) { 
             cout << "File scores.json dang rong, bat dau danh sach moi." << endl;
             file_in.close();
             return allPlayers;
        }
    } catch (json::parse_error& e) {
        cout << "File scores.json dang rong hoac loi, bat dau danh sach moi." << endl;
        file_in.close();
        return allPlayers;
    }
    file_in.close();
    for (auto& item : j) {
        Player p;
        p.name = item["name"];
        p.score = item["score"];
        allPlayers.push_back(p);
    }
    cout << "Da tai " << allPlayers.size() << " luot choi truoc." << endl;
    return allPlayers;
}

void saveScores(const vector<Player>& allPlayers) {
    json j_array = json::array();
    for (const Player& p : allPlayers) {
        json j_item;
        j_item["name"] = p.name;
        j_item["score"] = p.score;
        j_array.push_back(j_item);
    }
    ofstream file_out("scores.json");
    file_out << j_array.dump(4);
    file_out.close();
    cout << "Da luu diem so vao file scores.json." << endl;
}

// HAM LOGIC GAME 

long long playGame(const vector<Question>& allQuestions) {
    int currentLevel = 1;
    long long money = 0;
    bool used5050 = false; 
    bool usedAskAudience = false;
    bool usedPhoneAFriend = false;
    bool playerWantsToStop = false;

    const long long prizeMoney[] = {
        0, 200000, 400000, 600000, 1000000, 2000000,
        3000000, 6000000, 1000000, 14000000, 22000000,
        30000000, 40000000, 60000000, 85000000, 150000000
    };

    while (currentLevel <= 15) {
        Question currentQuestion = getRandomQuestionByLevel(allQuestions, currentLevel);
        bool answerSubmitted = false;
        char playerAnswer;

        while (!answerSubmitted) {
            displayQuestion(currentQuestion, used5050, usedAskAudience, usedPhoneAFriend); 
            cout << "Nhap lua chon (A, B, C, D) hoac (5, 6, 7) de tro giup, [S] stop: ";
            cin >> playerAnswer;

            while (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                setColor(12);
                cout << "Nhap sai. Vui long nhap (A, B, C, D) hoac (5, 6, 7), [S]: ";
                resetColor();
                cin >> playerAnswer;
            }

            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            playerAnswer = toupper(playerAnswer); 

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
            cout << "Ban ra ve voi so tien thuong la: " << money << " VND" << endl;
            resetColor();
            pressEnterToContinue();
            break; 
        }

        //Neu nguoi choi tra loi
        if (playerAnswer == currentQuestion.answer[0]) {
            money = prizeMoney[currentLevel];
            setColor(10); 
            cout << "\nChinh xac! Ban nhan duoc " << money << " VND" << endl;
            resetColor();
            if (currentLevel == 5 || currentLevel == 10 || currentLevel == 15) {
                cout << "Ban da vuot qua moc quan trong so " << currentLevel << "!" << endl;
            }
            if (currentLevel == 15) {
                setColor(14);
                cout << "========================================" << endl;
                cout << "CHUC MUNG! BAN DA TRO THANH TRIEU PHU!" << endl;
                cout << "BAN CHIEN THANG 150,000,000 VND!" << endl;
                cout << "========================================" << endl;
                resetColor();
            }
            currentLevel++; 
            pressEnterToContinue();

        } else {
            setColor(12); 
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
            cout << "Ban ra ve voi so tien thuong la: " << money << " VND" << endl;
            pressEnterToContinue();
            return money; 
            break; 
        }
    } 
    
    return money; 
}


// Ham hien thi cau hoi 
void displayQuestion(const Question& q, bool used5050, bool usedAskAudience, bool usedPhoneAFriend) {
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
    cout << "║       [6] Hoi khan gia (Da su dung)           ║                ║" << endl;
    } else {
    setColor(10); 
    cout << "║       [6] Hoi khan gia                        ║                " << endl;
    }
    resetColor();

    // Goi dien thoai
    if (usedPhoneAFriend) {
    setColor(7); 
    cout << "║       [7] Goi dien thoai (Da su dung)         ║                ║" << endl;
    } else {
    setColor(10); 
    cout << "║       [7] Goi dien thoai                      ║                " << endl;
    }
    resetColor();

    cout << "╚══════════════════════════════════════════════ ╩ ═════════════════════════════╝" << endl << endl;
                                                              
    setColor(14); 
    cout << "╔══════════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║                  Cau hoi so " << q.level << "                                 " << endl;
    cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << endl;
    cout << "║ " << q.questionText << endl; 
    cout << "╠══════════════════════════════════════════════════════════════════════════════╣" << endl;
    resetColor(); 
    cout << "║ " << q.options[0] << endl;
    cout << "║ " << q.options[1] << endl;
    cout << "║ " << q.options[2] << endl;
    cout << "║ " << q.options[3] << endl;
    cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << endl;
}


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


    currentQuestion.options[indexToHide1] = string(1, 'A' + indexToHide1) + ". ...";
    currentQuestion.options[indexToHide2] = string(1, 'A' + indexToHide2) + ". ...";
}

// Ham thuc hien tro giup Hoi y kien khan gia 
void applyAskAudience(const Question& currentQuestion) {
    clearScreen();
    setColor(14); 
    cout << "Dang lay y kien khan gia..." << endl;
    resetColor();
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
        
        cout << "]"; // Dau ngoac dong
        resetColor(); 
        cout << endl;
    }
    cout << "╚═══════════════════════════════════════════╝" << endl;
    
    pressEnterToContinue(); 
}

// Ham thuc hien tro giup Goi dien thoai 
void applyPhoneAFriend(const Question& currentQuestion) {
    clearScreen();
    setColor(14); 
    cout << "Dang ket noi voi 'Nha Thong Thai'..." << endl;
    sleep(1500);
    cout << "..." << endl;
    sleep(1000);
    cout << "Nha Thong Thai: 'Alo, toi nghe!'" << endl;
    sleep(1500);
    cout << "Ban: (Doc cau hoi...)" << endl;
    sleep(2000);
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
    cout << "Nha Thong Thai: 'Toi kha chac chan... nhung quyet dinh la o ban. Chuc may man!'" << endl;
    setColor(15);
    resetColor();
    cout << "Tit... tit... (Ket thuc cuoc goi)" << endl;

    pressEnterToContinue();
}


bool compareScores(const Player& a, const Player& b) {
    return a.score > b.score;
}

// Ham so sanh de sap xep cau hoi 
bool compareLevels(const Question& a, const Question& b) {
    return a.level < b.level; 
}

void viewHighScores(vector<Player>& allPlayers) {
    clearScreen();
    setColor(14); 
    cout << "╔══════════════════════════════════════╗" << endl;
    cout << "║            BANG XEP HANG             ║" << endl;
    cout << "╚══════════════════════════════════════╝" << endl;
    resetColor();

    if (allPlayers.empty()) {
        cout << "Chua co ai choi!" << endl;
    } else {
        sort(allPlayers.begin(), allPlayers.end(), compareScores);

        cout << left << setw(5) << "HANG" << setw(20) << "TEN NGUOI CHOI" << "DIEM SO" << endl;
        cout << "----------------------------------------" << endl;
        int rank = 1;
        for (const Player& p : allPlayers) {
            cout << left << setw(5) << rank << setw(20) << p.name << p.score << endl;
            if (rank == 10) break; 
            rank++;
        }
    }
    pressEnterToContinue();
}


// --- HAM LOGIC ADMIN ---

const string ADMIN_PASSWORD = "2509";

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


void adminMenu(vector<Question>& allQuestions) {
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
        
        cout << "\n--- Cau hoi " << (i + 1) << " (Level: " << q.level << ") ---" << endl;
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

    cout << "Nhap dap an A (VD: A. Noi dung): ";
    getline(cin, newQ.options[0]); 
    cout << "Nhap dap an B (VD: B. Noi dung): ";
    getline(cin, newQ.options[1]);
    cout << "Nhap dap an C (VD: C. Noi dung): ";
    getline(cin, newQ.options[2]);
    cout << "Nhap dap an D (VD: D. Noi dung): ";
    getline(cin, newQ.options[3]); 

    cout << "Nhap dap an dung (A, B, C hay D): ";
    cin >> newQ.answer;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
    newQ.answer = toupper(newQ.answer[0]);

    allQuestions.push_back(newQ);
    sort(allQuestions.begin(), allQuestions.end(), compareLevels);
    saveQuestions(allQuestions);

    setColor(10);
    cout << "\nDa them va luu cau hoi moi thanh cong!" << endl;
    resetColor();
    pressEnterToContinue();
}


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

    cout << "Day la danh sach tat ca cau hoi hien co:" << endl;
    for (int i = 0; i < allQuestions.size(); ++i) {
        const Question& q = allQuestions[i];
        cout << "\n--- " << (i + 1) << ". (Level: " << q.level << ") ---" << endl;
        cout << q.questionText << endl;
    }
    
    cout << "\n========================================" << endl;
    cout << "Nhap so thu tu (1, 2, ...) cua cau hoi ban muon XOA (Nhap 0 de huy): ";
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
        cout << "Da huy xoa." << endl;
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