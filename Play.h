#ifndef PLAY_H
#define PLAY_H
#include <cctype>
#include "Chessboard.h"
#include "Engine.h"
#include "Notebook.h"
#include "Notice.h"
/**
 * @brief The Play class orchestrates a single game, managing multiple
 * components such as Notice, Chessboard, Engine, and Notebook.
 * 
 * This class is responsible for managing a single game, handling user input
 * for moves, and coordinating various components to execute the game logic.
 * It utilizes the Engine class to validate and execute moves, displaying
 * relevant messages using the Notice class based on the game situation.
 * If a move is valid, it updates the chessboard using the Chessboard class
 * and computes the next move by calling appropriate functions from the Engine class.
 * Additionally, it generate and write the game notation using the Notebook class,
 * documenting both user and AI moves. When the game concludes, it displays
 * the result and, upon user request, saves the game notation to a text file. 
 */
class Play{
    bool color;
    bool engineMoveUnderlighted;
    bool correcSquareChosen;
    bool correctMovementMade;

    int engineMoveCode;
    int userMoveCode;
    int userSquareChosenCoordinates;
    int promotionCode;

    Notice     notice;
    Chessboard chessboard;
    Engine     engine;
    Notebook   notebook;

    std::string currentChessboardUpdateCode;
    std::string previousChessboardUpdateCode;
    std::string SquareUpdateCode;
//********************************************************************************
public: Play(bool k)noexcept;
        void playWithUser();
private:    bool userMoveServive();
                int loadCoordinates();
                    bool endgameMenu(char cHar);
                        void colorfullElementSettingMenu();
                            void colorSettingMenu(globalType::Color &color);
                std::string generateSquareUpdateCode()noexcept;
                void updateChessboard(std::string updateCode, bool underlight);
                    int  getPieceCode (char cHar);
                    bool getPieceColor(char cHar);
                void offPreviousEngineMoveUnderlight();
                bool isChosenSquareCorrect();
                bool isUserMakesPromotion()noexcept;
            bool engineMoveServive(int moveCode);
                bool isItGameover();
            void notationSavingMenu();
};
#endif//PLAY_H