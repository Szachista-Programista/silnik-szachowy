#ifndef MOVE_H
#define MOVE_H
#include "GlobalDefinitions.h"
/**
 * @brief The Move class represents an artificial intelligence player that
 * searches the entire game tree and selects the best move among the considered ones.
 * 
 * This class receives the move made by the user and generates the move to be made
 * by the AI player in response. It searches the entire game tree to evaluate possible
 * moves and selects the best one. In summary, it handles the generation of AI moves
 * in response to user moves.
 */
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
    double findNextMove          (globalType::chessboardPointer &ptr_X);
        void verifyKingsLocation (globalType::chessboardPointer  ptr_X);
        void isRooksAndKingsMoved(globalType::chessboardPointer  ptr_X);
        void isEngineKingChecked (globalType::chessboardPointer  ptr_X);
        void setSearchingDepth()noexcept;
        double findBestMove                     (globalType::chessboardPointer &ptr_X);
            double beginningSearchingTreeService(globalType::chessboardPointer &ptr_X);
                Move(Move* pointer,double &status_materialny,globalType::chessboardPointer ptr_X, int currentIterationNr, int lastIterationNr);
                    void rewriteKingsAndRooksMovesData(Move* ptr);
                void checkmateAndStalemateSearching                (globalType::chessboardPointer &ptr_X, int &index);
                    void insufficientMaterialServiceAfterUserMove  (globalType::chessboardPointer  ptr_X);
                        bool isGameFinishedByInsufficientMaterial  (globalType::chessboardPointer  ptr_X);
                    void insufficientMaterialServiceAfterEngineMove(globalType::chessboardPointer  ptr_X);
                    void isGameFinishedByUser                      (globalType::chessboardPointer &ptr_X);
                    void isGameFinishedByEngine                    (globalType::chessboardPointer &ptr_X, int &index);
            double endingSearchingTreeService();
                double countMiddlegameMaterialStatus(const globalType::chessboardPointer  ptr_X);
                double countEndgameMaterialStatus   (const globalType::chessboardPointer  ptr_X);
                    static double goOppositeDirectionOfUserKing  (double x, double y)noexcept;
                    static double goToSideOfUserKing             (double x, double y)noexcept;
                    static double goToCornerOfUserKing           (double x, double y)noexcept;
                    static double separateUserKingFromRestOfBoard(double x, double y)noexcept;
                    static double makeNothing                    (double x, double y)noexcept;
                    static double runSidewaysFromKing            (double x, double y)noexcept;
                    static double followUserKingToSide           (double x, double y)noexcept;
                    static double followUserKingToCorner         (double x, double y)noexcept;
    void makeEngineMoves(globalType::chessboardPointer ptr_X);
        void makeEnginePawnMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X);
        void makeEngineKnightMoves(int fromX, int fromY, globalType::chessboardPointer ptr_X);
        void makeEngineBishopMoves(int fromX, int fromY, globalType::chessboardPointer ptr_X);
        void makeEngineRookMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X);
        void makeEngineQueenMoves (int fromX, int fromY, globalType::chessboardPointer ptr_X);
        void makeEngineKingMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X);
            bool engineLinearMovesService   (int fromX, int fromY, int toX, int toY,   char piece, globalType::chessboardPointer ptr_X);
            void engineUsualKingMovesService(int fromX, int fromY, int directionX, int directionY, globalType::chessboardPointer ptr_X);
            void engineCastleService                                                 (int kingToX, globalType::chessboardPointer ptr_X);
            void engineMovesService           (int fromX, int fromY, int toX, int toY, char piece, globalType::chessboardPointer ptr_X);
                void makeEngineMovesIfAllowed                                     (int &x, int &y, globalType::chessboardPointer  cOpy);
                    bool isEngineSquareCaptured                        (const int &x,const int &y, globalType::chessboardPointer  cOpy);
    void makeUserMoves(globalType::chessboardPointer ptr_X);
        void makeUserPawnMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X);
        void makeUserKnightMoves(int fromX, int fromY, globalType::chessboardPointer ptr_X);
        void makeUserBishopMoves(int fromX, int fromY, globalType::chessboardPointer ptr_X);
        void makeUserRookMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X);
        void makeUserQueenMoves (int fromX, int fromY, globalType::chessboardPointer ptr_X);
        void makeUserKingMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X);
            bool userLinearMovesService     (int fromX, int fromY, int toX, int toY, char piece, globalType::chessboardPointer ptr_X);
            void userUsualKingMovesService(int fromX, int fromY, int directionX, int directionY, globalType::chessboardPointer ptr_X);
            void userCastleService                                                 (int kingToX, globalType::chessboardPointer ptr_X);
            void userMovesService           (int fromX, int fromY, int toX, int toY, char piece, globalType::chessboardPointer ptr_X);
                void makeUserMovesIfAllowed                                     (int &x, int &y, globalType::chessboardPointer  cOpy);
                    bool isUserSquareCaptured                       (const int &x, const int &y, globalType::chessboardPointer  cOpy);
    globalType::chessboardPointer copyChessboard                                          (const globalType::chessboardPointer oryginal);
    void writeChessboard                                                                  (const globalType::chessboardPointer ptr_X);
};
#endif//MOVE_H