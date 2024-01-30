#ifndef MOVE_H
#define MOVE_H
#include "GlobalDefinitions.h"

class Move{
    friend class Engine;

    Move* baseMoveClassPointer;
    std::vector<std::vector<globalType::chessboardPointer>> movements;
    std::vector<double> potentialMovementMaterialStatus;

    int machineKingLocationX;
    int machineKingLocationY;
    int userKingLocationX;
    int userKingLocationY;

    int movementGeneration;
    int lastMovementGeneration =  1;
    int engineMoveNumber       = -1;
    int worstMoveIndex         =  0;
    int consideredEngineMovementsDepth[10]{12,7,4,1,1,1,1,1,1,1};
    int consideredUserMovementsDepth  [10]{6,3,2,1,1,1,1,1,1,1};

    double LeastMaterialStatusOfconsideredMovements = 100.0;

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
//********************************************************************************
    Move(bool k)noexcept;
   ~Move()noexcept;
    void prepareMove()noexcept;
    double findNextMove(globalType::chessboardPointer &wsk_X);
        void verifyKingsLocation(globalType::chessboardPointer wsk_X);
        void checkIfRooksAndKingsMoved(globalType::chessboardPointer wsk_X);
        void checkIfEngineRookChecked(globalType::chessboardPointer wsk_X);
        double findBestMove(globalType::chessboardPointer &wsk_X);
            double beginningSearchingTreeService(globalType::chessboardPointer &wsk_X);
                void endingSearchingService(globalType::chessboardPointer &wsk_X, int &index);
                    void checkIfGameFinishedByUser(globalType::chessboardPointer &wsk_X);
                    void checkIfGameFinishedByEngine(globalType::chessboardPointer &wsk_X, int &index);
            double endingSearchingTreeService()noexcept;
            void resetMovements()noexcept;
            Move(Move* wskaznik,double &status_materialny,globalType::chessboardPointer wsk_X, int iteration);
                void rewriteKingsAndRooksMovesData(Move* wsk);
    void makeEngineMoves(globalType::chessboardPointer wsk_X);
        void engineMovesService(int fromX, int fromY, int toX, int toY, char bierka, globalType::chessboardPointer wsk_X);
            void makeEngineMovesIfAllowed(int &y, int &x, globalType::chessboardPointer cOpy);
                bool checkIfEngineSquareCaptured(const int &x,const int &y, globalType::chessboardPointer cOpy);
                void discardWorstEngineMove();
    void makeUserMoves(globalType::chessboardPointer wsk_X);
        void userMovesService(int fromX, int fromY, int toX, int toY, char bierka, globalType::chessboardPointer wsk_X);
            void makeUserMovesIfAllowed(int &y, int &x, globalType::chessboardPointer cOpy);
                bool checkIfUserSquareCaptured(const int &x, const int &y, globalType::chessboardPointer cOpy);
            void discardWorstUserMove();
    double countMaterialStatus(const globalType::chessboardPointer wsk_X);
    globalType::chessboardPointer copyChessboard(const globalType::chessboardPointer oryginal);
};
#endif//MOVE_H








