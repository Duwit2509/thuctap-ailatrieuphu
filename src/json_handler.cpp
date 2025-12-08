#include "../include/json_handler.h"
#include "../include/helpers.h"
#include <fstream>
#include <iostream>

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
        if (item.contains("level")) {
                p.level = item["level"].get<int>();
            } else {
                p.level = 0; 
            }
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
        j_item["level"] = p.level;
        j_item["score"] = p.score;
        j_array.push_back(j_item);
    }
    ofstream file_out("scores.json");
    file_out << j_array.dump(4);
    file_out.close();
    cout << "Da luu diem so vao file scores.json." << endl;
}

bool compareLevels(const Question& a, const Question& b) {
    return a.level < b.level; 
}