#ifndef MOVE_H
#define MOVE_H
#include "GlobalDefinitions.h"

class Move{
    friend class Engine;

    bool color;

    bool engineKingMoved;
    bool engineLeftRookMoved;
    bool engineRightRookMoved;
    bool userKingMoved;
    bool userLeftRookMoved;
    bool userRightRookMoved;

    bool engineKingChecked;
    bool userKingChecked;

    bool gameOverEngineWin         = false;
    bool gameOverUserWin           = false;
    bool gameOverStalemateByEngine = false;
    bool gameOverStalemateByUser   = false;
    bool gameOver                  = false;

    int engineKingLocationX;
    int engineKingLocationY;
    int userKingLocationX;
    int userKingLocationY;

    int movementGeneration;
    int lastMovementGeneration =  1;
    int engineMoveNumber       = -1;

    std::vector<std::vector<globalType::chessboardPointer>> movements;

    Move* baseMoveClassPointer;
    //********************************************************************************************
    Move(bool k) noexcept;
   ~Move()noexcept;
        void resetMovements()noexcept;
    double findNextMove                          (globalType::chessboardPointer &wsk_X);
        void verifyKingsLocation                 (globalType::chessboardPointer  wsk_X);
        void checkIfRooksAndKingsMoved           (globalType::chessboardPointer  wsk_X);
        void checkIfEngineKingChecked            (globalType::chessboardPointer  wsk_X);
        double findBestMove                      (globalType::chessboardPointer &wsk_X);
            double beginningSearchingTreeService (globalType::chessboardPointer &wsk_X);
                Move(Move* wskaznik,double &status_materialny,globalType::chessboardPointer wsk_X, int iteration);
                    void rewriteKingsAndRooksMovesData(Move* wsk);
                void checkmateAndStalemateSearching  (globalType::chessboardPointer &wsk_X, int &index);
                    void checkIfGameFinishedByUser   (globalType::chessboardPointer &wsk_X);
                    void checkIfGameFinishedByEngine (globalType::chessboardPointer &wsk_X, int &index);
            double endingSearchingTreeService();
                double countMiddlegameMaterialStatus (const globalType::chessboardPointer wsk_X);
                double countEndgameMaterialStatus    (const globalType::chessboardPointer wsk_X);
                    static double goOppositeDirectionOfUserKing   (double x, double y)noexcept;
                    static double goToSideOfUserKing              (double x, double y)noexcept;
                    static double separateUserKingFromRestOfBoard (double x, double y)noexcept;
                    static double makeNothing                     (double x, double y)noexcept;
                    static double runSidewaysFromKing             (double x, double y)noexcept;
                    static double followUserKingToSide            (double x, double y)noexcept;
                    static double followUserKingToCorner          (double x, double y)noexcept;
                    static double goToCornerOfUserKing            (double x, double y)noexcept;
                    //static double ooooooo(int x, int y);
    void makeEngineMoves(globalType::chessboardPointer wsk_X);
        void engineMovesService(int fromX, int fromY, int toX, int toY, char bierka, globalType::chessboardPointer wsk_X);
            void makeEngineMovesIfAllowed(int &y, int &x, globalType::chessboardPointer cOpy);
                bool checkIfEngineSquareCaptured(const int &x,const int &y, globalType::chessboardPointer cOpy);
    void makeUserMoves(globalType::chessboardPointer wsk_X);
        void userMovesService(int fromX, int fromY, int toX, int toY, char bierka, globalType::chessboardPointer wsk_X);
            void makeUserMovesIfAllowed(int &y, int &x, globalType::chessboardPointer cOpy);
                bool checkIfUserSquareCaptured(const int &x, const int &y, globalType::chessboardPointer cOpy);
    globalType::chessboardPointer copyChessboard(const globalType::chessboardPointer oryginal);
    void writeChessboard(const globalType::chessboardPointer wsk_X);
};
#endif//MOVE_H








