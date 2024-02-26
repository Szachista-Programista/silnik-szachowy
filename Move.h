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
    int lastMovementGeneration;
    int engineMoveNumber       = -1;

    std::vector<std::vector<globalType::chessboardPointer>> movements;

    Move* baseMoveClassPointer;
    //********************************************************************************************
    Move(bool k) noexcept;
   ~Move()noexcept;
        void resetMovements()noexcept;
    double findNextMove                          (globalType::chessboardPointer &ptr_X);
        void verifyKingsLocation                 (globalType::chessboardPointer  ptr_X);
        void checkIfRooksAndKingsMoved           (globalType::chessboardPointer  ptr_X);
        void checkIfEngineKingChecked            (globalType::chessboardPointer  ptr_X);
        void setSearchingDepth()noexcept;
        double findBestMove                      (globalType::chessboardPointer &ptr_X);
            double beginningSearchingTreeService (globalType::chessboardPointer &ptr_X);
                Move(Move* pointer,double &status_materialny,globalType::chessboardPointer ptr_X, int currentIterationNr, int lastIterationNr);
                    void rewriteKingsAndRooksMovesData(Move* ptr);
                void checkmateAndStalemateSearching                    (globalType::chessboardPointer &ptr_X, int &index);
                    void insufficientMaterialServiceAfterUserMove      (globalType::chessboardPointer  ptr_X);
                        bool checkIfGameFinishedByInsufficientMaterial (globalType::chessboardPointer  ptr_X);
                    void insufficientMaterialServiceAfterEngineMove    (globalType::chessboardPointer  ptr_X);
                    void checkIfGameFinishedByUser                     (globalType::chessboardPointer &ptr_X);
                    void checkIfGameFinishedByEngine                   (globalType::chessboardPointer &ptr_X, int &index);
            double endingSearchingTreeService();
                double countMiddlegameMaterialStatus (const globalType::chessboardPointer ptr_X);
                double countEndgameMaterialStatus    (const globalType::chessboardPointer ptr_X);
                    static double goOppositeDirectionOfUserKing   (double x, double y)noexcept;
                    static double goToSideOfUserKing              (double x, double y)noexcept;
                    static double goToCornerOfUserKing            (double x, double y)noexcept;
                    static double separateUserKingFromRestOfBoard (double x, double y)noexcept;
                    static double makeNothing                     (double x, double y)noexcept;
                    static double runSidewaysFromKing             (double x, double y)noexcept;
                    static double followUserKingToSide            (double x, double y)noexcept;
                    static double followUserKingToCorner          (double x, double y)noexcept;
    void makeEngineMoves                                                              (globalType::chessboardPointer ptr_X);
        void engineMovesService  (int fromX, int fromY, int toX, int toY, char bierka, globalType::chessboardPointer ptr_X);
            void makeEngineMovesIfAllowed                             (int &x, int &y, globalType::chessboardPointer cOpy);
                bool checkIfEngineSquareCaptured           (const int &x,const int &y, globalType::chessboardPointer cOpy);
    void makeUserMoves                                                                (globalType::chessboardPointer ptr_X);
        void userMovesService    (int fromX, int fromY, int toX, int toY, char bierka, globalType::chessboardPointer ptr_X);
            void makeUserMovesIfAllowed                               (int &x, int &y, globalType::chessboardPointer cOpy);
                bool checkIfUserSquareCaptured            (const int &x, const int &y, globalType::chessboardPointer cOpy);
    globalType::chessboardPointer copyChessboard                                (const globalType::chessboardPointer oryginal);
    void writeChessboard                                                        (const globalType::chessboardPointer ptr_X);
};
#endif//MOVE_H








