#ifndef ENGINE_H
#define ENGINE_H
#include <cmath>
#include "Move.h"
#include "Chessboard.h"//////////////////////////////////////////////////////
#include "Notice.h"///////////////////////////////////////////////////////////
class Engine{
private:Move movement;

        int promotionCode;
        int openingMoveNumber = 0;
        int movementNumber = 0;

        bool color;

        std::string userPiecesMovedInOpening;

        globalType::chessboardPointer comparativeChessboardPointer;
        globalType::chessboardPointer workingChessboardPointer;
    //********************************************************************************
public: Engine(bool k);
private:    globalType::chessboardPointer initializeChessboard();
public:~Engine()noexcept;
        bool canUserMakeSuchMove(int userMoveCode);
private:    void decipherUserMove(int userMoveCode);
            bool isPieceOfUserOnStartingSquare();
            bool isPieceOfUserOnFinalSquare();
            bool isThisMoveExposesKingToCapture();
            bool ifAllowedMove();
public: int makeMove(int userMoveCode);
private:    void getEngineReadyForMove(int userMoveCode);
                void markUserMoveOnChessboard(int userMoveCode);
            void makeOpeningMove()noexcept;
                void blachOpeningMove()noexcept;
                void whiteOpeningMove()noexcept;
                int  randomChance(int liczba_opcji)noexcept;
            void makeMiddlegameMove();
                void isItEndgameTime();
            void makeEndgameMove();
            int engineMoveCoding()noexcept;
                void findEngineMove()noexcept;
                int  encodeEngineMove()noexcept;
                void markEngineMoveOnChessboard()noexcept;
                int  isItGameOver()noexcept;

    //********************************************************************************************
        struct Movement{
            int fromX,
                fromY,
                toX,
                toY;
        }u,e;//user/engine

        enum GameStage{
            opening    = 1,
            middlegame = 2,
            endgame    = 3
        }gameStage = opening;//XXXXXXXXXXXXXXXXX

};
#endif//ENGINE_H
