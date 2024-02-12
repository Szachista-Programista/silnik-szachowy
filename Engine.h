#ifndef ENGINE_H
#define ENGINE_H
#include <cmath>
#include "Move.h"
#include "Chessboard.h"//#####################################################
#include "Notice.h"//#####################################################

class Engine{
        bool color;

        int promotionCode;
        int movementNumber = 0;

        std::string userPiecesMovedInOpening;

        globalType::chessboardPointer comparativeChessboardPointer;
        globalType::chessboardPointer workingChessboardPointer;

        Move movement;
    //********************************************************************************
public: Engine(bool k);
private:    globalType::chessboardPointer loadPiecesArrangement();
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
                void blackOpeningMove()noexcept;
                void whiteOpeningMove()noexcept;
                int  randomChance(int liczba_opcji)noexcept;
            void makeMiddlegameMove();
                void isItEndgameTime()noexcept;
                    void setKindOfEndgame()noexcept;
                    void setUserKingSideLocation();
                    void setUserKingCornerLocation();
            void makeEndgameMove();
                void makeRooksAndQueensMateMove();
                void makeRookMateMove();
                void makeQueenMateMove();
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
};
#endif//ENGINE_H
