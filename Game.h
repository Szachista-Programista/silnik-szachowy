#ifndef GAME_H
#define GAME_H
#include <cstdlib>
#include <ctime>
#include "Play.h"

class Game{
private:Chessboard chessboard;
        Notice notice;
        bool pastFirstPlay = false;
//********************************************************************************
public: void start();
private:    void gameMenu();
                void runThePlay();
                    void userPiecesColorMenu(bool &color);
                        int randomColor()noexcept;
                void colorfullElementSettingMenu();
                    void colorSettingMenu(globalType::Color &color);
                void languageSettingMenu();
                void exitService();
};
#endif//GAME_H
