#ifndef PLAY_H
#define PLAY_H
#include <cctype>
#include "Chessboard.h"
#include "Engine.h"
#include "Notebook.h"
#include "Notice.h"

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
                    int  getPieceCode  (char cHar);
                    bool getPieceColor (char cHar);
                void offPreviousEngineMoveUnderlight();
                bool isChosenSquareCorrect();
                bool isUserMakesPromotion()noexcept;
            bool engineMoveServive(int moveCode);
                bool isItGameover();
            void notationSavingMenu();
};
#endif//PLAY_H






