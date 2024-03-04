#ifndef ENGINE_H
#define ENGINE_H
#include <cmath>
#include "Move.h"
/**
 * @brief The Engine class manages the entire game, handling opening,
 * middle game, and endgame moves.
 * 
 * This class contains an instance of the Move class and utilizes it
 * for generating moves. It orchestrates the entire game, starting with
 * opening moves and deciding when the opening phase ends. Once the opening
 * phase is complete, it executes middle game moves using the Move class.
 * Finally, when it determines that it's time for the endgame,
 * it performs endgame moves, also utilizing the Move class to generate moves
 * according to different rules aimed at checkmating the opponent.
 */
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