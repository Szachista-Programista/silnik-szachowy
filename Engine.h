#ifndef ENGINE_H
#define ENGINE_H
#include <cmath>
#include "Move.h"

class Engine{
    bool color;

    int promotionCode;
    int movementNumber = 0;

    std::vector<globalType::chessboardPointer>arrangements;
    std::vector<uint64_t>controlNumbersOfArrangements;

    std::string userPiecesMovedInOpening;

    globalType::chessboardPointer comparativeChessboardPointer;
    globalType::chessboardPointer workingChessboardPointer;

    Move movement;
//********************************************************************************
public: Engine(bool k);
private:    globalType::chessboardPointer loadPiecesArrangement();
public:~Engine()noexcept;
private:    void clearArrangements()noexcept;
public: bool canUserMakeSuchMove(int userMoveCode);
private:    void decipherUserMove(int userMoveCode);
            bool isPieceOfUserOnStartingSquare();
            bool isPieceOfUserOnFinalSquare();
            bool isThisMoveExposesKingToCapture();
            bool isAllowedMove();
                bool isAllowedMoveByPawn();
                bool isAllowedMoveByKnight();
                bool isAllowedSlantMove();
                bool isAllowedNonslantMove();
                bool isAllowedMovebyKing();
public: int makeMove(int userMoveCode);
private:    void getEngineReadyForMove(int userMoveCode);
                void markUserMoveOnChessboard(int userMoveCode);
            void arrangeServiceAfterUserMove(int userMoveCode);
                void setArrangements(globalType::chessboardPointer ptr_X);
                bool isArrangementRepeatedThirdTime();
                    bool isControlNumberRepeatedThirdTime();
                    bool compareChessboards(globalType::chessboardPointer ptr_A, globalType::chessboardPointer ptr_B);
            void arrangeServiceAfterEngineMove();
            void makeOpeningMove()noexcept;
                void blackOpeningMove()noexcept;
                void whiteOpeningMove()noexcept;
                int  randomChance(int liczba_opcji)noexcept;
            void makeMiddlegameMove();
                void isItEndgameTime()noexcept;
                    void setUserKingSideLocation()noexcept;
                    void setUserKingCornerLocation()noexcept;
            void makeEndgameMove();
                void setKindOfEndgame()noexcept;
                void makeRooksAndQueensMateMove()noexcept;
                void makeSingleRookOrQueenMateMove()noexcept;
                void makeUnspecifiedMateMove()noexcept;
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