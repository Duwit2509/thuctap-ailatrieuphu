#ifndef ADMIN_H
#define ADMIN_H

#include <vector>
#include "json_handler.h"
using namespace std;

void adminLogin(vector<Question>& allQuestions);
void adminMenu(vector<Question>& allQuestions);
void viewAllQuestions(const vector<Question>& allQuestions);
void addQuestion(vector<Question>& allQuestions);
void deleteQuestion(vector<Question>& allQuestions);
void editQuestion(vector<Question>& allQuestions);

#endif