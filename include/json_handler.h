#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H

#include <vector>
#include <string>
#include "json.hpp"
#include "game.h"

using namespace std;
using json = nlohmann::json;

// Ham JSON
vector<Question> loadQuestions();
void saveQuestions(const vector<Question>& allQuestions);
vector<Player> loadScores();
void saveScores(const vector<Player>& allPlayers);
bool compareLevels(const Question& a, const Question& b);

// Ham luu va load so tien nguoi choi
long long loadUserMoney(string name);
void saveUserMoney(string name, long long money);


#endif 