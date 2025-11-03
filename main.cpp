#include <iostream>  
#include <fstream>   
#include <vector>    
#include <string>    
#include "json.hpp"
using namespace std;
using json = nlohmann::json; 

// --- GIAI DOAN 1: XAY DUNG LOI ---
struct Question {
    int level;
    string questionText;
    vector<string> options; // Dung vector de luu 4 lua chon
    string answer;          // Luu dap an dung (vi du: "B")
};

// Viet ham de doc file questions.json va tra ve 1 danh sach cau hoi
vector<Question> loadQuestions() {
    // Mo file de doc
    ifstream file_in("questions.json");

    // Neu mo file that bai (vi du: sai ten file), thong bao loi
    if (!file_in.is_open()) {
        cerr << "LOI: Khong the mo file questions.json. Vui long kiem tra lai!" << endl;
        // Tra ve mot danh sach rong
        return {}; 
    }

    // Tao mot doi tuong JSON 'j'
    json j;

    // Doc tat ca noi dung tu file vao doi tuong 'j'
    file_in >> j;

    // Dong file lai (rat quan trong)
    file_in.close();

    // Tao mot vector rong de luu danh sach cau hoi
    vector<Question> questionList;

    // Bat dau lap qua tung phan tu trong file JSON
    // (file JSON of chung ta la 1 cai mang lon [])
    for (auto& item : j) {
        Question q; // Tao 1 cau hoi moi

        // Day du lieu tu JSON vao bien 'q'
        // item["level"] phai khop voi "level" trong file JSON
        q.level = item["level"];
        q.questionText = item["question"];
        q.answer = item["answer"];
        
        // Dac biet: vi 'options' la mot mang, chung ta lay no nhu sau
        q.options = item["options"].get<vector<string>>();

        // Them cau hoi 'q' da hoan chinh vao danh sach
        questionList.push_back(q);
    }

    cout << "Da tai " << questionList.size() << " cau hoi tu file JSON." << endl;
    return questionList; // Tra ve toan bo danh sach cau hoi
}


int main() {
    // Goi ham de tai cau hoi
    vector<Question> allQuestions = loadQuestions();

    // Kiem tra xem da doc duoc chua
    if (allQuestions.empty()) {
        cout << "Khong co cau hoi nao duoc tai. Ket thuc chuong trinh." << endl;
        return 1; // Ket thuc voi ma loi
    }

    // In thu cau hoi dau tien ra de kiem tra
    cout << "\n--- KIEM TRA CAU HOI DAU TIEN ---" << endl;
    cout << "Level: " << allQuestions[0].level << endl;
    cout << "Cau hoi: " << allQuestions[0].questionText << endl;
    
    // In ra 4 lua chon
    for (const string& opt : allQuestions[0].options) {
        cout << opt << endl;
    }
    
    cout << "Dap an dung: " << allQuestions[0].answer << endl;

    return 0; // Ket thuc thanh cong
}