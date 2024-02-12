#include "Move.h"

Move::Move(bool k)noexcept: color{k}{
//==============================================================================================================
    movementGeneration = 0;

    engineKingLocationX = color?3:4;
    engineKingLocationY =         7;
    userKingLocationX    = color?3:4;
    userKingLocationY    =         0;

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
    {
        for(auto singeMove: oneDimensionalVector)
            delete[]singeMove;
    }
    movements.clear();
}
double Move::findNextMove                          (globalType::chessboardPointer &wsk_X){//7
//==============================================================================================================
    try{
        if(wsk_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        verifyKingsLocation(wsk_X);
        checkIfRooksAndKingsMoved(wsk_X);
        checkIfEngineKingChecked(wsk_X);
        makeEngineMoves(wsk_X);
        return findBestMove(wsk_X);
//#########################################################################
    }
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
    void Move::verifyKingsLocation                 (globalType::chessboardPointer  wsk_X){//0+
//==============================================================================================================
    try{
        if(wsk_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        for(int i=7; i>=0; i--)
            for(int j=0; j<8; j++)
                if(wsk_X[i][j]=='K')
                {
                    engineKingLocationX = j;
                    engineKingLocationY = i;
                    break;
                }
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                if(wsk_X[i][j]=='k')
                {
                    userKingLocationX = j;
                    userKingLocationY = i;
                    break;
                }
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    void Move::checkIfRooksAndKingsMoved           (globalType::chessboardPointer  wsk_X){//0+
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        if(wsk_X[7][0] != 'W') engineLeftRookMoved  = true;
        if(wsk_X[7][7] != 'W') engineRightRookMoved = true;
        if(wsk_X[0][0] != 'w') userLeftRookMoved    = true;
        if(wsk_X[0][7] != 'w') userRightRookMoved   = true;
        if(color? wsk_X[7][3] != 'K': wsk_X[7][4] != 'K') engineKingMoved = true;
        if(color? wsk_X[0][3] != 'k': wsk_X[0][4] != 'k') userKingMoved   = true;
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    void Move::checkIfEngineKingChecked            (globalType::chessboardPointer  wsk_X){//1
//==============================================================================================================
    try{
        engineKingChecked = checkIfEngineSquareCaptured(engineKingLocationX, engineKingLocationY, wsk_X);
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    double Move::findBestMove                      (globalType::chessboardPointer &wsk_X){//r
//==============================================================================================================
    try{
        if(movementGeneration <  lastMovementGeneration) return beginningSearchingTreeService(wsk_X);
        if(movementGeneration == lastMovementGeneration) return endingSearchingTreeService();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        double Move::beginningSearchingTreeService (globalType::chessboardPointer &wsk_X){//r
//==============================================================================================================
    try{
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
                Move(this, currentMaterialStatus, movements[i][j], movementGeneration + 1);
                if(currentMaterialStatus < worstMaterialStatus)
                    worstMaterialStatus = currentMaterialStatus;
            }
            if(worstMaterialStatus == 110.0)
                if(checkIfUserSquareCaptured(userKingLocationX, userKingLocationY, movements[i][0]))
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
        checkmateAndStalemateSearching(wsk_X, bestMaterialStatusIndex);
        resetMovements();
        return bestMaterialStatus;
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            Move::Move(Move* pointer, double &materialStatus, globalType::chessboardPointer wsk_X, int iteration): movementGeneration {iteration}{//r
//==============================================================================================================
    try{
        rewriteKingsAndRooksMovesData(pointer);
        materialStatus = findNextMove(wsk_X);
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
                void Move::rewriteKingsAndRooksMovesData(Move* wsk){//0+
//==============================================================================================================
    try{
        if (wsk == nullptr)
            throw std::invalid_argument("Nullptr of Move pointer.");
        engineKingMoved      = wsk->engineKingMoved;
        engineLeftRookMoved  = wsk->engineLeftRookMoved;
        engineRightRookMoved = wsk->engineRightRookMoved;
        userKingMoved        = wsk->userKingMoved;
        userLeftRookMoved    = wsk->userLeftRookMoved;
        userRightRookMoved   = wsk->userRightRookMoved;
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
            void Move::checkmateAndStalemateSearching  (globalType::chessboardPointer &wsk_X, int &index){//2
//==============================================================================================================
    try{
        if(movementGeneration == 0)
        {
            checkIfGameFinishedByUser(wsk_X);
            delete[]wsk_X;
            wsk_X = nullptr;
            if( ! gameOver)
                checkIfGameFinishedByEngine(wsk_X, index);
            if(wsk_X == nullptr && movements.size() != 0)
                wsk_X = copyChessboard(movements[index][0]);
        }
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
                void Move::checkIfGameFinishedByUser   (globalType::chessboardPointer &wsk_X){//1
//==============================================================================================================
    try{
        if(movements.size()==0)
        {
            if(checkIfEngineSquareCaptured(engineKingLocationX, engineKingLocationY, wsk_X))
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
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
                void Move::checkIfGameFinishedByEngine (globalType::chessboardPointer &wsk_X, int &index){//1
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
                if(checkIfUserSquareCaptured(userKingLocationX, userKingLocationY, movements[i][0]))
                {
                    wsk_X = copyChessboard(movements[i][0]);
                    gameOverEngineWin = true;
                    gameOver = true;
                    break;
                }
                else if(j == movements.size())
                {
                    wsk_X = copyChessboard(movements[i][0]);
                    gameOverStalemateByEngine = true;
                    gameOver = true;
                    break;
                }
            }
//#########################################################################
    }
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
                if(checkIfUserSquareCaptured(userKingLocationX, userKingLocationY, movements[i][0]))
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
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            double Move::countMiddlegameMaterialStatus (const globalType::chessboardPointer wsk_X){//0+
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
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
                switch(wsk_X[i][j])
                {
                    case ' ': break;
                    case 'k': break;
                    case 'K': break;

                    case 'p': materialStatus += /*(i==6)? -pawnValue :*/ -pawnValue; break;
                    case 's': materialStatus += (i==0 || i==7 || j==0 || j==7)? -sideKnightValue : -knightValue; break;
                    case 'g': materialStatus += (i==0 || i==7 || j==0 || j==7)? -sideBishopValue : -bishopValue; break;
                    case 'w': materialStatus += -rookValue;  break;
                    case 'h': materialStatus += -queenValue; break;

                    case 'P': materialStatus += /*(i==1)? pawnValue :*/ pawnValue; break;
                    case 'S': materialStatus += (i==0 || i==7 || j==0 || j==7)? sideKnightValue : knightValue; break;
                    case 'G': materialStatus += (i==0 || i==7 || j==0 || j==7)? sideBishopValue : bishopValue; break;
                    case 'W': materialStatus +=  rookValue;  break;
                    case 'H': materialStatus +=  queenValue; break;

                    default: throw std::invalid_argument("Wrong sign.");
                }
            }
        return materialStatus;
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
            double Move::countEndgameMaterialStatus    (const globalType::chessboardPointer wsk_X){//0+
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
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
                switch(wsk_X[i][j])
                {
                    case ' ': break;

                    case 'p': materialStatus += -pawnValue;   break;
                    case 's': materialStatus += -knightValue; break;
                    case 'g': materialStatus += -bishopValue; break;
                    case 'w': materialStatus += -rookValue;   break;
                    case 'h': materialStatus += -queenValue;  break;
                    case 'k': materialStatus +=                *globalType::userKingBehaviorPriority1    * globalType::userKingBehaviorPoints1(j,i);
                                                             /*+ *globalType::userKingBehaviorPriority2    * globalType::userKingBehaviorPoints2(j,i);*/break;
                    case 'P': materialStatus +=  pawnValue;   break;
                    case 'S': materialStatus +=  knightValue + *globalType::engineKnightBehaviorPriority * globalType::engineKnightBehaviorPoints(j,i); break;
                    case 'G': materialStatus +=  bishopValue + *globalType::engineBishopBehaviorPriority * globalType::engineBishopBehaviorPoints(j,i); break;
                    case 'W': materialStatus +=  rookValue   + *globalType::engineRookBehaviorPriority1  * globalType::engineRookBehaviorPoints1(j,i)
                                                             + *globalType::engineRookBehaviorPriority2  * globalType::engineRookBehaviorPoints2(j,i);  break;
                    case 'H': materialStatus +=  queenValue  + *globalType::engineQueenBehaviorPriority1 * globalType::engineQueenBehaviorPoints1(j,i)
                                                             + *globalType::engineQueenBehaviorPriority2 * globalType::engineQueenBehaviorPoints2(j,i); break;
                    case 'K': materialStatus +=                *globalType::engineKingBehaviorPriority   * globalType::engineKingBehaviorPoints(j,i);   break;
                    default: throw std::invalid_argument("Wrong sign.");
                }
            }
        return materialStatus;
//#########################################################################
    }
    catch(const std::invalid_argument &e){
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
        if(globalType::userKingX <= x)
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
        if(globalType::userKingY <= y)
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
        if(globalType::userKingX >= x)
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
        if(globalType::userKingY >= y)
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
        if((x == globalType::userKingX || x+1 == globalType::userKingX) && (y == globalType::userKingY || y-1 == globalType::userKingY))
            value += 2.0;
        return value/2.0;
    }
    if(globalType::userKingCornerLocation == globalType::engineDownRightCorner)
    {
        value += 7.0 - abs(globalType::userKingY - 1.0 - y);
        value += 7.0 - abs(globalType::userKingX - 1.0 - x);
        if((x == globalType::userKingX || x+1 == globalType::userKingX) && (y == globalType::userKingY || y+1 == globalType::userKingY))
            value += 2.0;
        return value/2.0;
    }
    if(globalType::userKingCornerLocation == globalType::engineDownLeftCorner)
    {
        value += 7.0 - abs(globalType::userKingY - 1.0 - y);
        value += 7.0 - abs(globalType::userKingX + 1.0 - x);
        if((x == globalType::userKingX || x-1 == globalType::userKingX) && (y == globalType::userKingY || y+1 == globalType::userKingY))
            value += 2.0;
        return value/2.0;
    }
    if(globalType::userKingCornerLocation == globalType::engineUpLeftCorner)
    {
        value += 7.0 - abs(globalType::userKingY + 1.0 - y);
        value += 7.0 - abs(globalType::userKingX + 1.0 - x);
        if((x == globalType::userKingX || x-1 == globalType::userKingX) && (y == globalType::userKingY || y-1 == globalType::userKingY))
            value += 2.0;
        return value/2.0;
    }

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
                //double Move::ooooooo                       (double x, double y)noexcept{
//==============================================================================================================
//}
                double Move::makeNothing                     (double x, double y)noexcept{
//==============================================================================================================
    return 0.0;
}
void Move::makeEngineMoves(globalType::chessboardPointer wsk_X){//6
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        std::vector<int>iterators = {3, 4, 2, 5, 1, 6, 0, 7};
        engineMoveNumber = -1 ;

        for(int i=0; i<8; i++)
            for (int j: iterators)
            {
                switch(wsk_X[i][j])
                {
                    case ' ': break;
                    case 'H':                                                       //queen issue
                    {
                        for(int y=i-1; 0<=y; y--)                                       //movement towards 12:00
                        {
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')                                         break;
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w'){engineMovesService(j,i,j,y,'H',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                     engineMovesService(j,i,j,y,'H',wsk_X);
                        }
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //movement towards 01:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                         break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w'){engineMovesService(j,i,x,y,'H',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                     engineMovesService(j,i,x,y,'H',wsk_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //movement towards 03:00
                        {
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')                                         break;
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w'){engineMovesService(j,i,x,i,'H',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                     engineMovesService(j,i,x,i,'H',wsk_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //movement towards 04:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                         break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w'){engineMovesService(j,i,x,y,'H',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                     engineMovesService(j,i,x,y,'H',wsk_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //movement towards 06:00
                        {
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')                                         break;
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w'){engineMovesService(j,i,j,y,'H',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                     engineMovesService(j,i,j,y,'H',wsk_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //movement towards 07:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                         break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w'){engineMovesService(j,i,x,y,'H',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                     engineMovesService(j,i,x,y,'H',wsk_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //movement towards 09:00
                        {
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')                                         break;
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w'){engineMovesService(j,i,x,i,'H',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                     engineMovesService(j,i,x,i,'H',wsk_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //movement towards 10:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                         break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w'){engineMovesService(j,i,x,y,'H',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                     engineMovesService(j,i,x,y,'H',wsk_X);
                        }
                        break;
                    }
                    case 'S':                                                       //knight issue
                    {
                        if(0<=i-2 && j+1<=7 && (wsk_X[i-2][j+1]<'G' || 'W'<wsk_X[i-2][j+1])) engineMovesService(j,i,j+1,i-2,'S',wsk_X);
                        if(0<=i-1 && j+2<=7 && (wsk_X[i-1][j+2]<'G' || 'W'<wsk_X[i-1][j+2])) engineMovesService(j,i,j+2,i-1,'S',wsk_X);
                        if(i+1<=7 && j+2<=7 && (wsk_X[i+1][j+2]<'G' || 'W'<wsk_X[i+1][j+2])) engineMovesService(j,i,j+2,i+1,'S',wsk_X);
                        if(i+2<=7 && j+1<=7 && (wsk_X[i+2][j+1]<'G' || 'W'<wsk_X[i+2][j+1])) engineMovesService(j,i,j+1,i+2,'S',wsk_X);
                        if(i+2<=7 && 0<=j-1 && (wsk_X[i+2][j-1]<'G' || 'W'<wsk_X[i+2][j-1])) engineMovesService(j,i,j-1,i+2,'S',wsk_X);
                        if(i+1<=7 && 0<=j-2 && (wsk_X[i+1][j-2]<'G' || 'W'<wsk_X[i+1][j-2])) engineMovesService(j,i,j-2,i+1,'S',wsk_X);
                        if(0<=i-1 && 0<=j-2 && (wsk_X[i-1][j-2]<'G' || 'W'<wsk_X[i-1][j-2])) engineMovesService(j,i,j-2,i-1,'S',wsk_X);
                        if(0<=i-2 && 0<=j-1 && (wsk_X[i-2][j-1]<'G' || 'W'<wsk_X[i-2][j-1])) engineMovesService(j,i,j-1,i-2,'S',wsk_X);
                        break;
                    }
                    case 'G':                                                       //bishop issue
                    {
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //movement towards 1:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                          break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w') {engineMovesService(j,i,x,y,'G',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      engineMovesService(j,i,x,y,'G',wsk_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //movement towards 4:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                          break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w') {engineMovesService(j,i,x,y,'G',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      engineMovesService(j,i,x,y,'G',wsk_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //movement towards 7:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                          break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w') {engineMovesService(j,i,x,y,'G',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      engineMovesService(j,i,x,y,'G',wsk_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //movement towards 10:30
                        {
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')                                          break;
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w') {engineMovesService(j,i,x,y,'G',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      engineMovesService(j,i,x,y,'G',wsk_X);
                        }
                        break;
                    }
                    case 'W':                                                       //rook issue
                    {
                        for(int y=i-1; 0<=y; y--)                                       //movement towards 12:00
                        {
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')                                          break;
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w') {engineMovesService(j,i,j,y,'W',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      engineMovesService(j,i,j,y,'W',wsk_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //movement towards 03:00
                        {
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')                                          break;
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w') {engineMovesService(j,i,x,i,'W',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      engineMovesService(j,i,x,i,'W',wsk_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //movement towards 06:00
                        {
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')                                          break;
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w') {engineMovesService(j,i,j,y,'W',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      engineMovesService(j,i,j,y,'W',wsk_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //movement towards 09:00
                        {
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')                                          break;
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w') {engineMovesService(j,i,x,i,'W',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      engineMovesService(j,i,x,i,'W',wsk_X);
                        }
                        break;
                    }
                    case 'P':                                                       // pawn issue
                    {
                        if(1<i && i<7)                                                  // pawn with no chance at promotion
                        {
                            if(wsk_X[i-1][j]==' ')                                   engineMovesService(j,i,j,i-1,'P',wsk_X);    // move forward by 1
                            if(i==6 && wsk_X[5][j]==' ' && wsk_X[4][j]==' ')         engineMovesService(j,i,j,4,'P',wsk_X);      // move forward by 2
                            if(j!=0 && 'g'<=wsk_X[i-1][j-1] && wsk_X[i-1][j-1]<='w') engineMovesService(j,i,j-1,i-1,'P',wsk_X);  // left capture
                            if(j!=7 && 'g'<=wsk_X[i-1][j+1] && wsk_X[i-1][j+1]<='w') engineMovesService(j,i,j+1,i-1,'P',wsk_X);  // right capture
                        }
                        if(i==1)                                                       // promotion
                        {
                            if(wsk_X[0][j]==' ')                                // just promotion with the creation of a
                            {
                                engineMovesService(j,1,j,0,'S',wsk_X);//knight
                                engineMovesService(j,1,j,0,'G',wsk_X);//bishop
                                engineMovesService(j,1,j,0,'W',wsk_X);//rook
                                engineMovesService(j,1,j,0,'H',wsk_X);//queen
                            }
                            if(j!=0 && 'g'<=wsk_X[0][j-1] && wsk_X[0][j-1]<='w')// promotion by left capture with the creation of a
                            {
                                engineMovesService(j,1,j-1,0,'S',wsk_X);//knight
                                engineMovesService(j,1,j-1,0,'G',wsk_X);//bishop
                                engineMovesService(j,1,j-1,0,'W',wsk_X);//rook
                                engineMovesService(j,1,j-1,0,'H',wsk_X);//queen
                            }
                            if(j!=7 && 'g'<=wsk_X[0][j+1] && wsk_X[0][j+1]<='w')// promotion by right capture with the creation of a
                            {
                                engineMovesService(j,1,j+1,0,'S',wsk_X);//knight
                                engineMovesService(j,1,j+1,0,'G',wsk_X);//bishop
                                engineMovesService(j,1,j+1,0,'W',wsk_X);//rook
                                engineMovesService(j,1,j+1,0,'H',wsk_X);//queen
                            }
                        }
                        break;
                    }
                    case 'K':                                                       //king issue
                    {
                        if(0<=i-1 && (wsk_X[i-1][j]<'G' || 'W'<wsk_X[i-1][j]))                    //movement towards 12:00
                        if(2<=abs(engineKingLocationX-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            engineKingLocationY--;
                            engineMovesService(j,i,j,i-1,'K',wsk_X);
                            engineKingLocationY++;
                        }
                        if(0<=i-1 && j+1<=7 && (wsk_X[i-1][j+1]<'G' || 'W'<wsk_X[i-1][j+1]))      //movement towards 1:30
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            engineKingLocationX++;
                            engineKingLocationY--;
                            engineMovesService(j,i,j+1,i-1,'K',wsk_X);
                            engineKingLocationX--;
                            engineKingLocationY++;
                        }
                        if(j+1<=7 && (wsk_X[i][j+1]<'G' || 'W'<wsk_X[i][j+1]))                    //movement towards 3:00
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY-userKingLocationY))
                        {
                            engineKingLocationX++;
                            engineMovesService(j,i,j+1,i,'K',wsk_X);
                            engineKingLocationX--;
                        }
                        if(i+1<=7 && j+1<=7 && (wsk_X[i+1][j+1]<'G' || 'W'<wsk_X[i+1][j+1]))      //movement towards 4:30
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            engineKingLocationX++;
                            engineKingLocationY++;
                            engineMovesService(j,i,j+1,i+1,'K',wsk_X);
                            engineKingLocationX--;
                            engineKingLocationY--;
                        }
                        if(i+1<=7 && (wsk_X[i+1][j]<'G' || 'W'<wsk_X[i+1][j]))                    //movement towards 6:00
                        if(2<=abs(engineKingLocationX-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            engineKingLocationY++;
                            engineMovesService(j,i,j,i+1,'K',wsk_X);
                            engineKingLocationY--;
                        }
                        if(i+1<=7 && 0<=j-1 && (wsk_X[i+1][j-1]<'G' || 'W'<wsk_X[i+1][j-1]))      //movement towards 7:30
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            engineKingLocationX--;
                            engineKingLocationY++;
                            engineMovesService(j,i,j-1,i+1,'K',wsk_X);
                            engineKingLocationX++;
                            engineKingLocationY--;
                        }
                        if(0<=j-1 && (wsk_X[i][j-1]<'G' || 'W'<wsk_X[i][j-1]))                    //movement towards 9:00
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY-userKingLocationY))
                        {
                            engineKingLocationX--;
                            engineMovesService(j,i,j-1,i,'K',wsk_X);
                            engineKingLocationX++;
                        }
                        if(0<=i-1 && 0<=j-1 && (wsk_X[i-1][j-1]<'G' || 'W'<wsk_X[i-1][j-1]))      //movement towards 10:30
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            engineKingLocationX--;
                            engineKingLocationY--;
                            engineMovesService(j,i,j-1,i-1,'K',wsk_X);
                            engineKingLocationX++;
                            engineKingLocationY++;
                        }
                        if(!color)                                                                      //castling when the user play black
                        {
                            if(engineKingMoved==false && engineLeftRookMoved==false && wsk_X[7][1]==' ' && wsk_X[7][2]==' ' && wsk_X[7][3]==' ')       //long castle
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfEngineSquareCaptured(2, 7, cOpy)==false && checkIfEngineSquareCaptured(3, 7, cOpy)==false && checkIfEngineSquareCaptured(4, 7, cOpy)==false)
                                {
                                    cOpy[7][0]=' ';
                                    cOpy[7][4]=' ';
                                    cOpy[7][2]='K';
                                    cOpy[7][3]='W';
                                    engineKingLocationX-=2;
                                    makeEngineMovesIfAllowed(i, j, cOpy);
                                    engineKingLocationX+=2;
                                }
                                else delete[]cOpy;
                            }
                            if(engineKingMoved==false && engineRightRookMoved==false && wsk_X[7][5]==' ' && wsk_X[7][6]==' ')                          //short caste
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfEngineSquareCaptured(4, 7, cOpy)==false && checkIfEngineSquareCaptured(5, 7, cOpy)==false && checkIfEngineSquareCaptured(6, 7, cOpy)==false)
                                {
                                    cOpy[7][7]=' ';
                                    cOpy[7][4]=' ';
                                    cOpy[7][6]='K';
                                    cOpy[7][5]='W';
                                    engineKingLocationX+=2;
                                    makeEngineMovesIfAllowed(i, j, cOpy);
                                    engineKingLocationX-=2;
                                }
                                else delete[]cOpy;
                            }
                        }
                        if(color)                                                                       //castling when the user play white
                        {
                            if(engineKingMoved==false && engineRightRookMoved==false && wsk_X[7][4]==' ' && wsk_X[7][5]==' ' && wsk_X[7][6]==' ')       //long castle
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfEngineSquareCaptured(3, 7, cOpy)==false && checkIfEngineSquareCaptured(4, 7, cOpy)==false && checkIfEngineSquareCaptured(5, 7, cOpy)==false)
                                {
                                    cOpy[7][7]=' ';
                                    cOpy[7][3]=' ';
                                    cOpy[7][5]='K';
                                    cOpy[7][4]='W';
                                    engineKingLocationX+=2;
                                    makeEngineMovesIfAllowed(i, j, cOpy);
                                    engineKingLocationX-=2;
                                }
                                else delete[]cOpy;
                            }
                            if(engineKingMoved==false && engineLeftRookMoved==false && wsk_X[7][1]==' ' && wsk_X[7][2]==' ')                          //short caste
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfEngineSquareCaptured(1, 7, cOpy)==false && checkIfEngineSquareCaptured(2, 7, cOpy)==false && checkIfEngineSquareCaptured(3, 7, cOpy)==false)
                                {
                                    cOpy[7][0]=' ';
                                    cOpy[7][3]=' ';
                                    cOpy[7][1]='K';
                                    cOpy[7][2]='W';
                                    engineKingLocationX-=2;
                                    makeEngineMovesIfAllowed(i, j, cOpy);
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
//#########################################################################
    }
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
    void Move::engineMovesService(int fromX, int fromY, int toX, int toY, char piece, globalType::chessboardPointer wsk_X){//5
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        if(fromX<0 || 7<fromX || fromY<0 || 7<fromY || toX<0 || 7<toX || toY<0 || 7<toY)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
        cOpy[fromY][fromX] = ' ';
        cOpy[toY][toX] = piece;
        makeEngineMovesIfAllowed(fromY, fromX, cOpy);
//#########################################################################
    }
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
        void Move::makeEngineMovesIfAllowed(int &y, int &x, globalType::chessboardPointer cOpy){//4
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
        else if(checkIfEngineSquareCaptured(engineKingLocationX, engineKingLocationY, cOpy)==false)
        {//If it's there, isn't it captured after all?
            engineMoveNumber++;
            movements.push_back(std::vector<globalType::chessboardPointer>());
            movements[engineMoveNumber].push_back(cOpy);
            makeUserMoves(cOpy);
        }
        else delete[]cOpy;
//#########################################################################
    }
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
            bool Move::checkIfEngineSquareCaptured(const int &x,const int &y, globalType::chessboardPointer cOpy){//0+
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        for(int i=1; 0<=y-i; i++)                                       // if capture from 12:00 direction
        {
            if(cOpy[y-i][x]==' ') continue;
            if(cOpy[y-i][x]=='h' || cOpy[y-i][x]=='w') return true;
            else break;
        }
        for(int i=1; 0<=y-i && x+i<=7; i++)                             // if capture from 01:30 direction
        {
            if(cOpy[y-i][x+i]==' ') continue;
            if(cOpy[y-i][x+i]=='h' || cOpy[y-i][x+i]=='g') return true;
            else break;
        }
        for(int i=1; x+i<=7; i++)                                       // if capture from 03:00 direction
        {
            if(cOpy[y][x+i]==' ') continue;
            if(cOpy[y][x+i]=='h' || cOpy[y][x+i]=='w') return true;
            else break;
        }
        for(int i=1; y+i<=7 && x+i<=7; i++)                             // if capture from 04:30 direction
        {
            if(cOpy[y+i][x+i]==' ') continue;
            if(cOpy[y+i][x+i]=='h' || cOpy[y+i][x+i]=='g') return true;
            else break;
        }
        for(int i=1; y+i<=7; i++)                                       // if capture from 06:00 direction
        {
            if(cOpy[y+i][x]==' ') continue;
            if(cOpy[y+i][x]=='h' || cOpy[y+i][x]=='w') return true;
            else break;
        }
        for(int i=1; y+i<=7 && 0<=x-i; i++)                             // if capture from 07:30 direction
        {
            if(cOpy[y+i][x-i]==' ') continue;
            if(cOpy[y+i][x-i]=='h' || cOpy[y+i][x-i]=='g') return true;
            else break;
        }
        for(int i=1; 0<=x-i; i++)                                       // if capture from 09:00 direction
        {
            if(cOpy[y][x-i]==' ') continue;
            if(cOpy[y][x-i]=='h' || cOpy[y][x-i]=='w') return true;
            else break;
        }
        for(int i=1; 0<=y-i && 0<=x-i; i++)                             // if capture from 10:30 direction
        {
            if(cOpy[y-i][x-i]==' ') continue;
            if(cOpy[y-i][x-i]=='h' || cOpy[y-i][x-i]=='g') return true;
            else break;
        }

        if(0<=y-2 && x+1<=7 && cOpy[y-2][x+1]=='s') return true;                       //BITE PRZEZ SKOCZKA
        if(0<=y-1 && x+2<=7 && cOpy[y-1][x+2]=='s') return true;
        if(y+1<=7 && x+2<=7 && cOpy[y+1][x+2]=='s') return true;
        if(y+2<=7 && x+1<=7 && cOpy[y+2][x+1]=='s') return true;
        if(y+2<=7 && 0<=x-1 && cOpy[y+2][x-1]=='s') return true;
        if(y+1<=7 && 0<=x-2 && cOpy[y+1][x-2]=='s') return true;
        if(0<=y-1 && 0<=x-2 && cOpy[y-1][x-2]=='s') return true;
        if(0<=y-2 && 0<=x-1 && cOpy[y-2][x-1]=='s') return true;

        if(2<=y)                                                                    //BITE PRZEZ PIONKA
        {
            if(1<=x && cOpy[y-1][x-1]=='p') return true;
            if(x<=6 && cOpy[y-1][x+1]=='p') return true;
        }
        return false;
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
void Move::makeUserMoves(globalType::chessboardPointer wsk_X){//3
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        userKingChecked = checkIfUserSquareCaptured(userKingLocationX, userKingLocationY, wsk_X);
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
            {
                switch(wsk_X[i][j])
                {
                    case ' ': break;
                    case 's':                                                       //knight issue
                    {
                        if(0<=i-2 && j+1<=7 && (wsk_X[i-2][j+1]<'g' || 'w'<wsk_X[i-2][j+1]))  userMovesService(j,i,j+1,i-2,'s',wsk_X);
                        if(0<=i-1 && j+2<=7 && (wsk_X[i-1][j+2]<'g' || 'w'<wsk_X[i-1][j+2]))  userMovesService(j,i,j+2,i-1,'s',wsk_X);
                        if(i+1<=7 && j+2<=7 && (wsk_X[i+1][j+2]<'g' || 'w'<wsk_X[i+1][j+2]))  userMovesService(j,i,j+2,i+1,'s',wsk_X);
                        if(i+2<=7 && j+1<=7 && (wsk_X[i+2][j+1]<'g' || 'w'<wsk_X[i+2][j+1]))  userMovesService(j,i,j+1,i+2,'s',wsk_X);
                        if(i+2<=7 && 0<=j-1 && (wsk_X[i+2][j-1]<'g' || 'w'<wsk_X[i+2][j-1]))  userMovesService(j,i,j-1,i+2,'s',wsk_X);
                        if(i+1<=7 && 0<=j-2 && (wsk_X[i+1][j-2]<'g' || 'w'<wsk_X[i+1][j-2]))  userMovesService(j,i,j-2,i+1,'s',wsk_X);
                        if(0<=i-1 && 0<=j-2 && (wsk_X[i-1][j-2]<'g' || 'w'<wsk_X[i-1][j-2]))  userMovesService(j,i,j-2,i-1,'s',wsk_X);
                        if(0<=i-2 && 0<=j-1 && (wsk_X[i-2][j-1]<'g' || 'w'<wsk_X[i-2][j-1]))  userMovesService(j,i,j-1,i-2,'s',wsk_X);
                        break;
                    }
                    case 'g':                                                       //bishop issue
                    {
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //movement towards 1:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                        break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {userMovesService(j,i,x,y,'g',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      userMovesService(j,i,x,y,'g',wsk_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //movement towards 4:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                        break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {userMovesService(j,i,x,y,'g',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      userMovesService(j,i,x,y,'g',wsk_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //movement towards 7:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                        break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {userMovesService(j,i,x,y,'g',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      userMovesService(j,i,x,y,'g',wsk_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //movement towards 10:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                        break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {userMovesService(j,i,x,y,'g',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      userMovesService(j,i,x,y,'g',wsk_X);
                        }
                        break;
                    }
                    case 'w':                                                       //rook issue
                    {
                        for(int y=i-1; 0<=y; y--)                                       //movement towards 12:00
                        {
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')                                        break;
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W') {userMovesService(j,i,j,y,'w',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      userMovesService(j,i,j,y,'w',wsk_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //movement towards 03:00
                        {
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')                                        break;
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W') {userMovesService(j,i,x,i,'w',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      userMovesService(j,i,x,i,'w',wsk_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //movement towards 06:00
                        {
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')                                        break;
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W') {userMovesService(j,i,j,y,'w',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      userMovesService(j,i,j,y,'w',wsk_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //movement towards 09:00
                        {
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')                                        break;
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W') {userMovesService(j,i,x,i,'w',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      userMovesService(j,i,x,i,'w',wsk_X);
                        }
                        break;
                    }
                    case 'h':                                                       //queen issue
                    {
                        for(int y=i-1; 0<=y; y--)                                       //movement towards 12:00
                        {
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')                                        break;
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W') {userMovesService(j,i,j,y,'h',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      userMovesService(j,i,j,y,'h',wsk_X);
                        }
                        for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //movement towards 01:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                        break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {userMovesService(j,i,x,y,'h',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      userMovesService(j,i,x,y,'h',wsk_X);
                        }
                        for(int x=j+1; x<=7; x++)                                       //movement towards 03:00
                        {
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')                                        break;
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W') {userMovesService(j,i,x,i,'h',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      userMovesService(j,i,x,i,'h',wsk_X);
                        }
                        for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //movement towards 04:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                        break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {userMovesService(j,i,x,y,'h',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      userMovesService(j,i,x,y,'h',wsk_X);
                        }
                        for(int y=i+1; y<=7; y++)                                       //movement towards 06:00
                        {
                            if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')                                        break;
                            if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W') {userMovesService(j,i,j,y,'h',wsk_X);  break;}
                            if(wsk_X[y][j]==' ')                      userMovesService(j,i,j,y,'h',wsk_X);
                        }
                        for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //movement towards 07:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                        break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {userMovesService(j,i,x,y,'h',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      userMovesService(j,i,x,y,'h',wsk_X);
                        }
                        for(int x=j-1; 0<=x; x--)                                       //movement towards 09:00
                        {
                            if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')                                        break;
                            if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W') {userMovesService(j,i,x,i,'h',wsk_X);  break;}
                            if(wsk_X[i][x]==' ')                      userMovesService(j,i,x,i,'h',wsk_X);
                        }
                        for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //movement towards 10:30
                        {
                            if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')                                        break;
                            if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W') {userMovesService(j,i,x,y,'h',wsk_X);  break;}
                            if(wsk_X[y][x]==' ')                      userMovesService(j,i,x,y,'h',wsk_X);
                        }
                        break;
                    }
                    case 'p':                                                       //pawn issue
                    {
                        if(0<i && i<6)                                                  //pawn with no chance at promotion
                        {
                            if(wsk_X[i+1][j]==' ')                                    userMovesService(j,i,j,i+1,'p',wsk_X);   //move forward by 1
                            if(i==1 && wsk_X[2][j]==' ' && wsk_X[3][j]==' ')          userMovesService(j,1,j,3,'p',wsk_X);     //move forward by 2
                            if(j!=0 && 'G'<=wsk_X[i+1][j-1] && wsk_X[i+1][j-1]<='W')  userMovesService(j,i,j-1,i+1,'p',wsk_X); //left capture
                            if(j!=7 && 'G'<=wsk_X[i+1][j+1] && wsk_X[i+1][j+1]<='W')  userMovesService(j,i,j+1,i+1,'p',wsk_X); //right capture

                        }
                        if(i==6)                                                       //promotion
                        {
                            if(wsk_X[7][j]==' ')                              //just promotion with the creation of a
                            {
                                userMovesService(j,6,j,7,'s',wsk_X); //knight
                                userMovesService(j,6,j,7,'g',wsk_X); //bishop
                                userMovesService(j,6,j,7,'w',wsk_X); //rook
                                userMovesService(j,6,j,7,'h',wsk_X); //queen
                            }
                            if(j!=0 && 'G'<=wsk_X[7][j-1] && wsk_X[7][j-1]<='W')//promotion by left capture with the creation of a
                            {
                                userMovesService(j,6,j-1,7,'s',wsk_X); //knight
                                userMovesService(j,6,j-1,7,'g',wsk_X); //bishop
                                userMovesService(j,6,j-1,7,'w',wsk_X); //rook
                                userMovesService(j,6,j-1,7,'h',wsk_X); //queen
                            }
                            if(j!=7 && 'G'<=wsk_X[7][j+1] && wsk_X[7][j+1]<='W')//promotion by right capture with the creation of a
                            {
                                userMovesService(j,6,j+1,7,'s',wsk_X); //knight
                                userMovesService(j,6,j+1,7,'g',wsk_X); //bishop
                                userMovesService(j,6,j+1,7,'w',wsk_X); //rook
                                userMovesService(j,6,j+1,7,'h',wsk_X); //queen
                            }
                        }
                        break;
                    }
                    case 'k':                                                       //king issue
                    {
                        if(0<=i-1 && (wsk_X[i-1][j]<'g' || 'w'<wsk_X[i-1][j]))                    //movement towards 12:00
                        if(2<=abs(engineKingLocationX-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                            cOpy[i][j]=' ';
                            cOpy[i-1][j]='k';
                            if(checkIfUserSquareCaptured(userKingLocationX, userKingLocationY-1, cOpy)==false)
                            {
                                userKingLocationY--;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationY++;
                            }
                            else delete[]cOpy;
                        }
                        if(0<=i-1 && j+1<=7 && (wsk_X[i-1][j+1]<'g' || 'w'<wsk_X[i-1][j+1]))      //movement towards 01:30
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                            cOpy[i][j]=' ';
                            cOpy[i-1][j+1]='k';
                            if(checkIfUserSquareCaptured(userKingLocationX+1, userKingLocationY-1, cOpy)==false)
                            {
                                userKingLocationX++;
                                userKingLocationY--;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX--;
                                userKingLocationY++;
                            }
                            else delete[]cOpy;
                        }
                        if(j+1<=7 && (wsk_X[i][j+1]<'g' || 'w'<wsk_X[i][j+1]))                    //movement towards 03:00
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                            cOpy[i][j]=' ';
                            cOpy[i][j+1]='k';
                            if(checkIfUserSquareCaptured(userKingLocationX+1, userKingLocationY, cOpy)==false)
                            {
                                userKingLocationX++;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX--;
                            }
                            else delete[]cOpy;
                        }
                        if(i+1<=7 && j+1<=7 && (wsk_X[i+1][j+1]<'g' || 'w'<wsk_X[i+1][j+1]))      //movement towards 04:30
                        if(2<=abs(engineKingLocationX-1-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                            cOpy[i][j]=' ';
                            cOpy[i+1][j+1]='k';
                            if(checkIfUserSquareCaptured(userKingLocationX+1, userKingLocationY+1, cOpy)==false)
                            {
                                userKingLocationX++;
                                userKingLocationY++;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX--;
                                userKingLocationY--;
                            }
                            else delete[]cOpy;
                        }
                        if(i+1<=7 && (wsk_X[i+1][j]<'g' || 'w'<wsk_X[i+1][j]))                    //movement towards 06:00
                        if(2<=abs(engineKingLocationX-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                            cOpy[i][j]=' ';
                            cOpy[i+1][j]='k';
                            if(checkIfUserSquareCaptured(userKingLocationX, userKingLocationY+1, cOpy)==false)
                            {
                                userKingLocationY++;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationY--;
                            }
                            else delete[]cOpy;
                        }
                        if(i+1<=7 && 0<=j-1 && (wsk_X[i+1][j-1]<'g' || 'w'<wsk_X[i+1][j-1]))      //movement towards 07:30
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY-1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                            cOpy[i][j]=' ';
                            cOpy[i+1][j-1]='k';
                            if(checkIfUserSquareCaptured(userKingLocationX-1, userKingLocationY+1, cOpy)==false)
                            {
                                userKingLocationX--;
                                userKingLocationY++;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX++;
                                userKingLocationY--;
                            }
                            else delete[]cOpy;
                        }
                        if(0<=j-1 && (wsk_X[i][j-1]<'g' || 'w'<wsk_X[i][j-1]))                    //movement towards 09:00
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                            cOpy[i][j]=' ';
                            cOpy[i][j-1]='k';
                            if(checkIfUserSquareCaptured(userKingLocationX-1, userKingLocationY, cOpy)==false)
                            {
                                userKingLocationX--;
                                movements[engineMoveNumber].push_back(cOpy);
                                userKingLocationX++;
                            }
                            else delete[]cOpy;
                        }
                        if(0<=i-1 && 0<=j-1 && (wsk_X[i-1][j-1]<'g' || 'w'<wsk_X[i-1][j-1]))      //movement towards 10:30
                        if(2<=abs(engineKingLocationX+1-userKingLocationX) || 2<=abs(engineKingLocationY+1-userKingLocationY))
                        {
                            globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                            cOpy[i][j]=' ';
                            cOpy[i-1][j-1]='k';
                            if(checkIfUserSquareCaptured(userKingLocationX-1, userKingLocationY-1, cOpy)==false)
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
                            if(userKingMoved==false && userLeftRookMoved==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ' && wsk_X[0][3]==' ')       //long castle
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfUserSquareCaptured(2, 0, cOpy)==false && checkIfUserSquareCaptured(3, 0, cOpy)==false && checkIfUserSquareCaptured(4, 0, cOpy)==false)
                                {
                                    cOpy[0][0]=' ';
                                    cOpy[0][4]=' ';
                                    cOpy[0][2]='k';
                                    cOpy[0][3]='w';
                                    userKingLocationX-=2;
                                    movements[engineMoveNumber].push_back(cOpy);
                                    userKingLocationX+=2;
                                }
                                else delete[]cOpy;
                            }
                            if(userKingMoved==false && userRightRookMoved==false && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')                          //short caste
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfUserSquareCaptured(4, 0, cOpy)==false && checkIfUserSquareCaptured(5, 0, cOpy)==false && checkIfUserSquareCaptured(6, 0, cOpy)==false)
                                {
                                    cOpy[0][7]=' ';
                                    cOpy[0][4]=' ';
                                    cOpy[0][6]='k';
                                    cOpy[0][5]='w';
                                    userKingLocationX+=2;
                                    movements[engineMoveNumber].push_back(cOpy);
                                    userKingLocationX-=2;
                                }
                                else delete[]cOpy;
                            }
                        }
                        if(color)                                                                       //castling when the user play white
                        {
                            if(userKingMoved==false && userRightRookMoved==false && wsk_X[0][4]==' ' && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')       //long castle
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfUserSquareCaptured(3, 0, cOpy)==false && checkIfUserSquareCaptured(4, 0, cOpy)==false && checkIfUserSquareCaptured(5, 0, cOpy)==false)
                                {
                                    cOpy[0][7]=' ';
                                    cOpy[0][3]=' ';
                                    cOpy[0][5]='k';
                                    cOpy[0][4]='w';
                                    userKingLocationX+=2;
                                    movements[engineMoveNumber].push_back(cOpy);
                                    userKingLocationX-=2;
                                }
                                else delete[]cOpy;
                            }
                            if(userKingMoved==false && userLeftRookMoved==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ')                          //short caste
                            {
                                globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfUserSquareCaptured(1, 0, cOpy)==false && checkIfUserSquareCaptured(2, 0, cOpy)==false && checkIfUserSquareCaptured(3, 0, cOpy)==false)
                                {
                                    cOpy[0][0]=' ';
                                    cOpy[0][3]=' ';
                                    cOpy[0][1]='k';
                                    cOpy[0][2]='w';
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
    //discardWorstUserMove();
//#########################################################################
    }
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
    void Move::userMovesService(int fromX, int fromY, int toX, int toY, char bierka, globalType::chessboardPointer wsk_X){//2
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        if(fromX<0 || 7<fromX || fromY<0 || 7<fromY || toX<0 || 7<toX || toY<0 || 7<toY)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        globalType::chessboardPointer cOpy = copyChessboard(wsk_X);
        cOpy[fromY][fromX] = ' ';
        cOpy[toY][toX] = bierka;
        makeUserMovesIfAllowed(fromY, fromX, cOpy);
//#########################################################################
    }
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
        void Move::makeUserMovesIfAllowed(int &y, int &x, globalType::chessboardPointer cOpy){//1
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        if(userKingChecked==false && x!=userKingLocationX && y!=userKingLocationY && (userKingLocationX+userKingLocationY!=x+y) && (userKingLocationX-userKingLocationY!=x-y))
            movements[engineMoveNumber].push_back(cOpy);
        else if(checkIfUserSquareCaptured(userKingLocationX, userKingLocationY, cOpy)==false)
            movements[engineMoveNumber].push_back(cOpy);
        else delete[]cOpy;
//#########################################################################
    }
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
            bool Move::checkIfUserSquareCaptured(const int &x, const int &y, globalType::chessboardPointer cOpy){//0+
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw std::invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        for(int i=1; 0<=y-i; i++)                                       //12:00 //BITE PRZEZ HETMANA / WIEZE / GONCA
        {
            if(cOpy[y-i][x]==' ') continue;
            if(cOpy[y-i][x]=='H' || cOpy[y-i][x]=='W') return true;
            else break;
        }
        for(int i=1; 0<=y-i && x+i<=7; i++)                             //1:30
        {
            if(cOpy[y-i][x+i]==' ') continue;
            if(cOpy[y-i][x+i]=='H' || cOpy[y-i][x+i]=='G') return true;
            else break;
        }
        for(int i=1; x+i<=7; i++)                                       //3:00
        {
            if(cOpy[y][x+i]==' ') continue;
            if(cOpy[y][x+i]=='H' || cOpy[y][x+i]=='W') return true;
            else break;
        }
        for(int i=1; y+i<=7 && x+i<=7; i++)                             //4:30
        {
            if(cOpy[y+i][x+i]==' ') continue;
            if(cOpy[y+i][x+i]=='H' || cOpy[y+i][x+i]=='G') return true;
            else break;
        }
        for(int i=1; y+i<=7; i++)                                       //6:00
        {
            if(cOpy[y+i][x]==' ') continue;
            if(cOpy[y+i][x]=='H' || cOpy[y+i][x]=='W') return true;
            else break;
        }
        for(int i=1; y+i<=7 && 0<=x-i; i++)                             //7:30
        {
            if(cOpy[y+i][x-i]==' ') continue;
            if(cOpy[y+i][x-i]=='H' || cOpy[y+i][x-i]=='G') return true;
            else break;
        }
        for(int i=1; 0<=x-i; i++)                                       //9:00
        {
            if(cOpy[y][x-i]==' ') continue;
            if(cOpy[y][x-i]=='H' || cOpy[y][x-i]=='W') return true;
            else break;
        }
        for(int i=1; 0<=y-i && 0<=x-i; i++)                             //10:30
        {
            if(cOpy[y-i][x-i]==' ') continue;
            if(cOpy[y-i][x-i]=='H' || cOpy[y-i][x-i]=='G') return true;
            else break;
        }

        if(0<=y-2 && x+1<=7 && cOpy[y-2][x+1]=='S') return true;                       //BITE PRZEZ SKOCZKA
        if(0<=y-1 && x+2<=7 && cOpy[y-1][x+2]=='S') return true;
        if(y+1<=7 && x+2<=7 && cOpy[y+1][x+2]=='S') return true;
        if(y+2<=7 && x+1<=7 && cOpy[y+2][x+1]=='S') return true;
        if(y+2<=7 && 0<=x-1 && cOpy[y+2][x-1]=='S') return true;
        if(y+1<=7 && 0<=x-2 && cOpy[y+1][x-2]=='S') return true;
        if(0<=y-1 && 0<=x-2 && cOpy[y-1][x-2]=='S') return true;
        if(0<=y-2 && 0<=x-1 && cOpy[y-2][x-1]=='S') return true;

        if(y<=5)                                                                    //BITE PRZEZ PIONKA
        {
            if(1<=x && cOpy[y+1][x-1]=='P') return true;
            if(x<=6 && cOpy[y+1][x+1]=='P') return true;
        }
        return false;
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
globalType::chessboardPointer Move::copyChessboard(const globalType::chessboardPointer oryginal){//0+
//==============================================================================================================
    try{
        if (oryginal == nullptr)
            throw std::invalid_argument("Attempting to copy the nullptr address of the chessboard.");
        globalType::chessboardPointer cOpy = new char[8][8];
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                cOpy[i][j] = oryginal[i][j];
        return cOpy;
//#########################################################################
    }
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
void Move::writeChessboard(const globalType::chessboardPointer wsk_X){//0+
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");

        std::cout<<std::string(23,'\333')<<std::endl;
        std::cout<<std::string(3 ,'\333')<<" 0 1 2 3 4 5 6 7 "<<std::string(3 ,'\333')<<std::endl;
        std::cout<<std::string(23,'\333')<<std::endl;
        //for(int i=0; i<8; i++)
        for(int i=7; i>=0; i--)
        {
            std::cout<<"\333"<<i<<"\333";
            for(int j=7; j>=0; j--)
                std::cout<<' '<<wsk_X[i][j];
            std::cout<<' ';
            std::cout<<"\333"<<i<<"\333"<<std::endl;
        }
        std::cout<<std::string(23,'\333')<<std::endl;
        std::cout<<std::string(3 ,'\333')<<" 0 1 2 3 4 5 6 7 "<<std::string(3 ,'\333')<<std::endl;
        std::cout<<std::string(23,'\333')<<std::endl;
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}

















