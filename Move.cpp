#include "Move.h"

Move::Move(bool k)noexcept: color{k}{
//==============================================================================================================
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
Move::~Move()noexcept{
//==============================================================================================================
    resetMovements();
}
    void Move::resetMovements()noexcept{
//==============================================================================================================
    for(auto& oneDimensionalVector: movements)
        for(auto& singeMove: oneDimensionalVector)
            delete[]singeMove;
    movements.clear();
}
double Move::findNextMove                          (globalType::chessboardPointer &ptr_X){//7
//==============================================================================================================
    try{
        if(ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        verifyKingsLocation(ptr_X);
        isRooksAndKingsMoved(ptr_X);
        isEngineKingChecked(ptr_X);
        makeEngineMoves(ptr_X);
        setSearchingDepth();
        return findBestMove(ptr_X);
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Move::verifyKingsLocation                 (globalType::chessboardPointer  ptr_X){//0+
//==============================================================================================================
    try{
        if(ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
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
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    void Move::isRooksAndKingsMoved                (globalType::chessboardPointer  ptr_X){//0+
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        if(ptr_X[7][0] != 'R') engineLeftRookMoved  = true;
        if(ptr_X[7][7] != 'R') engineRightRookMoved = true;
        if(ptr_X[0][0] != 'r') userLeftRookMoved    = true;
        if(ptr_X[0][7] != 'r') userRightRookMoved   = true;
        if(color? ptr_X[7][3] != 'K': ptr_X[7][4] != 'K') engineKingMoved = true;
        if(color? ptr_X[0][3] != 'k': ptr_X[0][4] != 'k') userKingMoved   = true;
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    void Move::isEngineKingChecked                 (globalType::chessboardPointer  ptr_X){//1
//==============================================================================================================
    try{
        engineKingChecked = isEngineSquareCaptured(engineKingLocationX, engineKingLocationY, ptr_X);
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Move::setSearchingDepth()noexcept{
//==============================================================================================================
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
    double Move::findBestMove                      (globalType::chessboardPointer &ptr_X){//r
//==============================================================================================================
    try{
        if(movementGeneration <  lastMovementGeneration) return beginningSearchingTreeService(ptr_X);
        if(movementGeneration == lastMovementGeneration) return endingSearchingTreeService();
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        double Move::beginningSearchingTreeService (globalType::chessboardPointer &ptr_X){//r
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of Move pointer.");
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
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            Move::Move(Move* pointer, double &materialStatus, globalType::chessboardPointer ptr_X, int currentIterationNr, int lastIterationNr): movementGeneration {currentIterationNr},lastMovementGeneration{lastIterationNr}{//r
//==============================================================================================================
    try{
        rewriteKingsAndRooksMovesData(pointer);
        materialStatus = findNextMove(ptr_X);
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
                void Move::rewriteKingsAndRooksMovesData(Move* ptr){//0+
//==============================================================================================================
    try{
        if (ptr == nullptr)
            throw std::invalid_argument("Nullptr of Move pointer.");
        engineKingMoved      = ptr -> engineKingMoved;
        engineLeftRookMoved  = ptr -> engineLeftRookMoved;
        engineRightRookMoved = ptr -> engineRightRookMoved;
        userKingMoved        = ptr -> userKingMoved;
        userLeftRookMoved    = ptr -> userLeftRookMoved;
        userRightRookMoved   = ptr -> userRightRookMoved;
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
            void Move::checkmateAndStalemateSearching                (globalType::chessboardPointer &ptr_X, int &index){//2
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of ptr_X pointer.");
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
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
                void Move::insufficientMaterialServiceAfterUserMove  (globalType::chessboardPointer  ptr_X){//1
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of ptr_X pointer.");
        if(isGameFinishedByInsufficientMaterial(ptr_X))
        {
            gameOver = true;
            gameOverStalemateByUser = true;
        }
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
                    bool Move::isGameFinishedByInsufficientMaterial  (globalType::chessboardPointer  ptr_X){//0+
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of ptr_X pointer.");
        int nrOfKnights     {};
        int nrOfBishops     {};
        int nrOfOtherPieces {};

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
        if(2 < nrOfOtherPieces) return false;
        else if(nrOfKnights == 1 && nrOfBishops == 0) return true;
        else if(nrOfKnights == 0 && nrOfBishops == 1) return true;
        else if(nrOfKnights == 0 && nrOfBishops == 0) return true;
        else return false;
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(const std::runtime_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
                void Move::insufficientMaterialServiceAfterEngineMove(globalType::chessboardPointer  ptr_X){//1
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of ptr_X pointer.");
        if(isGameFinishedByInsufficientMaterial(ptr_X))
        {
            gameOver = true;
            gameOverStalemateByEngine = true;
        }
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
                void Move::isGameFinishedByUser                      (globalType::chessboardPointer &ptr_X){//1
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of ptr_X pointer.");
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
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
                void Move::isGameFinishedByEngine                    (globalType::chessboardPointer &ptr_X, int &index){//1
//==============================================================================================================
    try{
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
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        double Move::endingSearchingTreeService(){//1
//==============================================================================================================
    try{
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
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            double Move::countMiddlegameMaterialStatus         (const globalType::chessboardPointer  ptr_X){//0+
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        double materialStatus  = 0.0;
        double pawnValue       = 1.0;
        double sideKnightValue = 2.9;
        double knightValue     = 3.0;
        double sideBishopValue = 2.9;
        double bishopValue     = 3.0;
        double rookValue       = 5.0;
        double queenValue      = 9.0;

        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
            {
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
        return materialStatus;
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(const std::runtime_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
            double Move::countEndgameMaterialStatus            (const globalType::chessboardPointer  ptr_X){//0+
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        double materialStatus = 0.0;
        double pawnValue      = 1.0;
        double knightValue    = 3.0;
        double bishopValue    = 3.0;
        double rookValue      = 5.0;
        double queenValue     = 9.0;

        globalType::userKingX = userKingLocationX;
        globalType::userKingY = userKingLocationY;

        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
            {
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
        return materialStatus;
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(const std::runtime_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
                double Move::goOppositeDirectionOfUserKing   (double x, double y)noexcept{
//==============================================================================================================
    if(globalType::userKingSideLocation == globalType::engineRightSide)
        return 7.0-x;
    if(globalType::userKingSideLocation == globalType::engineDownSide)
        return 7.0-y;
    if(globalType::userKingSideLocation == globalType::engineLeftSide)
        return x;
    if(globalType::userKingSideLocation == globalType::engineUpSide)
        return y;
}
                double Move::goToSideOfUserKing              (double x, double y)noexcept{
//==============================================================================================================
    if(globalType::userKingSideLocation == globalType::engineRightSide)
        return x;
    if(globalType::userKingSideLocation == globalType::engineDownSide)
        return y;
    if(globalType::userKingSideLocation == globalType::engineLeftSide)
        return 7.0-x;
    if(globalType::userKingSideLocation == globalType::engineUpSide)
        return 7.0-y;
}
                double Move::goToCornerOfUserKing            (double x, double y)noexcept{
//==============================================================================================================
    if(globalType::userKingCornerLocation == globalType::engineUpRightCorner)
        return (x + 7.0-y)/2.0;
    if(globalType::userKingCornerLocation == globalType::engineDownRightCorner)
        return (x + y)/2.0;
    if(globalType::userKingCornerLocation == globalType::engineDownLeftCorner)
        return (7.0-x + y)/2.0;
    if(globalType::userKingCornerLocation == globalType::engineUpLeftCorner)
        return (7.0-x + 7.0-y)/2.0;
}
                double Move::separateUserKingFromRestOfBoard (double x, double y)noexcept{
//==============================================================================================================
    if(globalType::userKingSideLocation == globalType::engineRightSide)
        return (x == globalType::userKingX-1.0)? 1.0: 0.0;
    if(globalType::userKingSideLocation == globalType::engineDownSide)
        return (y == globalType::userKingY-1.0)? 1.0: 0.0;
    if(globalType::userKingSideLocation == globalType::engineLeftSide)
        return (x == globalType::userKingX+1.0)? 1.0: 0.0;
    if(globalType::userKingSideLocation == globalType::engineUpSide)
        return (y == globalType::userKingY+1.0)? 1.0: 0.0;
}
                double Move::runSidewaysFromKing             (double x, double y)noexcept{
//==============================================================================================================
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
                double Move::followUserKingToSide            (double x, double y)noexcept{
//==============================================================================================================
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
                double Move::followUserKingToCorner          (double x, double y)noexcept{
//==============================================================================================================
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
                double Move::makeNothing                     (double x, double y)noexcept{
//==============================================================================================================
    return 0.0;
}
void Move::makeEngineMoves                                                            (globalType::chessboardPointer ptr_X){//6
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        engineMoveNumber = -1 ;

        for(int i=0; i<8; i++)
            for (int j: {3,4,2,5,1,6,0,7})
            {
                switch(ptr_X[i][j])
                {
                    case ' ': break;
                    case 'Q':                                                       //queen issue
                    {
                        for(int y=i-1; 0<=y; y--)                                       //movement towards 12:00
                        {
                            if('B'<=ptr_X[y][j] && ptr_X[y][j]<='R')                                         break;
                            if('b'<=ptr_X[y][j] && ptr_X[y][j]<='r'){engineMovesService(j,i,j,y,'Q',ptr_X);  break;}
                            if(ptr_X[y][j]==' ')                     engineMovesService(j,i,j,y,'Q',ptr_X);
                        }
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //movement towards 01:30
                        {
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R')                                         break;
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r'){engineMovesService(j,i,x,y,'Q',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                     engineMovesService(j,i,x,y,'Q',ptr_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //movement towards 03:00
                        {
                            if('B'<=ptr_X[i][x] && ptr_X[i][x]<='R')                                         break;
                            if('b'<=ptr_X[i][x] && ptr_X[i][x]<='r'){engineMovesService(j,i,x,i,'Q',ptr_X);  break;}
                            if(ptr_X[i][x]==' ')                     engineMovesService(j,i,x,i,'Q',ptr_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //movement towards 04:30
                        {
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R')                                         break;
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r'){engineMovesService(j,i,x,y,'Q',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                     engineMovesService(j,i,x,y,'Q',ptr_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //movement towards 06:00
                        {
                            if('B'<=ptr_X[y][j] && ptr_X[y][j]<='R')                                         break;
                            if('b'<=ptr_X[y][j] && ptr_X[y][j]<='r'){engineMovesService(j,i,j,y,'Q',ptr_X);  break;}
                            if(ptr_X[y][j]==' ')                     engineMovesService(j,i,j,y,'Q',ptr_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //movement towards 07:30
                        {
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R')                                         break;
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r'){engineMovesService(j,i,x,y,'Q',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                     engineMovesService(j,i,x,y,'Q',ptr_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //movement towards 09:00
                        {
                            if('B'<=ptr_X[i][x] && ptr_X[i][x]<='R')                                         break;
                            if('b'<=ptr_X[i][x] && ptr_X[i][x]<='r'){engineMovesService(j,i,x,i,'Q',ptr_X);  break;}
                            if(ptr_X[i][x]==' ')                     engineMovesService(j,i,x,i,'Q',ptr_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //movement towards 10:30
                        {
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R')                                         break;
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r'){engineMovesService(j,i,x,y,'Q',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                     engineMovesService(j,i,x,y,'Q',ptr_X);
                        }
                        break;
                    }
                    case 'N':                                                       //knight issue
                    {
                        if(0<=i-2 && j+1<=7 && (ptr_X[i-2][j+1]<'B' || 'R'<ptr_X[i-2][j+1])) engineMovesService(j,i,j+1,i-2,'N',ptr_X);
                        if(0<=i-1 && j+2<=7 && (ptr_X[i-1][j+2]<'B' || 'R'<ptr_X[i-1][j+2])) engineMovesService(j,i,j+2,i-1,'N',ptr_X);
                        if(i+1<=7 && j+2<=7 && (ptr_X[i+1][j+2]<'B' || 'R'<ptr_X[i+1][j+2])) engineMovesService(j,i,j+2,i+1,'N',ptr_X);
                        if(i+2<=7 && j+1<=7 && (ptr_X[i+2][j+1]<'B' || 'R'<ptr_X[i+2][j+1])) engineMovesService(j,i,j+1,i+2,'N',ptr_X);
                        if(i+2<=7 && 0<=j-1 && (ptr_X[i+2][j-1]<'B' || 'R'<ptr_X[i+2][j-1])) engineMovesService(j,i,j-1,i+2,'N',ptr_X);
                        if(i+1<=7 && 0<=j-2 && (ptr_X[i+1][j-2]<'B' || 'R'<ptr_X[i+1][j-2])) engineMovesService(j,i,j-2,i+1,'N',ptr_X);
                        if(0<=i-1 && 0<=j-2 && (ptr_X[i-1][j-2]<'B' || 'R'<ptr_X[i-1][j-2])) engineMovesService(j,i,j-2,i-1,'N',ptr_X);
                        if(0<=i-2 && 0<=j-1 && (ptr_X[i-2][j-1]<'B' || 'R'<ptr_X[i-2][j-1])) engineMovesService(j,i,j-1,i-2,'N',ptr_X);
                        break;
                    }
                    case 'B':                                                       //bishop issue
                    {
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //movement towards 1:30
                        {
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R')                                          break;
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r') {engineMovesService(j,i,x,y,'B',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      engineMovesService(j,i,x,y,'B',ptr_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //movement towards 4:30
                        {
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R')                                          break;
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r') {engineMovesService(j,i,x,y,'B',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      engineMovesService(j,i,x,y,'B',ptr_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //movement towards 7:30
                        {
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R')                                          break;
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r') {engineMovesService(j,i,x,y,'B',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      engineMovesService(j,i,x,y,'B',ptr_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //movement towards 10:30
                        {
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R')                                          break;
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r') {engineMovesService(j,i,x,y,'B',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      engineMovesService(j,i,x,y,'B',ptr_X);
                        }
                        break;
                    }
                    case 'R':                                                       //rook issue
                    {
                        for(int y=i-1; 0<=y; y--)                                       //movement towards 12:00
                        {
                            if('B'<=ptr_X[y][j] && ptr_X[y][j]<='R')                                          break;
                            if('b'<=ptr_X[y][j] && ptr_X[y][j]<='r') {engineMovesService(j,i,j,y,'R',ptr_X);  break;}
                            if(ptr_X[y][j]==' ')                      engineMovesService(j,i,j,y,'R',ptr_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //movement towards 03:00
                        {
                            if('B'<=ptr_X[i][x] && ptr_X[i][x]<='R')                                          break;
                            if('b'<=ptr_X[i][x] && ptr_X[i][x]<='r') {engineMovesService(j,i,x,i,'R',ptr_X);  break;}
                            if(ptr_X[i][x]==' ')                      engineMovesService(j,i,x,i,'R',ptr_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //movement towards 06:00
                        {
                            if('B'<=ptr_X[y][j] && ptr_X[y][j]<='R')                                          break;
                            if('b'<=ptr_X[y][j] && ptr_X[y][j]<='r') {engineMovesService(j,i,j,y,'R',ptr_X);  break;}
                            if(ptr_X[y][j]==' ')                      engineMovesService(j,i,j,y,'R',ptr_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //movement towards 09:00
                        {
                            if('B'<=ptr_X[i][x] && ptr_X[i][x]<='R')                                          break;
                            if('b'<=ptr_X[i][x] && ptr_X[i][x]<='r') {engineMovesService(j,i,x,i,'R',ptr_X);  break;}
                            if(ptr_X[i][x]==' ')                      engineMovesService(j,i,x,i,'R',ptr_X);
                        }
                        break;
                    }
                    case 'P':                                                       // pawn issue
                    {
                        if(1<i && i<7)                                                  // pawn with no chance at promotion
                        {
                            if(ptr_X[i-1][j]==' ')                                   engineMovesService(j,i,j,i-1,'P',ptr_X);    // move forward by 1
                            if(i==6 && ptr_X[5][j]==' ' && ptr_X[4][j]==' ')         engineMovesService(j,i,j,4,'P',ptr_X);      // move forward by 2
                            if(j!=0 && 'b'<=ptr_X[i-1][j-1] && ptr_X[i-1][j-1]<='r') engineMovesService(j,i,j-1,i-1,'P',ptr_X);  // left capture
                            if(j!=7 && 'b'<=ptr_X[i-1][j+1] && ptr_X[i-1][j+1]<='r') engineMovesService(j,i,j+1,i-1,'P',ptr_X);  // right capture
                        }
                        if(i==1)                                                       // promotion
                        {
                            if(ptr_X[0][j]==' ')                                // just promotion with the creation of a
                            {
                                engineMovesService(j,1,j,0,'N',ptr_X);//knight
                                engineMovesService(j,1,j,0,'B',ptr_X);//bishop
                                engineMovesService(j,1,j,0,'R',ptr_X);//rook
                                engineMovesService(j,1,j,0,'Q',ptr_X);//queen
                            }
                            if(j!=0 && 'b'<=ptr_X[0][j-1] && ptr_X[0][j-1]<='r')// promotion by left capture with the creation of a
                            {
                                engineMovesService(j,1,j-1,0,'N',ptr_X);//knight
                                engineMovesService(j,1,j-1,0,'B',ptr_X);//bishop
                                engineMovesService(j,1,j-1,0,'R',ptr_X);//rook
                                engineMovesService(j,1,j-1,0,'Q',ptr_X);//queen
                            }
                            if(j!=7 && 'b'<=ptr_X[0][j+1] && ptr_X[0][j+1]<='r')// promotion by right capture with the creation of a
                            {
                                engineMovesService(j,1,j+1,0,'N',ptr_X);//knight
                                engineMovesService(j,1,j+1,0,'B',ptr_X);//bishop
                                engineMovesService(j,1,j+1,0,'R',ptr_X);//rook
                                engineMovesService(j,1,j+1,0,'Q',ptr_X);//queen
                            }
                        }
                        break;
                    }
                    case 'K':                                                       //king issue
                    {
                        if(0<=i-1 && (ptr_X[i-1][j]<'B' || 'R'<ptr_X[i-1][j]))                    //movement towards 12:00
                        if(2<=abs(engineKingLocationX-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            engineKingLocationY--;
                            engineMovesService(j,i,j,i-1,'K',ptr_X);
                            engineKingLocationY++;
                        }
                        if(0<=i-1 && j+1<=7 && (ptr_X[i-1][j+1]<'B' || 'R'<ptr_X[i-1][j+1]))      //movement towards 1:30
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            engineKingLocationX++;
                            engineKingLocationY--;
                            engineMovesService(j,i,j+1,i-1,'K',ptr_X);
                            engineKingLocationX--;
                            engineKingLocationY++;
                        }
                        if(j+1<=7 && (ptr_X[i][j+1]<'B' || 'R'<ptr_X[i][j+1]))                    //movement towards 3:00
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY-userKingLocationY))
                        {
                            engineKingLocationX++;
                            engineMovesService(j,i,j+1,i,'K',ptr_X);
                            engineKingLocationX--;
                        }
                        if(i+1<=7 && j+1<=7 && (ptr_X[i+1][j+1]<'B' || 'R'<ptr_X[i+1][j+1]))      //movement towards 4:30
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            engineKingLocationX++;
                            engineKingLocationY++;
                            engineMovesService(j,i,j+1,i+1,'K',ptr_X);
                            engineKingLocationX--;
                            engineKingLocationY--;
                        }
                        if(i+1<=7 && (ptr_X[i+1][j]<'B' || 'R'<ptr_X[i+1][j]))                    //movement towards 6:00
                        if(2<=abs(engineKingLocationX-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            engineKingLocationY++;
                            engineMovesService(j,i,j,i+1,'K',ptr_X);
                            engineKingLocationY--;
                        }
                        if(i+1<=7 && 0<=j-1 && (ptr_X[i+1][j-1]<'B' || 'R'<ptr_X[i+1][j-1]))      //movement towards 7:30
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            engineKingLocationX--;
                            engineKingLocationY++;
                            engineMovesService(j,i,j-1,i+1,'K',ptr_X);
                            engineKingLocationX++;
                            engineKingLocationY--;
                        }
                        if(0<=j-1 && (ptr_X[i][j-1]<'B' || 'R'<ptr_X[i][j-1]))                    //movement towards 9:00
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY-userKingLocationY))
                        {
                            engineKingLocationX--;
                            engineMovesService(j,i,j-1,i,'K',ptr_X);
                            engineKingLocationX++;
                        }
                        if(0<=i-1 && 0<=j-1 && (ptr_X[i-1][j-1]<'B' || 'R'<ptr_X[i-1][j-1]))      //movement towards 10:30
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            engineKingLocationX--;
                            engineKingLocationY--;
                            engineMovesService(j,i,j-1,i-1,'K',ptr_X);
                            engineKingLocationX++;
                            engineKingLocationY++;
                        }
                        if(!color)                                                                      //castling when the user play black
                        {
                            if(engineKingMoved==false && engineLeftRookMoved==false && ptr_X[7][1]==' ' && ptr_X[7][2]==' ' && ptr_X[7][3]==' ')       //long castle
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                                if(isEngineSquareCaptured(2, 7, cOpy)==false && isEngineSquareCaptured(3, 7, cOpy)==false && isEngineSquareCaptured(4, 7, cOpy)==false)
                                {
                                    cOpy[7][0]=' ';
                                    cOpy[7][4]=' ';
                                    cOpy[7][2]='K';
                                    cOpy[7][3]='R';
                                    engineKingLocationX-=2;
                                    makeEngineMovesIfAllowed(j, i, cOpy);
                                    engineKingLocationX+=2;
                                }
                                else delete[]cOpy;
                            }
                            if(engineKingMoved==false && engineRightRookMoved==false && ptr_X[7][5]==' ' && ptr_X[7][6]==' ')                          //short caste
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                                if(isEngineSquareCaptured(4, 7, cOpy)==false && isEngineSquareCaptured(5, 7, cOpy)==false && isEngineSquareCaptured(6, 7, cOpy)==false)
                                {
                                    cOpy[7][7]=' ';
                                    cOpy[7][4]=' ';
                                    cOpy[7][6]='K';
                                    cOpy[7][5]='R';
                                    engineKingLocationX+=2;
                                    makeEngineMovesIfAllowed(j, i, cOpy);
                                    engineKingLocationX-=2;
                                }
                                else delete[]cOpy;
                            }
                        }
                        if(color)                                                                       //castling when the user play white
                        {
                            if(engineKingMoved==false && engineRightRookMoved==false && ptr_X[7][4]==' ' && ptr_X[7][5]==' ' && ptr_X[7][6]==' ')       //long castle
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                                if(isEngineSquareCaptured(3, 7, cOpy)==false && isEngineSquareCaptured(4, 7, cOpy)==false && isEngineSquareCaptured(5, 7, cOpy)==false)
                                {
                                    cOpy[7][7]=' ';
                                    cOpy[7][3]=' ';
                                    cOpy[7][5]='K';
                                    cOpy[7][4]='R';
                                    engineKingLocationX+=2;
                                    makeEngineMovesIfAllowed(j, i, cOpy);
                                    engineKingLocationX-=2;
                                }
                                else delete[]cOpy;
                            }
                            if(engineKingMoved==false && engineLeftRookMoved==false && ptr_X[7][1]==' ' && ptr_X[7][2]==' ')                          //short caste
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                                if(isEngineSquareCaptured(1, 7, cOpy)==false && isEngineSquareCaptured(2, 7, cOpy)==false && isEngineSquareCaptured(3, 7, cOpy)==false)
                                {
                                    cOpy[7][0]=' ';
                                    cOpy[7][3]=' ';
                                    cOpy[7][1]='K';
                                    cOpy[7][2]='R';
                                    engineKingLocationX-=2;
                                    makeEngineMovesIfAllowed(j, i, cOpy);
                                    engineKingLocationX+=2;
                                }
                                else delete[]cOpy;
                            }
                        }
                        break;
                    }
                default: break;
            }
        }
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Move::engineMovesService (int fromX, int fromY, int toX, int toY, char piece, globalType::chessboardPointer ptr_X){//5
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        if(fromX<0 || 7<fromX || fromY<0 || 7<fromY || toX<0 || 7<toX || toY<0 || 7<toY)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
        cOpy[fromY][fromX] = ' ';
        cOpy[toY][toX] = piece;
        makeEngineMovesIfAllowed(fromX, fromY, cOpy);
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Move::makeEngineMovesIfAllowed                           (int &x, int &y, globalType::chessboardPointer cOpy){//4
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
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
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            bool Move::isEngineSquareCaptured              (const int &x,const int &y, globalType::chessboardPointer cOpy){//0+
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
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

        if(0<=y-2 && x+1<=7 && cOpy[y-2][x+1]=='n') return true;                       //BITE PRZEZ SKOCZKA
        if(0<=y-1 && x+2<=7 && cOpy[y-1][x+2]=='n') return true;
        if(y+1<=7 && x+2<=7 && cOpy[y+1][x+2]=='n') return true;
        if(y+2<=7 && x+1<=7 && cOpy[y+2][x+1]=='n') return true;
        if(y+2<=7 && 0<=x-1 && cOpy[y+2][x-1]=='n') return true;
        if(y+1<=7 && 0<=x-2 && cOpy[y+1][x-2]=='n') return true;
        if(0<=y-1 && 0<=x-2 && cOpy[y-1][x-2]=='n') return true;
        if(0<=y-2 && 0<=x-1 && cOpy[y-2][x-1]=='n') return true;

        if(2<=y)                                                                    //BITE PRZEZ PIONKA
        {
            if(1<=x && cOpy[y-1][x-1]=='p') return true;
            if(x<=6 && cOpy[y-1][x+1]=='p') return true;
        }
        return false;
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
void Move::makeUserMoves                                                              (globalType::chessboardPointer ptr_X){//3
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        userKingChecked = isUserSquareCaptured(userKingLocationX, userKingLocationY, ptr_X);
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
            {
                switch(ptr_X[i][j])
                {
                    case ' ': break;
                    case 'n':                                                       //knight issue
                    {
                        if(0<=i-2 && j+1<=7 && (ptr_X[i-2][j+1]<'b' || 'r'<ptr_X[i-2][j+1]))  userMovesService(j,i,j+1,i-2,'n',ptr_X);
                        if(0<=i-1 && j+2<=7 && (ptr_X[i-1][j+2]<'b' || 'r'<ptr_X[i-1][j+2]))  userMovesService(j,i,j+2,i-1,'n',ptr_X);
                        if(i+1<=7 && j+2<=7 && (ptr_X[i+1][j+2]<'b' || 'r'<ptr_X[i+1][j+2]))  userMovesService(j,i,j+2,i+1,'n',ptr_X);
                        if(i+2<=7 && j+1<=7 && (ptr_X[i+2][j+1]<'b' || 'r'<ptr_X[i+2][j+1]))  userMovesService(j,i,j+1,i+2,'n',ptr_X);
                        if(i+2<=7 && 0<=j-1 && (ptr_X[i+2][j-1]<'b' || 'r'<ptr_X[i+2][j-1]))  userMovesService(j,i,j-1,i+2,'n',ptr_X);
                        if(i+1<=7 && 0<=j-2 && (ptr_X[i+1][j-2]<'b' || 'r'<ptr_X[i+1][j-2]))  userMovesService(j,i,j-2,i+1,'n',ptr_X);
                        if(0<=i-1 && 0<=j-2 && (ptr_X[i-1][j-2]<'b' || 'r'<ptr_X[i-1][j-2]))  userMovesService(j,i,j-2,i-1,'n',ptr_X);
                        if(0<=i-2 && 0<=j-1 && (ptr_X[i-2][j-1]<'b' || 'r'<ptr_X[i-2][j-1]))  userMovesService(j,i,j-1,i-2,'n',ptr_X);
                        break;
                    }
                    case 'b':                                                       //bishop issue
                    {
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //movement towards 1:30
                        {
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r')                                        break;
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R') {userMovesService(j,i,x,y,'b',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      userMovesService(j,i,x,y,'b',ptr_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //movement towards 4:30
                        {
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r')                                        break;
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R') {userMovesService(j,i,x,y,'b',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      userMovesService(j,i,x,y,'b',ptr_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //movement towards 7:30
                        {
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r')                                        break;
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R') {userMovesService(j,i,x,y,'b',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      userMovesService(j,i,x,y,'b',ptr_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //movement towards 10:30
                        {
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r')                                        break;
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R') {userMovesService(j,i,x,y,'b',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      userMovesService(j,i,x,y,'b',ptr_X);
                        }
                        break;
                    }
                    case 'r':                                                       //rook issue
                    {
                        for(int y=i-1; 0<=y; y--)                                       //movement towards 12:00
                        {
                            if('b'<=ptr_X[y][j] && ptr_X[y][j]<='r')                                        break;
                            if('B'<=ptr_X[y][j] && ptr_X[y][j]<='R') {userMovesService(j,i,j,y,'r',ptr_X);  break;}
                            if(ptr_X[y][j]==' ')                      userMovesService(j,i,j,y,'r',ptr_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //movement towards 03:00
                        {
                            if('b'<=ptr_X[i][x] && ptr_X[i][x]<='r')                                        break;
                            if('B'<=ptr_X[i][x] && ptr_X[i][x]<='R') {userMovesService(j,i,x,i,'r',ptr_X);  break;}
                            if(ptr_X[i][x]==' ')                      userMovesService(j,i,x,i,'r',ptr_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //movement towards 06:00
                        {
                            if('b'<=ptr_X[y][j] && ptr_X[y][j]<='r')                                        break;
                            if('B'<=ptr_X[y][j] && ptr_X[y][j]<='R') {userMovesService(j,i,j,y,'r',ptr_X);  break;}
                            if(ptr_X[y][j]==' ')                      userMovesService(j,i,j,y,'r',ptr_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //movement towards 09:00
                        {
                            if('b'<=ptr_X[i][x] && ptr_X[i][x]<='r')                                        break;
                            if('B'<=ptr_X[i][x] && ptr_X[i][x]<='R') {userMovesService(j,i,x,i,'r',ptr_X);  break;}
                            if(ptr_X[i][x]==' ')                      userMovesService(j,i,x,i,'r',ptr_X);
                        }
                        break;
                    }
                    case 'q':                                                       //queen issue
                    {
                        for(int y=i-1; 0<=y; y--)                                       //movement towards 12:00
                        {
                            if('b'<=ptr_X[y][j] && ptr_X[y][j]<='r')                                        break;
                            if('B'<=ptr_X[y][j] && ptr_X[y][j]<='R') {userMovesService(j,i,j,y,'q',ptr_X);  break;}
                            if(ptr_X[y][j]==' ')                      userMovesService(j,i,j,y,'q',ptr_X);
                        }
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //movement towards 01:30
                        {
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r')                                        break;
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R') {userMovesService(j,i,x,y,'q',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      userMovesService(j,i,x,y,'q',ptr_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //movement towards 03:00
                        {
                            if('b'<=ptr_X[i][x] && ptr_X[i][x]<='r')                                        break;
                            if('B'<=ptr_X[i][x] && ptr_X[i][x]<='R') {userMovesService(j,i,x,i,'q',ptr_X);  break;}
                            if(ptr_X[i][x]==' ')                      userMovesService(j,i,x,i,'q',ptr_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //movement towards 04:30
                        {
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r')                                        break;
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R') {userMovesService(j,i,x,y,'q',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      userMovesService(j,i,x,y,'q',ptr_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //movement towards 06:00
                        {
                            if('b'<=ptr_X[y][j] && ptr_X[y][j]<='r')                                        break;
                            if('B'<=ptr_X[y][j] && ptr_X[y][j]<='R') {userMovesService(j,i,j,y,'q',ptr_X);  break;}
                            if(ptr_X[y][j]==' ')                      userMovesService(j,i,j,y,'q',ptr_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //movement towards 07:30
                        {
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r')                                        break;
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R') {userMovesService(j,i,x,y,'q',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      userMovesService(j,i,x,y,'q',ptr_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //movement towards 09:00
                        {
                            if('b'<=ptr_X[i][x] && ptr_X[i][x]<='r')                                        break;
                            if('B'<=ptr_X[i][x] && ptr_X[i][x]<='R') {userMovesService(j,i,x,i,'q',ptr_X);  break;}
                            if(ptr_X[i][x]==' ')                      userMovesService(j,i,x,i,'q',ptr_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //movement towards 10:30
                        {
                            if('b'<=ptr_X[y][x] && ptr_X[y][x]<='r')                                        break;
                            if('B'<=ptr_X[y][x] && ptr_X[y][x]<='R') {userMovesService(j,i,x,y,'q',ptr_X);  break;}
                            if(ptr_X[y][x]==' ')                      userMovesService(j,i,x,y,'q',ptr_X);
                        }
                        break;
                    }
                    case 'p':                                                       //pawn issue
                    {
                        if(0<i && i<6)                                                  //pawn with no chance at promotion
                        {
                            if(ptr_X[i+1][j]==' ')                                    userMovesService(j,i,j,i+1,'p',ptr_X);   //move forward by 1
                            if(i==1 && ptr_X[2][j]==' ' && ptr_X[3][j]==' ')          userMovesService(j,1,j,3,'p',ptr_X);     //move forward by 2
                            if(j!=0 && 'B'<=ptr_X[i+1][j-1] && ptr_X[i+1][j-1]<='R')  userMovesService(j,i,j-1,i+1,'p',ptr_X); //left capture
                            if(j!=7 && 'B'<=ptr_X[i+1][j+1] && ptr_X[i+1][j+1]<='R')  userMovesService(j,i,j+1,i+1,'p',ptr_X); //right capture

                        }
                        if(i==6)                                                       //promotion
                        {
                            if(ptr_X[7][j]==' ')                              //just promotion with the creation of a
                            {
                                userMovesService(j,6,j,7,'n',ptr_X); //knight
                                userMovesService(j,6,j,7,'b',ptr_X); //bishop
                                userMovesService(j,6,j,7,'r',ptr_X); //rook
                                userMovesService(j,6,j,7,'q',ptr_X); //queen
                            }
                            if(j!=0 && 'B'<=ptr_X[7][j-1] && ptr_X[7][j-1]<='R')//promotion by left capture with the creation of a
                            {
                                userMovesService(j,6,j-1,7,'n',ptr_X); //knight
                                userMovesService(j,6,j-1,7,'b',ptr_X); //bishop
                                userMovesService(j,6,j-1,7,'r',ptr_X); //rook
                                userMovesService(j,6,j-1,7,'q',ptr_X); //queen
                            }
                            if(j!=7 && 'B'<=ptr_X[7][j+1] && ptr_X[7][j+1]<='R')//promotion by right capture with the creation of a
                            {
                                userMovesService(j,6,j+1,7,'n',ptr_X); //knight
                                userMovesService(j,6,j+1,7,'b',ptr_X); //bishop
                                userMovesService(j,6,j+1,7,'r',ptr_X); //rook
                                userMovesService(j,6,j+1,7,'q',ptr_X); //queen
                            }
                        }
                        break;
                    }
                    case 'k':                                                       //king issue
                    {
                        if(0<=i-1 && (ptr_X[i-1][j]<'b' || 'r'<ptr_X[i-1][j]))                    //movement towards 12:00
                        if(2<=abs(engineKingLocationX-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                            cOpy[i][j]=' ';
                            cOpy[i-1][j]='k';
                            if(isUserSquareCaptured(userKingLocationX, userKingLocationY-1, cOpy)==false)
                            {
                                userKingLocationY--;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationY++;
                            }
                            else delete[]cOpy;
                        }
                        if(0<=i-1 && j+1<=7 && (ptr_X[i-1][j+1]<'b' || 'r'<ptr_X[i-1][j+1]))      //movement towards 01:30
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                            cOpy[i][j]=' ';
                            cOpy[i-1][j+1]='k';
                            if(isUserSquareCaptured(userKingLocationX+1, userKingLocationY-1, cOpy)==false)
                            {
                                userKingLocationX++;
                                userKingLocationY--;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX--;
                                userKingLocationY++;
                            }
                            else delete[]cOpy;
                        }
                        if(j+1<=7 && (ptr_X[i][j+1]<'b' || 'r'<ptr_X[i][j+1]))                    //movement towards 03:00
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                            cOpy[i][j]=' ';
                            cOpy[i][j+1]='k';
                            if(isUserSquareCaptured(userKingLocationX+1, userKingLocationY, cOpy)==false)
                            {
                                userKingLocationX++;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX--;
                            }
                            else delete[]cOpy;
                        }
                        if(i+1<=7 && j+1<=7 && (ptr_X[i+1][j+1]<'b' || 'r'<ptr_X[i+1][j+1]))      //movement towards 04:30
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                            cOpy[i][j]=' ';
                            cOpy[i+1][j+1]='k';
                            if(isUserSquareCaptured(userKingLocationX+1, userKingLocationY+1, cOpy)==false)
                            {
                                userKingLocationX++;
                                userKingLocationY++;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX--;
                                userKingLocationY--;
                            }
                            else delete[]cOpy;
                        }
                        if(i+1<=7 && (ptr_X[i+1][j]<'b' || 'r'<ptr_X[i+1][j]))                    //movement towards 06:00
                        if(2<=abs(engineKingLocationX-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                            cOpy[i][j]=' ';
                            cOpy[i+1][j]='k';
                            if(isUserSquareCaptured(userKingLocationX, userKingLocationY+1, cOpy)==false)
                            {
                                userKingLocationY++;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationY--;
                            }
                            else delete[]cOpy;
                        }
                        if(i+1<=7 && 0<=j-1 && (ptr_X[i+1][j-1]<'b' || 'r'<ptr_X[i+1][j-1]))      //movement towards 07:30
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                            cOpy[i][j]=' ';
                            cOpy[i+1][j-1]='k';
                            if(isUserSquareCaptured(userKingLocationX-1, userKingLocationY+1, cOpy)==false)
                            {
                                userKingLocationX--;
                                userKingLocationY++;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX++;
                                userKingLocationY--;
                            }
                            else delete[]cOpy;
                        }
                        if(0<=j-1 && (ptr_X[i][j-1]<'b' || 'r'<ptr_X[i][j-1]))                    //movement towards 09:00
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                            cOpy[i][j]=' ';
                            cOpy[i][j-1]='k';
                            if(isUserSquareCaptured(userKingLocationX-1, userKingLocationY, cOpy)==false)
                            {
                                userKingLocationX--;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX++;
                            }
                            else delete[]cOpy;
                        }
                        if(0<=i-1 && 0<=j-1 && (ptr_X[i-1][j-1]<'b' || 'r'<ptr_X[i-1][j-1]))      //movement towards 10:30
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                            cOpy[i][j]=' ';
                            cOpy[i-1][j-1]='k';
                            if(isUserSquareCaptured(userKingLocationX-1, userKingLocationY-1, cOpy)==false)
                            {
                                userKingLocationX--;
                                userKingLocationY--;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX++;
                                userKingLocationY++;
                            }
                            else delete[]cOpy;
                        }
                        if(!color)                                                                      //castling when the user play black
                        {
                            if(userKingMoved==false && userLeftRookMoved==false && ptr_X[0][1]==' ' && ptr_X[0][2]==' ' && ptr_X[0][3]==' ')       //long castle
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                                if(isUserSquareCaptured(2, 0, cOpy)==false && isUserSquareCaptured(3, 0, cOpy)==false && isUserSquareCaptured(4, 0, cOpy)==false)
                                {
                                    cOpy[0][0]=' ';
                                    cOpy[0][4]=' ';
                                    cOpy[0][2]='k';
                                    cOpy[0][3]='r';
                                    userKingLocationX-=2;
                                    movements[engineMoveNumber].push_back(cOpy);
                                    userKingLocationX+=2;
                                }
                                else delete[]cOpy;
                            }
                            if(userKingMoved==false && userRightRookMoved==false && ptr_X[0][5]==' ' && ptr_X[0][6]==' ')                          //short caste
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                                if(isUserSquareCaptured(4, 0, cOpy)==false && isUserSquareCaptured(5, 0, cOpy)==false && isUserSquareCaptured(6, 0, cOpy)==false)
                                {
                                    cOpy[0][7]=' ';
                                    cOpy[0][4]=' ';
                                    cOpy[0][6]='k';
                                    cOpy[0][5]='r';
                                    userKingLocationX+=2;
                                    movements[engineMoveNumber].push_back(cOpy);
                                    userKingLocationX-=2;
                                }
                                else delete[]cOpy;
                            }
                        }
                        if(color)                                                                       //castling when the user play white
                        {
                            if(userKingMoved==false && userRightRookMoved==false && ptr_X[0][4]==' ' && ptr_X[0][5]==' ' && ptr_X[0][6]==' ')       //long castle
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                                if(isUserSquareCaptured(3, 0, cOpy)==false && isUserSquareCaptured(4, 0, cOpy)==false && isUserSquareCaptured(5, 0, cOpy)==false)
                                {
                                    cOpy[0][7]=' ';
                                    cOpy[0][3]=' ';
                                    cOpy[0][5]='k';
                                    cOpy[0][4]='r';
                                    userKingLocationX+=2;
                                    movements[engineMoveNumber].push_back(cOpy);
                                    userKingLocationX-=2;
                                }
                                else delete[]cOpy;
                            }
                            if(userKingMoved==false && userLeftRookMoved==false && ptr_X[0][1]==' ' && ptr_X[0][2]==' ')                          //short caste
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
                                if(isUserSquareCaptured(1, 0, cOpy)==false && isUserSquareCaptured(2, 0, cOpy)==false && isUserSquareCaptured(3, 0, cOpy)==false)
                                {
                                    cOpy[0][0]=' ';
                                    cOpy[0][3]=' ';
                                    cOpy[0][1]='k';
                                    cOpy[0][2]='r';
                                    userKingLocationX-=2;
                                    movements[engineMoveNumber].push_back(cOpy);
                                    userKingLocationX+=2;
                                }
                                else delete[]cOpy;
                            }
                        }
                        break;
                    }
                default: break;
            }
        }
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Move::userMovesService  (int fromX, int fromY, int toX, int toY, char bierka, globalType::chessboardPointer ptr_X){//2
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        if(fromX<0 || 7<fromX || fromY<0 || 7<fromY || toX<0 || 7<toX || toY<0 || 7<toY)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        globalType::chessboardPointer cOpy = copyChessboard(ptr_X);
        cOpy[fromY][fromX] = ' ';
        cOpy[toY][toX] = bierka;
        makeUserMovesIfAllowed(fromX, fromY, cOpy);
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Move::makeUserMovesIfAllowed                             (int &x, int &y, globalType::chessboardPointer cOpy){//1
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        if(userKingChecked==false && x!=userKingLocationX && y!=userKingLocationY && (userKingLocationX+userKingLocationY!=x+y) && (userKingLocationX-userKingLocationY!=x-y))
            movements[engineMoveNumber].push_back(cOpy);
        else if(isUserSquareCaptured(userKingLocationX, userKingLocationY, cOpy)==false)
            movements[engineMoveNumber].push_back(cOpy);
        else delete[]cOpy;
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            bool Move::isUserSquareCaptured               (const int &x, const int &y, globalType::chessboardPointer cOpy){//0+
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        for(int i=1; 0<=y-i; i++)                                       //12:00 //BITE PRZEZ HETMANA / WIEZE / GONCA
        {
            if(cOpy[y-i][x]==' ') continue;
            if(cOpy[y-i][x]=='Q' || cOpy[y-i][x]=='R') return true;
            else break;
        }
        for(int i=1; 0<=y-i && x+i<=7; i++)                             //1:30
        {
            if(cOpy[y-i][x+i]==' ') continue;
            if(cOpy[y-i][x+i]=='Q' || cOpy[y-i][x+i]=='B') return true;
            else break;
        }
        for(int i=1; x+i<=7; i++)                                       //3:00
        {
            if(cOpy[y][x+i]==' ') continue;
            if(cOpy[y][x+i]=='Q' || cOpy[y][x+i]=='R') return true;
            else break;
        }
        for(int i=1; y+i<=7 && x+i<=7; i++)                             //4:30
        {
            if(cOpy[y+i][x+i]==' ') continue;
            if(cOpy[y+i][x+i]=='Q' || cOpy[y+i][x+i]=='B') return true;
            else break;
        }
        for(int i=1; y+i<=7; i++)                                       //6:00
        {
            if(cOpy[y+i][x]==' ') continue;
            if(cOpy[y+i][x]=='Q' || cOpy[y+i][x]=='R') return true;
            else break;
        }
        for(int i=1; y+i<=7 && 0<=x-i; i++)                             //7:30
        {
            if(cOpy[y+i][x-i]==' ') continue;
            if(cOpy[y+i][x-i]=='Q' || cOpy[y+i][x-i]=='B') return true;
            else break;
        }
        for(int i=1; 0<=x-i; i++)                                       //9:00
        {
            if(cOpy[y][x-i]==' ') continue;
            if(cOpy[y][x-i]=='Q' || cOpy[y][x-i]=='R') return true;
            else break;
        }
        for(int i=1; 0<=y-i && 0<=x-i; i++)                             //10:30
        {
            if(cOpy[y-i][x-i]==' ') continue;
            if(cOpy[y-i][x-i]=='Q' || cOpy[y-i][x-i]=='B') return true;
            else break;
        }

        if(0<=y-2 && x+1<=7 && cOpy[y-2][x+1]=='N') return true;                       //BITE PRZEZ SKOCZKA
        if(0<=y-1 && x+2<=7 && cOpy[y-1][x+2]=='N') return true;
        if(y+1<=7 && x+2<=7 && cOpy[y+1][x+2]=='N') return true;
        if(y+2<=7 && x+1<=7 && cOpy[y+2][x+1]=='N') return true;
        if(y+2<=7 && 0<=x-1 && cOpy[y+2][x-1]=='N') return true;
        if(y+1<=7 && 0<=x-2 && cOpy[y+1][x-2]=='N') return true;
        if(0<=y-1 && 0<=x-2 && cOpy[y-1][x-2]=='N') return true;
        if(0<=y-2 && 0<=x-1 && cOpy[y-2][x-1]=='N') return true;

        if(y<=5)                                                                    //BITE PRZEZ PIONKA
        {
            if(1<=x && cOpy[y+1][x-1]=='P') return true;
            if(x<=6 && cOpy[y+1][x+1]=='P') return true;
        }
        return false;
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
globalType::chessboardPointer Move::copyChessboard                              (const globalType::chessboardPointer oryginal){//0+
//==============================================================================================================
    try{
        if (oryginal == nullptr)
            throw std::invalid_argument("Attempting to copy the nullptr address of the chessboard.");
        globalType::chessboardPointer cOpy = new char[8][8];
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                cOpy[i][j] = oryginal[i][j];
        return cOpy;
    }
//#########################################################################
    catch(const std::bad_alloc &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
void Move::writeChessboard                                                      (const globalType::chessboardPointer ptr_X){//0+
//==============================================================================================================
    try{
        if (ptr_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");

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
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}

















