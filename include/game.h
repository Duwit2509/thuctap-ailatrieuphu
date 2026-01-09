#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
using namespace std;

struct Question {
    int level;
    string questionText;
    vector<string> options;
    string answer;
};

struct Player {
    string name;
    long long score;
    int level; 
};

//Ham lay cau hoi ngau nhien theo level
Question getRandomQuestionByLevel(const vector<Question>& allQuestions, int level);

//Ham hien thi cau hoi
void displayQuestion(const Question& q, long long currentPrize, bool used5050, bool usedAskAudience, bool usedPhoneAFriend, char selectedOption = ' ');

//Ham choi game
long long playGame(const vector<Question>& allQuestions, int &finalLevel);

//Ham tro giup 
void apply5050(Question& currentQuestion);
void applyAskAudience(const Question& currentQuestion);
void applyCallWiseMan(const Question& currentQuestion);

//Ham dem nguoc thoi gian tra loi
char getUserInputWithTimer(int& timeLeft);

//Ham hien thi Bang xep hang
void viewHighScores(vector<Player>& allPlayers);
bool compareScores(const Player& a, const Player& b);

//Ham dinh dang tien te VND
string formatMoney(long long money);

//Ham xuong dong van ban
string wrapText(const string& text, int width); 

void viewGameRules();

#endif