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
public: bool canUserMakeSuchMove  (int userMoveCode);
private:    void decipherUserMove (int userMoveCode);
            bool isPieceOfUserOnStartingSquare();
            bool isPieceOfUserOnFinalSquare();
            bool isThisMoveExposesKingToCapture();
            bool ifAllowedMove();
public: int makeMove                             (int userMoveCode);
private:    void getEngineReadyForMove           (int userMoveCode);
                void markUserMoveOnChessboard    (int userMoveCode);
            void arrangementServiceAfterUserMove (int userMoveCode);
                void setArrangements(globalType::chessboardPointer ptr_X);
                bool checkIfArrangementRepeatedThirdTime();
                    bool checkIfControlNumberRepeatedThirdTime();
                    bool compareChessboards(globalType::chessboardPointer ptr_A, globalType::chessboardPointer ptr_B);
            void arrangementServiceAfterEngineMove();
            void makeOpeningMove      ()noexcept;
                void blackOpeningMove ()noexcept;
                void whiteOpeningMove ()noexcept;
                int  randomChance(int liczba_opcji)noexcept;
            void makeMiddlegameMove();
                void isItEndgameTime      ()noexcept;
                    void setKindOfEndgame ()noexcept;
                    void setUserKingSideLocation();
                    void setUserKingCornerLocation();
            void makeEndgameMove();
                void makeRooksAndQueensMateMove    ()noexcept;
                void makeSingleRookOrQueenMateMove ()noexcept;
            int engineMoveCoding                   ()noexcept;
                void findEngineMove                ()noexcept;
                int  encodeEngineMove              ()noexcept;
                void markEngineMoveOnChessboard    ()noexcept;
                int  isItGameOver                  ()noexcept;
    //********************************************************************************************
        struct Movement{
            int fromX,
                fromY,
                toX,
                toY;
        }u,e;//user/engine
};
#endif//ENGINE_H
