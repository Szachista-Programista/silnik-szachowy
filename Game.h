#ifndef GAME_H
#define GAME_H
#include <ctime>
#include "Play.h"
/**
 * @brief The Game class handles the overall management of the program, including
 * displaying the main menu, configuring program settings (language and color),
 * and initiating gameplay.
 * 
 * This class is responsible for managing the main functionality of the program.
 * It displays the main menu, allowing users to configure language settings
 * and color settings (yellow, red, green, blue) for various program elements
 * such as notation, highlighted squares, and displayed messages. Users
 * can also start a new game, choosing their preferred color (white, black,
 * or random), and then gameplay is delegated to the Play class, which handles
 * the specifics of each game. After a game concludes, control returns to the
 * Game class for further actions or menu navigation.
 */
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