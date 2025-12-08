#ifndef HELPERS_H
#define HELPERS_H

#include <string>
using std::string;

void setColor(int color);
void resetColor();
void clearScreen();
void sleep(int milliseconds);
void pressEnterToContinue();
void playSelectSound();
void playCorrectSound();
void playWrongSound();
void playNextQuestionSound();
void playWinsStepSound();
void playIntroSound();
void playEndGameSound();
void playWelcomeSound();
void playApply5050Sound();
void playApplyAskAudienceSound();
void playApplyPhoneAFriendSound();
void playPalpitatingSound();

string formatMoney(long long money);
string wrapText(const string& text, int width);

#endif
