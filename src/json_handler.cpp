#include "../include/json_handler.h"
#include "../include/helpers.h"
#include <fstream>
#include <iostream>

vector<Question> loadQuestions() {
    ifstream file_in("questions.json");
    if (!file_in.is_open()) {
        cerr << "LỖI: Không thể mở file questions.json. Vui lòng kiểm tra lại!" << endl;
        return {}; 
    }
    json j;
    try {
        file_in >> j;
        if (j.is_null() || j.empty()) { 
             cout << "File questions.json đang rỗng, bắt đầu danh sách mới." << endl;
             file_in.close();
             return {};
        }
    } catch (json::parse_error& e) {
        cerr << "LỖI: File questions.json bị lỗi định dạng! " << e.what() << endl;
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
    cout << "Đã tải " << questionList.size() << " câu hỏi từ file JSON." << endl;
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
        cerr << "LỖI: Không thể mở file questions.json để LƯU!" << endl;
        return;
    }
    file_out << j_array.dump(4);
    file_out.close();
    cout << "Đã lưu thành công " << allQuestions.size() << " câu hỏi." << endl;
}

vector<Player> loadScores() {
    ifstream file_in("scores.json");
    vector<Player> allPlayers;
    if (!file_in.is_open()) {
        cout << "File scores.json không tìm thấy, sẽ tạo file mới." << endl;
        return allPlayers; 
    }
    json j;
    try {
        file_in >> j;
        if (j.is_null() || j.empty()) { 
             cout << "File scores.json đang rỗng, bắt đầu danh sách mới." << endl;
             file_in.close();
             return allPlayers;
        }
    } catch (json::parse_error& e) {
        cout << "File scores.json đang rỗng hoặc lỗi, bắt đầu danh sách mới." << endl;
        file_in.close();
        return allPlayers;
    }
    file_in.close();
    for (auto& item : j) {
        Player p;
        p.name = item["name"];
        if (item.contains("level")) {
                p.level = item["level"].get<int>();
            } else {
                p.level = 0; 
            }
        p.score = item["score"];
        allPlayers.push_back(p);
    }
    cout << "Đã tải " << allPlayers.size() << " lượt chơi trước." << endl;
    return allPlayers;
}

void saveScores(const vector<Player>& allPlayers) {
    json j_array = json::array();
    for (const Player& p : allPlayers) {
        json j_item;
        j_item["name"] = p.name;
        j_item["level"] = p.level;
        j_item["score"] = p.score;
        j_array.push_back(j_item);
    }
    ofstream file_out("scores.json");
    file_out << j_array.dump(4);
    file_out.close();
    cout << "Đã lưu điểm vào file scores.json." << endl;
}

bool compareLevels(const Question& a, const Question& b) {
    return a.level < b.level; 
}


// Doc tien cua rieng mot nguoi choi
long long loadUserMoney(string name) {
    ifstream file_in("userdata.json");
    if (!file_in.is_open()) return 0;
    
    json j;
    try {
        file_in >> j;
        if (j.contains(name)) {
            return j[name].get<long long>();
        }
    } catch (...) { return 0; }
    return 0; 
}

// Luu tien cho rieng mot nguoi choi
void saveUserMoney(string name, long long money) {
    json j;
    
     // Doc du lieu hien tai tu file 
    ifstream file_in("userdata.json");
    if (file_in.is_open()) {
        try {
            file_in >> j;
        } catch (...) {} // Neu loi thi coi nhu file rong
        file_in.close();
    }

    j[name] = money;
    ofstream file_out("userdata.json");
    file_out << j.dump(4);
}