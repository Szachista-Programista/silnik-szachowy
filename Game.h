#ifndef GAME_H
#define GAME_H
#include <ctime>
#include "Play.h"

class Game{
        bool pastFirstPlay = false;

        Chessboard chessboard;
        Notice notice;
//********************************************************************************
public: Game();
        void start();
private:    void gameMenu();
                void userPiecesColorMenu();
                    int randomColor()noexcept;
                void colorfullElementSettingMenu();
                    void colorSettingMenu(globalType::Color &color);
                void languageSettingMenu();
                void exitService();
};
#endif//GAME_H