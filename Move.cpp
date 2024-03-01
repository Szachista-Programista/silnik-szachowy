#include "Move.h"

Move::Move(bool k)noexcept: color{k}
{
    movementGeneration = 0;

    engineKingLocationX = color?3:4;
    engineKingLocationY =         7;
    userKingLocationX   = color?3:4;
    userKingLocationY   =         0;

    engineKingMoved      = false;
    engineLeftRookMoved  = false;
    engineRightRookMoved = false;
    userKingMoved        = false;
    userLeftRookMoved    = false;
    userRightRookMoved   = false;
}
Move::~Move()noexcept
{
    resetMovements();
}
    void Move::resetMovements()noexcept
{
    for(auto& oneDimensionalVector: movements)
        for(auto& singeMove: oneDimensionalVector)
            delete[]singeMove;
    movements.clear();
}
double Move::findNextMove          (globalType::chessboardPointer &ptr_X)
{
    try
    {
        if(ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    verifyKingsLocation(ptr_X);
    isRooksAndKingsMoved(ptr_X);
    isEngineKingChecked(ptr_X);
    makeEngineMoves(ptr_X);
    setSearchingDepth();
    return findBestMove(ptr_X);
}
    void Move::verifyKingsLocation (globalType::chessboardPointer  ptr_X)
{
    try
    {
        if(ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    for(int i=7; i>=0; i--)
        for(int j=0; j<8; j++)
            if(ptr_X[i][j]=='K')
            {
                engineKingLocationX = j;
                engineKingLocationY = i;
                break;
            }
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            if(ptr_X[i][j]=='k')
            {
                userKingLocationX = j;
                userKingLocationY = i;
                break;
            }
}
    void Move::isRooksAndKingsMoved(globalType::chessboardPointer  ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(ptr_X[7][0] != 'R') engineLeftRookMoved  = true;
    if(ptr_X[7][7] != 'R') engineRightRookMoved = true;
    if(ptr_X[0][0] != 'r') userLeftRookMoved    = true;
    if(ptr_X[0][7] != 'r') userRightRookMoved   = true;
    if(color? ptr_X[7][3] != 'K': ptr_X[7][4] != 'K') engineKingMoved = true;
    if(color? ptr_X[0][3] != 'k': ptr_X[0][4] != 'k') userKingMoved   = true;
}
    void Move::isEngineKingChecked (globalType::chessboardPointer  ptr_X)
{
    engineKingChecked = isEngineSquareCaptured(engineKingLocationX, engineKingLocationY, ptr_X);
}
    void Move::setSearchingDepth()noexcept
{
    if(movementGeneration == 0)
    {
        int movementsNumber{};
        for(auto& userMovements: movements)
            for(auto& singleMove: userMovements)
                movementsNumber++;
        if(movementsNumber < 200 && !engineKingChecked)
            lastMovementGeneration = 2;
        else
            lastMovementGeneration = 1;
    }
}
    double Move::findBestMove                     (globalType::chessboardPointer &ptr_X)
{
    if(movementGeneration <  lastMovementGeneration) return beginningSearchingTreeService(ptr_X);
    if(movementGeneration == lastMovementGeneration) return endingSearchingTreeService();
}
        double Move::beginningSearchingTreeService(globalType::chessboardPointer &ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of Move pointer.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    //the engine finds the best move for itself (the highest status for the engine)
    //among the best moves for the user (the worst status for the engine)
    double bestMaterialStatus = -100.0;
    int bestMaterialStatusIndex{};
    for(int i=0; i<movements.size(); i++)//the best engine move is found here
    {
        double worstMaterialStatus = 110.0;
        double currentMaterialStatus{};
        for(int j=1; j<movements[i].size(); j++)//the best user move is found here
        {
            Move(this, currentMaterialStatus, movements[i][j], movementGeneration+1, lastMovementGeneration);
            if(currentMaterialStatus < worstMaterialStatus)
                worstMaterialStatus = currentMaterialStatus;
        }
        if(worstMaterialStatus == 110.0)
            if(isUserSquareCaptured(userKingLocationX, userKingLocationY, movements[i][0]))
            {
                bestMaterialStatus = 100.0;
                break;
            }
            else
                break;
        if(bestMaterialStatus < worstMaterialStatus)
        {
            bestMaterialStatus = worstMaterialStatus;
            bestMaterialStatusIndex = i;
        }
    }
    checkmateAndStalemateSearching(ptr_X, bestMaterialStatusIndex);
    resetMovements();
    return bestMaterialStatus;
}
            Move::Move(Move* pointer, double &materialStatus, globalType::chessboardPointer ptr_X, int currentIterationNr, int lastIterationNr):
            movementGeneration {currentIterationNr},lastMovementGeneration{lastIterationNr}
{
    rewriteKingsAndRooksMovesData(pointer);
    materialStatus = findNextMove(ptr_X);
}
                void Move::rewriteKingsAndRooksMovesData(Move* ptr)
{
    try
    {
        if (ptr == nullptr)
            throw std::invalid_argument("Nullptr of Move pointer.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    engineKingMoved      = ptr -> engineKingMoved;
    engineLeftRookMoved  = ptr -> engineLeftRookMoved;
    engineRightRookMoved = ptr -> engineRightRookMoved;
    userKingMoved        = ptr -> userKingMoved;
    userLeftRookMoved    = ptr -> userLeftRookMoved;
    userRightRookMoved   = ptr -> userRightRookMoved;
}
            void Move::checkmateAndStalemateSearching                (globalType::chessboardPointer &ptr_X, int &index)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of ptr_X pointer.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(movementGeneration == 0)
    {
        insufficientMaterialServiceAfterUserMove(ptr_X);
        if( ! gameOver)
            isGameFinishedByUser(ptr_X);
        delete[]ptr_X;
        ptr_X = nullptr;
        if( ! gameOver)
            isGameFinishedByEngine(ptr_X, index);
        if( ! gameOver)
        {
            ptr_X = copyChessboard(movements[index][0]);
            insufficientMaterialServiceAfterEngineMove(ptr_X);
        }
    }
}
                void Move::insufficientMaterialServiceAfterUserMove  (globalType::chessboardPointer  ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of ptr_X pointer.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(isGameFinishedByInsufficientMaterial(ptr_X))
    {
        gameOver = true;
        gameOverStalemateByUser = true;
    }
}
                    bool Move::isGameFinishedByInsufficientMaterial  (globalType::chessboardPointer  ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of ptr_X pointer.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    int nrOfKnights     {};
    int nrOfBishops     {};
    int nrOfOtherPieces {};
    try
    {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                switch(ptr_X[i][j])
                {
                    case ' ':                    break;
                    case 'n':
                    case 'N': nrOfKnights++;     break;
                    case 'b':
                    case 'B': nrOfBishops++;     break;
                    case 'p':
                    case 'P':
                    case 'r':
                    case 'R':
                    case 'q':
                    case 'Q':
                    case 'k':
                    case 'K': nrOfOtherPieces++; break;
                    default: throw std::runtime_error("Wrong char.");
                }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(2 < nrOfOtherPieces) return false;
    else if(nrOfKnights == 1 && nrOfBishops == 0) return true;
    else if(nrOfKnights == 0 && nrOfBishops == 1) return true;
    else if(nrOfKnights == 0 && nrOfBishops == 0) return true;
    else return false;
}
                void Move::insufficientMaterialServiceAfterEngineMove(globalType::chessboardPointer  ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of ptr_X pointer.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(isGameFinishedByInsufficientMaterial(ptr_X))
    {
        gameOver = true;
        gameOverStalemateByEngine = true;
    }
}
                void Move::isGameFinishedByUser                      (globalType::chessboardPointer &ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of ptr_X pointer.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(movements.size() == 0)
    {
        if(isEngineSquareCaptured(engineKingLocationX, engineKingLocationY, ptr_X))
        {
            gameOverUserWin = true;
            gameOver = true;
        }
        else
        {
            gameOverStalemateByUser = true;
            gameOver = true;
        }
    }
}
                void Move::isGameFinishedByEngine                    (globalType::chessboardPointer &ptr_X, int &index)
{
    for(int i=0, j=0; i<movements.size(); i++)
        if(movements[i].size()==1) // did the engine checkmate the user or cause a stalemate?
        {
            j++;
            if(index == i)
                for(int k=0; k<movements.size(); k++)
                    if(movements[k].size() > 1 )
                        index = k;
            if(isUserSquareCaptured(userKingLocationX, userKingLocationY, movements[i][0]))
            {
                ptr_X = copyChessboard(movements[i][0]);
                gameOverEngineWin = true;
                gameOver = true;
                break;
            }
            else if(j == movements.size())
            {
                ptr_X = copyChessboard(movements[i][0]);
                gameOverStalemateByEngine = true;
                gameOver = true;
                break;
            }
        }
}
        double Move::endingSearchingTreeService()
{
    //the engine finds the best move for itself (the highest status for the engine)
    //among the best moves for the user (the worst status for the engine)
    double bestMaterialStatus = -100.0;
    for(int i=0; i<movements.size(); i++)//the best engine move is found here
    {
        double worstMaterialStatus = 100.0;
        double currentMaterialStatus{};
        for(int j=1; j<movements[i].size(); j++)//the best user move is found here
        {
            if(globalType::gameStage == globalType::middlegame)
                currentMaterialStatus = countMiddlegameMaterialStatus(movements[i][j]);
            if(globalType::gameStage == globalType::endgame)
                currentMaterialStatus = countEndgameMaterialStatus(movements[i][j]);
            if(currentMaterialStatus < worstMaterialStatus)
                worstMaterialStatus = currentMaterialStatus;
        }
        if(worstMaterialStatus == 100.0)
            if(isUserSquareCaptured(userKingLocationX, userKingLocationY, movements[i][0]))
            {
                bestMaterialStatus = 100.0;
                break;
            }
            else
                break;
        if(bestMaterialStatus < worstMaterialStatus)
            bestMaterialStatus = worstMaterialStatus;
    }
    resetMovements();
    return bestMaterialStatus;
}
            double Move::countMiddlegameMaterialStatus(const globalType::chessboardPointer  ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    double materialStatus  = 0.0;
    double pawnValue       = 1.0;
    double sideKnightValue = 2.9;
    double knightValue     = 3.0;
    double sideBishopValue = 2.9;
    double bishopValue     = 3.0;
    double rookValue       = 5.0;
    double queenValue      = 9.0;
    try
    {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                switch(ptr_X[i][j])
                {
                    case ' ': break;
                    case 'k': break;
                    case 'K': break;

                    case 'p': materialStatus += /*(i==6)? -pawnValue :*/ -pawnValue; break;
                    case 'n': materialStatus += (i==0 || i==7 || j==0 || j==7)? -sideKnightValue : -knightValue; break;
                    case 'b': materialStatus += (i==0 || i==7 || j==0 || j==7)? -sideBishopValue : -bishopValue; break;
                    case 'r': materialStatus += -rookValue;  break;
                    case 'q': materialStatus += -queenValue; break;

                    case 'P': materialStatus += /*(i==1)? pawnValue :*/ pawnValue; break;
                    case 'N': materialStatus += (i==0 || i==7 || j==0 || j==7)? sideKnightValue : knightValue; break;
                    case 'B': materialStatus += (i==0 || i==7 || j==0 || j==7)? sideBishopValue : bishopValue; break;
                    case 'R': materialStatus +=  rookValue;  break;
                    case 'Q': materialStatus +=  queenValue; break;

                    default: std::cout<<ptr_X[i][j]; throw std::runtime_error("Wrong sign.");
                }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    return materialStatus;
}
            double Move::countEndgameMaterialStatus   (const globalType::chessboardPointer  ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    double materialStatus = 0.0;
    double pawnValue      = 1.0;
    double knightValue    = 3.0;
    double bishopValue    = 3.0;
    double rookValue      = 5.0;
    double queenValue     = 9.0;

    globalType::userKingX = userKingLocationX;
    globalType::userKingY = userKingLocationY;
    try
    {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                switch(ptr_X[i][j])
                {
                    case ' ': break;
                    case 'p': materialStatus += -pawnValue;   break;
                    case 'n': materialStatus += -knightValue; break;
                    case 'b': materialStatus += -bishopValue; break;
                    case 'r': materialStatus += -rookValue;   break;
                    case 'q': materialStatus += -queenValue;  break;
                    case 'k': materialStatus +=                *globalType::userKingBehaviorPriority1    * globalType::userKingBehaviorPoints1(j,i);    break;
                    case 'P': materialStatus +=  pawnValue;   break;
                    case 'N': materialStatus +=  knightValue + *globalType::engineKnightBehaviorPriority * globalType::engineKnightBehaviorPoints(j,i); break;
                    case 'B': materialStatus +=  bishopValue + *globalType::engineBishopBehaviorPriority * globalType::engineBishopBehaviorPoints(j,i); break;
                    case 'R': materialStatus +=  rookValue   + *globalType::engineRookBehaviorPriority1  * globalType::engineRookBehaviorPoints1(j,i)
                                                             + *globalType::engineRookBehaviorPriority2  * globalType::engineRookBehaviorPoints2(j,i);  break;
                    case 'Q': materialStatus +=  queenValue  + *globalType::engineQueenBehaviorPriority1 * globalType::engineQueenBehaviorPoints1(j,i)
                                                             + *globalType::engineQueenBehaviorPriority2 * globalType::engineQueenBehaviorPoints2(j,i); break;
                    case 'K': materialStatus +=                *globalType::engineKingBehaviorPriority   * globalType::engineKingBehaviorPoints(j,i);   break;
                    default: throw std::runtime_error("Wrong sign.");
                }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    return materialStatus;
}
                double Move::goOppositeDirectionOfUserKing  (double x, double y)noexcept
{
    if(globalType::userKingSideLocation == globalType::engineRightSide)
        return 7.0-x;
    if(globalType::userKingSideLocation == globalType::engineDownSide)
        return 7.0-y;
    if(globalType::userKingSideLocation == globalType::engineLeftSide)
        return x;
    if(globalType::userKingSideLocation == globalType::engineUpSide)
        return y;
}
                double Move::goToSideOfUserKing             (double x, double y)noexcept
{
    if(globalType::userKingSideLocation == globalType::engineRightSide)
        return x;
    if(globalType::userKingSideLocation == globalType::engineDownSide)
        return y;
    if(globalType::userKingSideLocation == globalType::engineLeftSide)
        return 7.0-x;
    if(globalType::userKingSideLocation == globalType::engineUpSide)
        return 7.0-y;
}
                double Move::goToCornerOfUserKing           (double x, double y)noexcept
{
    if(globalType::userKingCornerLocation == globalType::engineUpRightCorner)
        return (x + 7.0-y)/2.0;
    if(globalType::userKingCornerLocation == globalType::engineDownRightCorner)
        return (x + y)/2.0;
    if(globalType::userKingCornerLocation == globalType::engineDownLeftCorner)
        return (7.0-x + y)/2.0;
    if(globalType::userKingCornerLocation == globalType::engineUpLeftCorner)
        return (7.0-x + 7.0-y)/2.0;
}
                double Move::separateUserKingFromRestOfBoard(double x, double y)noexcept
{
    if(globalType::userKingSideLocation == globalType::engineRightSide)
        return (x == globalType::userKingX-1.0)? 1.0: 0.0;
    if(globalType::userKingSideLocation == globalType::engineDownSide)
        return (y == globalType::userKingY-1.0)? 1.0: 0.0;
    if(globalType::userKingSideLocation == globalType::engineLeftSide)
        return (x == globalType::userKingX+1.0)? 1.0: 0.0;
    if(globalType::userKingSideLocation == globalType::engineUpSide)
        return (y == globalType::userKingY+1.0)? 1.0: 0.0;
}
                double Move::runSidewaysFromKing            (double x, double y)noexcept
{
    if(abs(globalType::userKingX - x) <= 1.0 && abs(globalType::userKingY - y) <= 1.0)
        if(globalType::userKingSideLocation % 2)
            if(3 <= abs(globalType::userKingY - y))
                return abs(globalType::userKingY - y);
            else
                return 0.0;
        else
            if(3 <= abs(globalType::userKingX - x))
                return abs(globalType::userKingX - x);
            else
                return 0.0;
    else
        return 0.0;
}
                double Move::followUserKingToSide           (double x, double y)noexcept
{
    double value{};

    if(globalType::userKingSideLocation == globalType::engineRightSide)
    {
        value += 7.0 - abs(globalType::userKingY       - y);
        value += 7.0 - abs(globalType::userKingX - 2.0 - x);
        if(globalType::userKingX-2 > x)
            value += -1.0;
        if(globalType::userKingX < x)
        {
            if(globalType::userKingY == y)
                value += -4.0;
            else if(1.0 == abs(globalType::userKingY - y))
                value += -2.0;
        }
        return value/2.0;
    }
    if(globalType::userKingSideLocation == globalType::engineDownSide)
    {
        value += 7.0 - abs(globalType::userKingY - 2.0 - y);
        value += 7.0 - abs(globalType::userKingX       - x);
        if(globalType::userKingY-2 > y)
            value += -1.0;
        if(globalType::userKingY < y)
        {
            if(globalType::userKingX == x)
                value += -4.0;
            else if(1.0 == abs(globalType::userKingX - x))
                value += -2.0;
        }
        return value/2.0;
    }
    if(globalType::userKingSideLocation == globalType::engineLeftSide)
    {
        value += 7.0 - abs(globalType::userKingY       - y);
        value += 7.0 - abs(globalType::userKingX + 2.0 - x);
        if(globalType::userKingX+2 < x)
            value += -1.0;
        if(globalType::userKingX > x)
        {
            if(globalType::userKingY == y)
                value += -4.0;
            else if(1.0 == abs(globalType::userKingY - y))
                value += -2.0;
        }
        return value/2.0;
    }
    if(globalType::userKingSideLocation == globalType::engineUpSide)
    {
        value += 7.0 - abs(globalType::userKingY + 2.0 - y);
        value += 7.0 - abs(globalType::userKingX       - x);
        if(globalType::userKingY+2 < y)
            value += -1.0;
        if(globalType::userKingY > y)
        {
            if(globalType::userKingX == x)
                value += -4.0;
            else if(1.0 == abs(globalType::userKingX - x))
                value += -2.0;
        }
        return value/2.0;
    }
}
                double Move::followUserKingToCorner         (double x, double y)noexcept
{
    double value{};

    if(globalType::userKingCornerLocation == globalType::engineUpRightCorner)
    {
        value += 7.0 - abs(globalType::userKingY + 1.0 - y);
        value += 7.0 - abs(globalType::userKingX - 1.0 - x);
        return value/2.0;
    }
    if(globalType::userKingCornerLocation == globalType::engineDownRightCorner)
    {
        value += 7.0 - abs(globalType::userKingY - 1.0 - y);
        value += 7.0 - abs(globalType::userKingX - 1.0 - x);
        return value/2.0;
    }
    if(globalType::userKingCornerLocation == globalType::engineDownLeftCorner)
    {
        value += 7.0 - abs(globalType::userKingY - 1.0 - y);
        value += 7.0 - abs(globalType::userKingX + 1.0 - x);
        return value/2.0;
    }
    if(globalType::userKingCornerLocation == globalType::engineUpLeftCorner)
    {
        value += 7.0 - abs(globalType::userKingY + 1.0 - y);
        value += 7.0 - abs(globalType::userKingX + 1.0 - x);
        return value/2.0;
    }
}
                double Move::makeNothing                    (double x, double y)noexcept
{
    return 0.0;
}
void Move::makeEngineMoves(globalType::chessboardPointer ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }

    engineMoveNumber = -1 ;
    for(int i=0; i<8; i++)
        for (int j: {3,4,2,5,1,6,0,7})
            switch(ptr_X[i][j])
            {
                case ' ': break;
                case 'Q': makeEngineQueenMoves (j, i, ptr_X); break;
                case 'N': makeEngineKnightMoves(j, i, ptr_X); break;
                case 'B': makeEngineBishopMoves(j, i, ptr_X); break;
                case 'R': makeEngineRookMoves  (j, i, ptr_X); break;
                case 'P': makeEnginePawnMoves  (j, i, ptr_X); break;
                case 'K': makeEngineKingMoves  (j, i, ptr_X); break;
                default: break;
            }
}
    void Move::makeEnginePawnMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    if(1<fromY && fromY<7)                                                  // pawn with no chance at promotion
    {
        if(ptr_X[fromY-1][fromX]==' ')                                               engineMovesService(fromX,fromY,fromX,  fromY-1,'P',ptr_X);  // move forward by 1
        if(fromY==6 && ptr_X[5][fromX]==' ' && ptr_X[4][fromX]==' ')                 engineMovesService(fromX,fromY,fromX,  4,      'P',ptr_X);  // move forward by 2
        if(fromX!=0 && 'b'<=ptr_X[fromY-1][fromX-1] && ptr_X[fromY-1][fromX-1]<='r') engineMovesService(fromX,fromY,fromX-1,fromY-1,'P',ptr_X);  // left capture
        if(fromX!=7 && 'b'<=ptr_X[fromY-1][fromX+1] && ptr_X[fromY-1][fromX+1]<='r') engineMovesService(fromX,fromY,fromX+1,fromY-1,'P',ptr_X);  // right capture
    }
    if(fromY==1)                                                       // promotion
    {
        if(ptr_X[0][fromX]==' ')                                // just promotion with the creation of a
        {
            engineMovesService(fromX,1,fromX,0,'N',ptr_X);//knight
            engineMovesService(fromX,1,fromX,0,'B',ptr_X);//bishop
            engineMovesService(fromX,1,fromX,0,'R',ptr_X);//rook
            engineMovesService(fromX,1,fromX,0,'Q',ptr_X);//queen
        }
        if(fromX!=0 && 'b'<=ptr_X[0][fromX-1] && ptr_X[0][fromX-1]<='r')// promotion by left capture with the creation of a
        {
            engineMovesService(fromX,1,fromX-1,0,'N',ptr_X);//knight
            engineMovesService(fromX,1,fromX-1,0,'B',ptr_X);//bishop
            engineMovesService(fromX,1,fromX-1,0,'R',ptr_X);//rook
            engineMovesService(fromX,1,fromX-1,0,'Q',ptr_X);//queen
        }
        if(fromX!=7 && 'b'<=ptr_X[0][fromX+1] && ptr_X[0][fromX+1]<='r')// promotion by right capture with the creation of a
        {
            engineMovesService(fromX,1,fromX+1,0,'N',ptr_X);//knight
            engineMovesService(fromX,1,fromX+1,0,'B',ptr_X);//bishop
            engineMovesService(fromX,1,fromX+1,0,'R',ptr_X);//rook
            engineMovesService(fromX,1,fromX+1,0,'Q',ptr_X);//queen
        }
    }
}
    void Move::makeEngineKnightMoves(int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    if(0<=fromY-2 && fromX+1<=7 && (ptr_X[fromY-2][fromX+1]<'B' || 'R'<ptr_X[fromY-2][fromX+1])) engineMovesService(fromX,fromY,fromX+1,fromY-2,'N',ptr_X);
    if(0<=fromY-1 && fromX+2<=7 && (ptr_X[fromY-1][fromX+2]<'B' || 'R'<ptr_X[fromY-1][fromX+2])) engineMovesService(fromX,fromY,fromX+2,fromY-1,'N',ptr_X);
    if(fromY+1<=7 && fromX+2<=7 && (ptr_X[fromY+1][fromX+2]<'B' || 'R'<ptr_X[fromY+1][fromX+2])) engineMovesService(fromX,fromY,fromX+2,fromY+1,'N',ptr_X);
    if(fromY+2<=7 && fromX+1<=7 && (ptr_X[fromY+2][fromX+1]<'B' || 'R'<ptr_X[fromY+2][fromX+1])) engineMovesService(fromX,fromY,fromX+1,fromY+2,'N',ptr_X);
    if(fromY+2<=7 && 0<=fromX-1 && (ptr_X[fromY+2][fromX-1]<'B' || 'R'<ptr_X[fromY+2][fromX-1])) engineMovesService(fromX,fromY,fromX-1,fromY+2,'N',ptr_X);
    if(fromY+1<=7 && 0<=fromX-2 && (ptr_X[fromY+1][fromX-2]<'B' || 'R'<ptr_X[fromY+1][fromX-2])) engineMovesService(fromX,fromY,fromX-2,fromY+1,'N',ptr_X);
    if(0<=fromY-1 && 0<=fromX-2 && (ptr_X[fromY-1][fromX-2]<'B' || 'R'<ptr_X[fromY-1][fromX-2])) engineMovesService(fromX,fromY,fromX-2,fromY-1,'N',ptr_X);
    if(0<=fromY-2 && 0<=fromX-1 && (ptr_X[fromY-2][fromX-1]<'B' || 'R'<ptr_X[fromY-2][fromX-1])) engineMovesService(fromX,fromY,fromX-1,fromY-2,'N',ptr_X);
}
    void Move::makeEngineBishopMoves(int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    for(int y=fromY-1, x=fromX+1; 0<=y && x<=7 && engineLinearMovesService(fromX,fromY,x,y,'B',ptr_X); y--, x++); //movement towards 1:30
    for(int y=fromY+1, x=fromX+1; y<=7 && x<=7 && engineLinearMovesService(fromX,fromY,x,y,'B',ptr_X); y++, x++); //movement towards 4:30
    for(int y=fromY+1, x=fromX-1; y<=7 && 0<=x && engineLinearMovesService(fromX,fromY,x,y,'B',ptr_X); y++, x--); //movement towards 7:30
    for(int y=fromY-1, x=fromX-1; 0<=y && 0<=x && engineLinearMovesService(fromX,fromY,x,y,'B',ptr_X); y--, x--); //movement towards 10:30
}
    void Move::makeEngineRookMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    for(int y=fromY-1; 0<=y && engineLinearMovesService(fromX,fromY,fromX,y,'R',ptr_X); y--); //movement towards 12:00
    for(int x=fromX+1; x<=7 && engineLinearMovesService(fromX,fromY,x,fromY,'R',ptr_X); x++); //movement towards 03:00
    for(int y=fromY+1; y<=7 && engineLinearMovesService(fromX,fromY,fromX,y,'R',ptr_X); y++); //movement towards 06:00
    for(int x=fromX-1; 0<=x && engineLinearMovesService(fromX,fromY,x,fromY,'R',ptr_X); x--); //movement towards 09:00

}
    void Move::makeEngineQueenMoves (int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    for(int y=fromY-1;            0<=y &&         engineLinearMovesService(fromX,fromY,fromX,y,    'Q',ptr_X); y--     ); //movement towards 12:00
    for(int y=fromY-1, x=fromX+1; 0<=y && x<=7 && engineLinearMovesService(fromX,fromY,x,    y,    'Q',ptr_X); y--, x++); //movement towards 01:30
    for(int            x=fromX+1; x<=7 &&         engineLinearMovesService(fromX,fromY,x,    fromY,'Q',ptr_X);      x++); //movement towards 03:00
    for(int y=fromY+1, x=fromX+1; y<=7 && x<=7 && engineLinearMovesService(fromX,fromY,x    ,y,    'Q',ptr_X); y++, x++); //movement towards 04:30
    for(int y=fromY+1;            y<=7 &&         engineLinearMovesService(fromX,fromY,fromX,y,    'Q',ptr_X); y++     ); //movement towards 06:00
    for(int y=fromY+1, x=fromX-1; y<=7 && 0<=x && engineLinearMovesService(fromX,fromY,x,    y,    'Q',ptr_X); y++, x--); //movement towards 07:30
    for(int            x=fromX-1; 0<=x &&         engineLinearMovesService(fromX,fromY,x,    fromY,'Q',ptr_X);      x--); //movement towards 09:00
    for(int y=fromY-1, x=fromX-1; 0<=y && 0<=x && engineLinearMovesService(fromX,fromY,x,    y,    'Q',ptr_X); y--, x--); //movement towards 10:30
}
    void Move::makeEngineKingMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    engineUsualKingMovesService(fromX,fromY, 0,-1,ptr_X); //movement towards 12:00
    engineUsualKingMovesService(fromX,fromY, 1,-1,ptr_X); //movement towards 01:30
    engineUsualKingMovesService(fromX,fromY, 1, 0,ptr_X); //movement towards 03:00
    engineUsualKingMovesService(fromX,fromY, 1, 1,ptr_X); //movement towards 04:30
    engineUsualKingMovesService(fromX,fromY, 0, 1,ptr_X); //movement towards 06:00
    engineUsualKingMovesService(fromX,fromY,-1, 1,ptr_X); //movement towards 07:30
    engineUsualKingMovesService(fromX,fromY,-1, 0,ptr_X); //movement towards 09:00
    engineUsualKingMovesService(fromX,fromY,-1,-1,ptr_X); //movement towards 10:30

    if(!color) //castling when the user play black
    {
        engineCastleService(2, ptr_X);
        engineCastleService(6, ptr_X);
    }
    if(color) //castling when the user play white
    {
        engineCastleService(1, ptr_X);
        engineCastleService(5, ptr_X);
    }
}
        bool Move::engineLinearMovesService   (int fromX, int fromY, int toX, int toY, char piece,   globalType::chessboardPointer ptr_X)
{
    try
    {
        if(fromX < 0 || 7 < fromX || fromY < 0 || 7 < fromY || toX < 0 || 7 < toX || toY < 0 || 7 < toY)
            throw std::invalid_argument("Chessboard coordinates out of range.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if('B'<=ptr_X[toY][toX] && ptr_X[toY][toX]<='R')
        return false;
    if('b'<=ptr_X[toY][toX] && ptr_X[toY][toX]<='r')
    {
        engineMovesService(fromX,fromY,toX,toY,piece,ptr_X);
        return false;
    }
    if(ptr_X[toY][toX]==' ')
    {
        engineMovesService(fromX,fromY,toX,toY,piece,ptr_X);
        return true;
    }
}
        void Move::engineUsualKingMovesService(int fromX, int fromY, int directionX, int directionY, globalType::chessboardPointer ptr_X)
{
    int toX = fromX + directionX;
    int toY = fromY + directionY;

    if (0 <= toX && toX <= 7 && 0 <= toY && toY <= 7)
    if (ptr_X[toY][toX] < 'B' || 'R' < ptr_X[toY][toX]) // is anything other than the engine piece on the target square
    if (2 <= abs(engineKingLocationX + directionX - userKingLocationX) || 2 <= abs(engineKingLocationY + directionY - userKingLocationY)) // are the kings too close to each other
    {
        engineKingLocationX = engineKingLocationX + directionX;
        engineKingLocationY = engineKingLocationY + directionY;
        engineMovesService(fromX,fromY,toX,toY,'K',ptr_X);
        engineKingLocationX = engineKingLocationX - directionX;
        engineKingLocationY = engineKingLocationY - directionY;
    }
}
        void Move::engineCastleService                                                 (int kingToX, globalType::chessboardPointer ptr_X)
{
    int kingFromY = 7;
    int kingFromX = (3 < kingToX)? (kingToX - 2): (kingToX + 2);
    int rookFromX = (3 < kingToX)? 7: 0;
    int rookToX   = (3 < kingToX)? (kingToX - 1): (kingToX + 1);

    if((engineKingMoved == false) && ((rookFromX == 0)? (engineLeftRookMoved  == false): true) && ((rookFromX == 7)? (engineRightRookMoved == false): true))
    if((ptr_X[7][kingToX]==' ') && (ptr_X[7][rookToX]==' ') && ((abs(rookFromX - kingToX) == 2)? (ptr_X[7][(rookFromX + kingToX)/2]==' '): true))
    {
        globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
        if((isEngineSquareCaptured(kingFromX, 7, cOpy) == false) && (isEngineSquareCaptured(kingToX,   7, cOpy) == false) && (isEngineSquareCaptured(rookToX,   7, cOpy) == false))
        {
            cOpy[7][rookFromX]=' ';
            cOpy[7][kingFromX]=' ';
            cOpy[7][kingToX]  ='K';
            cOpy[7][rookToX]  ='R';
            engineKingLocationX += (rookFromX == 0)? -2:  2;
            makeEngineMovesIfAllowed(kingFromX, kingFromY, cOpy);
            engineKingLocationX += (rookFromX == 0)?  2: -2;
        }
        else delete[]cOpy;
    }
}
        void Move::engineMovesService         (int fromX, int fromY, int toX, int toY, char piece,   globalType::chessboardPointer ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        if(fromX<0 || 7<fromX || fromY<0 || 7<fromY || toX<0 || 7<toX || toY<0 || 7<toY)
            throw std::invalid_argument("Chessboard coordinates out of range.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
    cOpy[fromY][fromX] = ' ';
    cOpy[toY][toX] = piece;
    makeEngineMovesIfAllowed(fromX, fromY, cOpy);
}
            void Move::makeEngineMovesIfAllowed                                     (int &x, int &y, globalType::chessboardPointer  cOpy)
{
    try
    {
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(engineKingChecked==false && x!=engineKingLocationX && y!=engineKingLocationY && (engineKingLocationX+engineKingLocationY!=x+y) && (engineKingLocationX-engineKingLocationY!=x-y))
    {//whether the king stands on the square line from which the move took place
        engineMoveNumber++;
        movements.push_back(std::vector<globalType::chessboardPointer>());
        movements[engineMoveNumber].push_back(cOpy);
        makeUserMoves(cOpy);
    }
    else if(isEngineSquareCaptured(engineKingLocationX, engineKingLocationY, cOpy)==false)
    {//If it's there, isn't it captured after all?
        engineMoveNumber++;
        movements.push_back(std::vector<globalType::chessboardPointer>());
        movements[engineMoveNumber].push_back(cOpy);
        makeUserMoves(cOpy);
    }
    else delete[]cOpy;
}
                bool Move::isEngineSquareCaptured                        (const int &x,const int &y, globalType::chessboardPointer  cOpy)
{
    try
    {
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    for(int i=1; 0<=y-i; i++)                                       // if capture from 12:00 direction
    {
        if(cOpy[y-i][x]==' ') continue;
        if(cOpy[y-i][x]=='q' || cOpy[y-i][x]=='r') return true;
        else break;
    }
    for(int i=1; 0<=y-i && x+i<=7; i++)                             // if capture from 01:30 direction
    {
        if(cOpy[y-i][x+i]==' ') continue;
        if(cOpy[y-i][x+i]=='q' || cOpy[y-i][x+i]=='b') return true;
        else break;
    }
    for(int i=1; x+i<=7; i++)                                       // if capture from 03:00 direction
    {
        if(cOpy[y][x+i]==' ') continue;
        if(cOpy[y][x+i]=='q' || cOpy[y][x+i]=='r') return true;
        else break;
    }
    for(int i=1; y+i<=7 && x+i<=7; i++)                             // if capture from 04:30 direction
    {
        if(cOpy[y+i][x+i]==' ') continue;
        if(cOpy[y+i][x+i]=='q' || cOpy[y+i][x+i]=='b') return true;
        else break;
    }
    for(int i=1; y+i<=7; i++)                                       // if capture from 06:00 direction
    {
        if(cOpy[y+i][x]==' ') continue;
        if(cOpy[y+i][x]=='q' || cOpy[y+i][x]=='r') return true;
        else break;
    }
    for(int i=1; y+i<=7 && 0<=x-i; i++)                             // if capture from 07:30 direction
    {
        if(cOpy[y+i][x-i]==' ') continue;
        if(cOpy[y+i][x-i]=='q' || cOpy[y+i][x-i]=='b') return true;
        else break;
    }
    for(int i=1; 0<=x-i; i++)                                       // if capture from 09:00 direction
    {
        if(cOpy[y][x-i]==' ') continue;
        if(cOpy[y][x-i]=='q' || cOpy[y][x-i]=='r') return true;
        else break;
    }
    for(int i=1; 0<=y-i && 0<=x-i; i++)                             // if capture from 10:30 direction
    {
        if(cOpy[y-i][x-i]==' ') continue;
        if(cOpy[y-i][x-i]=='q' || cOpy[y-i][x-i]=='b') return true;
        else break;
    }

    if(0<=y-2 && x+1<=7 && cOpy[y-2][x+1]=='n') return true;                       //captured by knight
    if(0<=y-1 && x+2<=7 && cOpy[y-1][x+2]=='n') return true;
    if(y+1<=7 && x+2<=7 && cOpy[y+1][x+2]=='n') return true;
    if(y+2<=7 && x+1<=7 && cOpy[y+2][x+1]=='n') return true;
    if(y+2<=7 && 0<=x-1 && cOpy[y+2][x-1]=='n') return true;
    if(y+1<=7 && 0<=x-2 && cOpy[y+1][x-2]=='n') return true;
    if(0<=y-1 && 0<=x-2 && cOpy[y-1][x-2]=='n') return true;
    if(0<=y-2 && 0<=x-1 && cOpy[y-2][x-1]=='n') return true;

    if(2<=y)                                                                    //captureb by pawn
    {
        if(1<=x && cOpy[y-1][x-1]=='p') return true;
        if(x<=6 && cOpy[y-1][x+1]=='p') return true;
    }
    return false;
}
void Move::makeUserMoves(globalType::chessboardPointer ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    userKingChecked = isUserSquareCaptured(userKingLocationX, userKingLocationY, ptr_X);
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            switch(ptr_X[i][j])
            {
                case ' ': break;
                case 'n': makeUserKnightMoves(j, i, ptr_X); break;
                case 'b': makeUserBishopMoves(j, i, ptr_X); break;
                case 'r': makeUserRookMoves  (j, i, ptr_X); break;
                case 'q': makeUserQueenMoves (j, i, ptr_X); break;
                case 'p': makeUserPawnMoves  (j, i, ptr_X); break;
                case 'k': makeUserKingMoves  (j, i, ptr_X); break;
            }
}
    void Move::makeUserPawnMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    if(0<fromY && fromY<6)                                                  //pawn with no chance at promotion
    {
        if(ptr_X[fromY+1][fromX]==' ')                                                userMovesService(fromX,fromY,fromX,fromY+1,  'p',ptr_X); //move forward by 1
        if(fromY==1 && ptr_X[2][fromX]==' ' && ptr_X[3][fromX]==' ')                  userMovesService(fromX,1,    fromX,3,        'p',ptr_X); //move forward by 2
        if(fromX!=0 && 'B'<=ptr_X[fromY+1][fromX-1] && ptr_X[fromY+1][fromX-1]<='R')  userMovesService(fromX,fromY,fromX-1,fromY+1,'p',ptr_X); //left capture
        if(fromX!=7 && 'B'<=ptr_X[fromY+1][fromX+1] && ptr_X[fromY+1][fromX+1]<='R')  userMovesService(fromX,fromY,fromX+1,fromY+1,'p',ptr_X); //right capture

    }
    if(fromY==6)                                                       //promotion
    {
        if(ptr_X[7][fromX]==' ')                              //just promotion with the creation of a
        {
            userMovesService(fromX,6,fromX,7,'n',ptr_X); //knight
            userMovesService(fromX,6,fromX,7,'b',ptr_X); //bishop
            userMovesService(fromX,6,fromX,7,'r',ptr_X); //rook
            userMovesService(fromX,6,fromX,7,'q',ptr_X); //queen
        }
        if(fromX!=0 && 'B'<=ptr_X[7][fromX-1] && ptr_X[7][fromX-1]<='R')//promotion by left capture with the creation of a
        {
            userMovesService(fromX,6,fromX-1,7,'n',ptr_X); //knight
            userMovesService(fromX,6,fromX-1,7,'b',ptr_X); //bishop
            userMovesService(fromX,6,fromX-1,7,'r',ptr_X); //rook
            userMovesService(fromX,6,fromX-1,7,'q',ptr_X); //queen
        }
        if(fromX!=7 && 'B'<=ptr_X[7][fromX+1] && ptr_X[7][fromX+1]<='R')//promotion by right capture with the creation of a
        {
            userMovesService(fromX,6,fromX+1,7,'n',ptr_X); //knight
            userMovesService(fromX,6,fromX+1,7,'b',ptr_X); //bishop
            userMovesService(fromX,6,fromX+1,7,'r',ptr_X); //rook
            userMovesService(fromX,6,fromX+1,7,'q',ptr_X); //queen
        }
    }
}
    void Move::makeUserKnightMoves(int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    if(0<=fromY-2 && fromX+1<=7 && (ptr_X[fromY-2][fromX+1]<'b' || 'r'<ptr_X[fromY-2][fromX+1]))  userMovesService(fromX,fromY,fromX+1,fromY-2,'n',ptr_X);
    if(0<=fromY-1 && fromX+2<=7 && (ptr_X[fromY-1][fromX+2]<'b' || 'r'<ptr_X[fromY-1][fromX+2]))  userMovesService(fromX,fromY,fromX+2,fromY-1,'n',ptr_X);
    if(fromY+1<=7 && fromX+2<=7 && (ptr_X[fromY+1][fromX+2]<'b' || 'r'<ptr_X[fromY+1][fromX+2]))  userMovesService(fromX,fromY,fromX+2,fromY+1,'n',ptr_X);
    if(fromY+2<=7 && fromX+1<=7 && (ptr_X[fromY+2][fromX+1]<'b' || 'r'<ptr_X[fromY+2][fromX+1]))  userMovesService(fromX,fromY,fromX+1,fromY+2,'n',ptr_X);
    if(fromY+2<=7 && 0<=fromX-1 && (ptr_X[fromY+2][fromX-1]<'b' || 'r'<ptr_X[fromY+2][fromX-1]))  userMovesService(fromX,fromY,fromX-1,fromY+2,'n',ptr_X);
    if(fromY+1<=7 && 0<=fromX-2 && (ptr_X[fromY+1][fromX-2]<'b' || 'r'<ptr_X[fromY+1][fromX-2]))  userMovesService(fromX,fromY,fromX-2,fromY+1,'n',ptr_X);
    if(0<=fromY-1 && 0<=fromX-2 && (ptr_X[fromY-1][fromX-2]<'b' || 'r'<ptr_X[fromY-1][fromX-2]))  userMovesService(fromX,fromY,fromX-2,fromY-1,'n',ptr_X);
    if(0<=fromY-2 && 0<=fromX-1 && (ptr_X[fromY-2][fromX-1]<'b' || 'r'<ptr_X[fromY-2][fromX-1]))  userMovesService(fromX,fromY,fromX-1,fromY-2,'n',ptr_X);
}
    void Move::makeUserBishopMoves(int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    for(int y=fromY-1, x=fromX+1; 0<=y && x<=7 && userLinearMovesService(fromX,fromY,x,y,'b',ptr_X); y--, x++); //movement towards 01:30
    for(int y=fromY+1, x=fromX+1; y<=7 && x<=7 && userLinearMovesService(fromX,fromY,x,y,'b',ptr_X); y++, x++); //movement towards 04:30
    for(int y=fromY+1, x=fromX-1; y<=7 && 0<=x && userLinearMovesService(fromX,fromY,x,y,'b',ptr_X); y++, x--); //movement towards 07:30
    for(int y=fromY-1, x=fromX-1; 0<=y && 0<=x && userLinearMovesService(fromX,fromY,x,y,'b',ptr_X); y--, x--); //movement towards 10:30

}
    void Move::makeUserRookMoves  (int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    for(int y=fromY-1; 0<=y && userLinearMovesService(fromX,fromY,fromX,y,    'r',ptr_X); y--); //movement towards 12:00
    for(int x=fromX+1; x<=7 && userLinearMovesService(fromX,fromY,x,    fromY,'r',ptr_X); x++); //movement towards 03:00
    for(int y=fromY+1; y<=7 && userLinearMovesService(fromX,fromY,fromX,y,    'r',ptr_X); y++); //movement towards 06:00
    for(int x=fromX-1; 0<=x && userLinearMovesService(fromX,fromY,x,    fromY,'r',ptr_X); x--); //movement towards 09:00
}
    void Move::makeUserQueenMoves (int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    for(int y=fromY-1;            0<=y &&         userLinearMovesService(fromX,fromY,fromX,y,'q',ptr_X); y--     ); //movement towards 12:00
    for(int y=fromY-1, x=fromX+1; 0<=y && x<=7 && userLinearMovesService(fromX,fromY,x,    y,'q',ptr_X); y--, x++); //movement towards 01:30
    for(int            x=fromX+1; x<=7 &&         userLinearMovesService(fromX,fromY,x,fromY,'q',ptr_X);      x++); //movement towards 03:00
    for(int y=fromY+1, x=fromX+1; y<=7 && x<=7 && userLinearMovesService(fromX,fromY,x,    y,'q',ptr_X); y++, x++); //movement towards 04:30
    for(int y=fromY+1;            y<=7 &&         userLinearMovesService(fromX,fromY,fromX,y,'q',ptr_X); y++     ); //movement towards 06:00
    for(int y=fromY+1, x=fromX-1; y<=7 && 0<=x && userLinearMovesService(fromX,fromY,x,    y,'q',ptr_X); y++, x--); //movement towards 07:30
    for(int            x=fromX-1; 0<=x &&         userLinearMovesService(fromX,fromY,x,fromY,'q',ptr_X);      x--); //movement towards 09:00
    for(int y=fromY-1, x=fromX-1; 0<=y && 0<=x && userLinearMovesService(fromX,fromY,x,    y,'q',ptr_X); y--, x--); //movement towards 10:30

}
    void Move::makeUserKingMoves (int fromX, int fromY, globalType::chessboardPointer ptr_X)
{
    userUsualKingMovesService(fromX,fromY, 0,-1,ptr_X); //movement towards 12:00
    userUsualKingMovesService(fromX,fromY, 1,-1,ptr_X); //movement towards 01:30
    userUsualKingMovesService(fromX,fromY, 1, 0,ptr_X); //movement towards 03:00
    userUsualKingMovesService(fromX,fromY, 1, 1,ptr_X); //movement towards 04:30
    userUsualKingMovesService(fromX,fromY, 0, 1,ptr_X); //movement towards 06:00
    userUsualKingMovesService(fromX,fromY,-1, 1,ptr_X); //movement towards 07:30
    userUsualKingMovesService(fromX,fromY,-1, 0,ptr_X); //movement towards 09:00
    userUsualKingMovesService(fromX,fromY,-1,-1,ptr_X); //movement towards 10:30
    if(!color) //castling when the user play black
    {
        userCastleService(2, ptr_X);
        userCastleService(6, ptr_X);
    }
    if(color) //castling when the user play white
    {
        userCastleService(1, ptr_X);
        userCastleService(5, ptr_X);
    }
}
        bool Move::userLinearMovesService   (int fromX, int fromY, int toX, int toY, char piece,   globalType::chessboardPointer ptr_X)
{
    try
    {
        if(fromX < 0 || 7 < fromX || fromY < 0 || 7 < fromY || toX < 0 || 7 < toX || toY < 0 || 7 < toY)
            throw std::invalid_argument("Chessboard coordinates out of range.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if('b'<=ptr_X[toY][toX] && ptr_X[toY][toX]<='r')
        return false;
    if('B'<=ptr_X[toY][toX] && ptr_X[toY][toX]<='R')
    {
        userMovesService(fromX,fromY,toX,toY,piece,ptr_X);
        return false;
    }
    if(ptr_X[toY][toX]==' ')
    {
        userMovesService(fromX,fromY,toX,toY,piece,ptr_X);
        return true;
    }
}
        void Move::userUsualKingMovesService(int fromX, int fromY, int directionX, int directionY, globalType::chessboardPointer ptr_X)
{
    int toX = fromX + directionX;
    int toY = fromY + directionY;

    if (0 <= toX && toX <= 7 && 0 <= toY && toY <= 7)
    if(ptr_X[toY][toX] < 'b' || 'r' < ptr_X[toY][toX]) // is anything other than the engine piece on the target square
    if(2 <= abs(userKingLocationX + directionX - engineKingLocationX) || 2 <= abs(userKingLocationY + directionY - engineKingLocationY)) // are the kings too close to each other
    {
        globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
        cOpy[fromY][fromX]=' ';
        cOpy[toY]  [toX]  ='k';
        if(isUserSquareCaptured(userKingLocationX + directionX, userKingLocationY + directionY, cOpy) == false)
            movements[engineMoveNumber].push_back(cOpy);
        else delete[]cOpy;
    }
}
        void Move::userCastleService                                                 (int kingToX, globalType::chessboardPointer ptr_X)
{
    int kingFromX = (3 < kingToX)? (kingToX - 2): (kingToX + 2);
    int rookFromX = (3 < kingToX)? 7: 0;
    int rookToX   = (3 < kingToX)? (kingToX - 1): (kingToX + 1);

    if((userKingMoved == false) && ((rookFromX == 0)? (userLeftRookMoved  == false): true) && ((rookFromX == 7)? (userRightRookMoved == false): true))
    if((ptr_X[0][kingToX]==' ') && (ptr_X[0][rookToX]==' ') && ((abs(rookFromX - kingToX) == 2)? (ptr_X[0][(rookFromX + kingToX)/2]==' '): true))
    {
        globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
        if(isUserSquareCaptured(kingFromX, 0, cOpy) == false && isUserSquareCaptured(kingToX, 0, cOpy) == false && isUserSquareCaptured(rookToX, 0, cOpy) == false)
        {
            cOpy[0][rookFromX]=' ';
            cOpy[0][kingFromX]=' ';
            cOpy[0][kingToX]  ='k';
            cOpy[0][rookToX]  ='r';
            movements[engineMoveNumber].push_back(cOpy);
        }
        else delete[]cOpy;
    }
}
        void Move::userMovesService         (int fromX, int fromY, int toX, int toY, char piece,   globalType::chessboardPointer ptr_X)
{
    try
    {
        if(fromX<0 || 7<fromX || fromY<0 || 7<fromY || toX<0 || 7<toX || toY<0 || 7<toY)
            throw std::invalid_argument("Chessboard coordinates out of range.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
    cOpy[fromY][fromX] = ' ';
    cOpy[toY][toX] = piece;
    makeUserMovesIfAllowed(fromX, fromY, cOpy);
}
            void Move::makeUserMovesIfAllowed                                     (int &x, int &y, globalType::chessboardPointer  cOpy)
{

    try
    {
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(userKingChecked==false && x!=userKingLocationX && y!=userKingLocationY && (userKingLocationX+userKingLocationY!=x+y) && (userKingLocationX-userKingLocationY!=x-y))
        movements[engineMoveNumber].push_back(cOpy);
    else if(isUserSquareCaptured(userKingLocationX, userKingLocationY, cOpy)==false)
        movements[engineMoveNumber].push_back(cOpy);
    else delete[]cOpy;
}
                bool Move::isUserSquareCaptured                       (const int &x, const int &y, globalType::chessboardPointer  cOpy)
{
    try
    {
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    for(int i=1; 0<=y-i; i++)                                       // movement towards 12:00 captured by Q/R/B
    {
        if(cOpy[y-i][x]==' ') continue;
        if(cOpy[y-i][x]=='Q' || cOpy[y-i][x]=='R') return true;
        else break;
    }
    for(int i=1; 0<=y-i && x+i<=7; i++)                             // movement towards 01:30
    {
        if(cOpy[y-i][x+i]==' ') continue;
        if(cOpy[y-i][x+i]=='Q' || cOpy[y-i][x+i]=='B') return true;
        else break;
    }
    for(int i=1; x+i<=7; i++)                                       // movement towards 03:00
    {
        if(cOpy[y][x+i]==' ') continue;
        if(cOpy[y][x+i]=='Q' || cOpy[y][x+i]=='R') return true;
        else break;
    }
    for(int i=1; y+i<=7 && x+i<=7; i++)                             // movement towards 04:30
    {
        if(cOpy[y+i][x+i]==' ') continue;
        if(cOpy[y+i][x+i]=='Q' || cOpy[y+i][x+i]=='B') return true;
        else break;
    }
    for(int i=1; y+i<=7; i++)                                       // movement towards 06:00
    {
        if(cOpy[y+i][x]==' ') continue;
        if(cOpy[y+i][x]=='Q' || cOpy[y+i][x]=='R') return true;
        else break;
    }
    for(int i=1; y+i<=7 && 0<=x-i; i++)                             // movement towards 07:30
    {
        if(cOpy[y+i][x-i]==' ') continue;
        if(cOpy[y+i][x-i]=='Q' || cOpy[y+i][x-i]=='B') return true;
        else break;
    }
    for(int i=1; 0<=x-i; i++)                                       // movement towards 09:00
    {
        if(cOpy[y][x-i]==' ') continue;
        if(cOpy[y][x-i]=='Q' || cOpy[y][x-i]=='R') return true;
        else break;
    }
    for(int i=1; 0<=y-i && 0<=x-i; i++)                             // movement towards 10:30
    {
        if(cOpy[y-i][x-i]==' ') continue;
        if(cOpy[y-i][x-i]=='Q' || cOpy[y-i][x-i]=='B') return true;
        else break;
    }

    if(0<=y-2 && x+1<=7 && cOpy[y-2][x+1]=='N') return true;                       //captured by knight
    if(0<=y-1 && x+2<=7 && cOpy[y-1][x+2]=='N') return true;
    if(y+1<=7 && x+2<=7 && cOpy[y+1][x+2]=='N') return true;
    if(y+2<=7 && x+1<=7 && cOpy[y+2][x+1]=='N') return true;
    if(y+2<=7 && 0<=x-1 && cOpy[y+2][x-1]=='N') return true;
    if(y+1<=7 && 0<=x-2 && cOpy[y+1][x-2]=='N') return true;
    if(0<=y-1 && 0<=x-2 && cOpy[y-1][x-2]=='N') return true;
    if(0<=y-2 && 0<=x-1 && cOpy[y-2][x-1]=='N') return true;

    if(y<=5)                                                                    //captured by pawn
    {
        if(1<=x && cOpy[y+1][x-1]=='P') return true;
        if(x<=6 && cOpy[y+1][x+1]=='P') return true;
    }
    return false;
}
globalType::chessboardPointer Move::copyChessboard                                          (const globalType::chessboardPointer oryginal)
{
    try
    {
        if (oryginal == nullptr)
            throw std::invalid_argument("Attempting to copy the nullptr address of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    globalType::chessboardPointer cOpy;
    try
    {
        cOpy = new char[8][8];
    }
    catch(const std::bad_alloc &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            cOpy[i][j] = oryginal[i][j];
    return cOpy;
}
void Move::writeChessboard                                                                  (const globalType::chessboardPointer ptr_X)
{
    try
    {
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    std::cout<<std::string(23,'\333')<<std::endl;
    std::cout<<std::string(3 ,'\333')<<" 0 1 2 3 4 5 6 7 "<<std::string(3 ,'\333')<<std::endl;
    std::cout<<std::string(23,'\333')<<std::endl;
    //for(int i=0; i<8; i++)
    for(int i=7; i>=0; i--)
    {
        std::cout<<"\333"<<i<<"\333";
        for(int j=7; j>=0; j--)
            std::cout<<' '<<ptr_X[i][j];
        std::cout<<' ';
        std::cout<<"\333"<<i<<"\333"<<std::endl;
    }
    std::cout<<std::string(23,'\333')<<std::endl;
    std::cout<<std::string(3 ,'\333')<<" 0 1 2 3 4 5 6 7 "<<std::string(3 ,'\333')<<std::endl;
    std::cout<<std::string(23,'\333')<<std::endl;
}
