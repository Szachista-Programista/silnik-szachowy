#include "Engine.h"

Engine::Engine(bool k): color{k}, movement{k}
{
    globalType::gameStage        = globalType::opening;
    comparativeChessboardPointer = loadPiecesArrangement();
    workingChessboardPointer     = loadPiecesArrangement();
    setArrangements(workingChessboardPointer);
}
    globalType::chessboardPointer Engine::loadPiecesArrangement()
{
    std::ifstream reading;
    std::string line;
    globalType::chessboardPointer chessboard;
    try
    {
        chessboard = new char[8][8];
    }
    catch(const std::bad_alloc &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    try
    {
        reading.open("chessboard.txt");
        if (!reading.is_open())
            throw std::ifstream::failure("The file 'chessboard.txt' cannot be opened .");

        for(int i=0; i<8; i++)
        {
            if (!getline(reading, line))
                throw std::ifstream::failure("Error reading character from 'chessboard.txt' file .");
            for(int j=0; j<8; j++)
                if(line[j] == '*')
                    chessboard[i][j] = ' ';
                else if(line[j] == '.')
                {
                    if(i==0) {
                        if(j==3) chessboard[i][j] = color?'k':'q';
                        if(j==4) chessboard[i][j] = color?'q':'k';
                    }
                    if(i==7) {
                        if(j==3) chessboard[i][j] = color?'K':'Q';
                        if(j==4) chessboard[i][j] = color?'Q':'K';
                    }
                }
                else
                    chessboard[i][j] = line[j];
        }
    }
    catch(const std::ifstream::failure &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    reading.close();
    return chessboard;
}
Engine::~Engine()noexcept
{
    delete[]comparativeChessboardPointer;
    delete[]workingChessboardPointer;
    clearArrangements();
}
    void Engine::clearArrangements()noexcept
{
    for(auto& element: arrangements)
        delete[]element;
    controlNumbersOfArrangements.clear();
    arrangements.clear();
}
bool Engine::canUserMakeSuchMove(int userMoveCode)
{
    decipherUserMove(userMoveCode);
    if(!isPieceOfUserOnStartingSquare()   )return false;
    if( isPieceOfUserOnFinalSquare()      )return false;
    if( isThisMoveExposesKingToCapture()  )return false;
    if(!isAllowedMove()                   )return false;
    return true;
}
    void Engine::decipherUserMove(int userMoveCode)
{
    if(userMoveCode == 10000)
        u.toY = u.toX = u.fromY = u.fromX = promotionCode = 0;
    else
    {
    u.toY = userMoveCode    % 10;
    userMoveCode /=10;
    u.toX = userMoveCode    % 10;
    userMoveCode /=10;
    u.fromY  = userMoveCode % 10;
    userMoveCode /=10;
    u.fromX  = userMoveCode % 10;
    userMoveCode /=10;
    promotionCode = userMoveCode;
    }
    try
    {
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::runtime_error("User movement coordinates out of range.");
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    bool Engine::isPieceOfUserOnStartingSquare()
{
    try
    {
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::runtime_error("User movement coordinates out of range.");
        if (workingChessboardPointer == nullptr)
            throw std::runtime_error("Nullptr of the chessboard.");
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    globalType::chessboardPointer ptr_X = workingChessboardPointer;
    if(ptr_X[u.fromY][u.fromX]<'b' && 'r'<ptr_X[u.fromY][u.fromX])
        return false;
    else
        return true;
}
    bool Engine::isPieceOfUserOnFinalSquare()
{
    try
    {
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::runtime_error("User movement coordinates out of range.");
        if (workingChessboardPointer == nullptr)
            throw std::runtime_error("Nullptr of the chessboard.");
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    globalType::chessboardPointer ptr_X = workingChessboardPointer;
    if(ptr_X[u.toY][u.toX]<'b' || 'r'<ptr_X[u.toY][u.toX])
        return false;
    else
        return true;
}
    bool Engine::isThisMoveExposesKingToCapture()
{
    try
    {
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::runtime_error("User movement coordinates out of range.");
        if (workingChessboardPointer == nullptr)
            throw std::runtime_error("Nullptr of the chessboard.");
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    globalType::chessboardPointer ptr_X = workingChessboardPointer;
    int potentiallyUserKingLocationX;
    int potentiallyUserKingLocationY;

    if(ptr_X[u.fromY][u.fromX]=='k') //is it move of user's king
    {
        potentiallyUserKingLocationX = u.toX;
        potentiallyUserKingLocationY = u.toY;
    }
    else
    {
        potentiallyUserKingLocationX = movement.userKingLocationX;
        potentiallyUserKingLocationY = movement.userKingLocationY;
    }
    globalType::chessboardPointer cOpy = movement.copyChessboard(ptr_X);
    cOpy[u.toY][u.toX]=cOpy[u.fromY][u.fromX];
    cOpy[u.fromY][u.fromX]=' ';
    if(movement.isUserSquareCaptured(potentiallyUserKingLocationX, potentiallyUserKingLocationY, cOpy))
    {
        delete[]cOpy;
        return true;
    }
    else
    {
        delete[]cOpy;
        return false;
    }
}
    bool Engine::isAllowedMove()
{
    try
    {
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::runtime_error("User movement coordinates out of range.");
        if (e.fromX < 0 || 7 < e.fromX || e.fromY < 0 || 7 < e.fromY || e.toX < 0 || 7 < e.toX || e.toY < 0 || 7 < e.toY)
            throw std::runtime_error("Engine movement coordinates out of range.");
        if (workingChessboardPointer == nullptr)
            throw std::runtime_error("Nullptr of the chessboard.");
        switch(workingChessboardPointer[u.fromY][u.fromX])
        {
            case 'p':
                return isAllowedMoveByPawn();
            case 'n':
                return isAllowedMoveByKnight();
            case 'b':
                return isAllowedSlantMove();
            case 'q':
                return (isAllowedSlantMove() || isAllowedNonslantMove());
            case 'r':
                return isAllowedNonslantMove();
            case 'k':
                return isAllowedMovebyKing();
            default: throw std::runtime_error("Wrong sign.");
        }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
        bool Engine::isAllowedMoveByPawn()
{
    globalType::chessboardPointer ptr_X = workingChessboardPointer;
    if(u.fromY==1 && u.toY-u.fromY==2 && u.fromX==u.toX && ptr_X[u.fromY+1][u.toX]==' ' && ptr_X[u.toY][u.toX]==' ') return true;//move 2 forward
    if(u.toY-u.fromY==1 && u.fromX==u.toX && ptr_X[u.toY][u.toX]==' ')                                               return true;//move 1 forward
    if(u.toY-u.fromY==1 && u.fromX-u.toX==1 && 'B'<=ptr_X[u.toY][u.toX] && ptr_X[u.toY][u.toX]<='R')                 return true;//capture <<
    if(u.toY-u.fromY==1 && u.toX-u.fromX==1 && 'B'<=ptr_X[u.toY][u.toX] && ptr_X[u.toY][u.toX]<='R')                 return true;//capture >>
    if(ptr_X[e.toY][e.toX] == 'P' && e.fromY-e.toY == 2 && e.toX == u.toX && u.fromY == e.toY)
    {
        if(u.toY-u.fromY==1 && u.fromX-u.toX==1 && ' ' == ptr_X[u.toY][u.toX]) return true;//En passant <<
        if(u.toY-u.fromY==1 && u.toX-u.fromX==1 && ' ' == ptr_X[u.toY][u.toX]) return true;//En passant >>
    }
    return false;
}
        bool Engine::isAllowedMoveByKnight()
{
    if((abs(u.fromX-u.toX)==2 && abs(u.fromY-u.toY)==1) || (abs(u.fromX-u.toX)==1 && abs(u.fromY-u.toY)==2))
        return true;
    else
        return false;
}
        bool Engine::isAllowedSlantMove()
{
    globalType::chessboardPointer ptr_X = workingChessboardPointer;
    if(u.fromX-u.fromY==u.toX-u.toY) // slant movements (\)
    {
        if(u.fromX<u.toX) //movement towards 04:30
            for(int x=u.fromX+1, y=u.fromY+1; x<u.toX && y<u.toY; x++, y++)
                if(ptr_X[y][x]!=' ')
                    return false;
        if(u.fromX>u.toX) //movement towards 10:30
            for(int x=u.fromX-1, y=u.fromY-1; x>u.toX && y>u.toY; x--, y--)
                if(ptr_X[y][x]!=' ')
                    return false;
        return true;
    }
    if(u.fromX+u.fromY==u.toX+u.toY) // slant movements (/)
    {
        if(u.fromX<u.toX) //movement towards 01:30
            for(int x=u.fromX+1, y=u.fromY-1; x<u.toX && y>u.toY; x++, y--)
                if(ptr_X[y][x]!=' ')
                    return false;
        if(u.fromX>u.toX) //movement towards 07:30
            for(int x=u.fromX-1, y=u.fromY+1; x>u.toX && y<u.toY; x--, y++)
                if(ptr_X[y][x]!=' ')
                    return false;
        return true;
    }
    return false;
}
        bool Engine::isAllowedNonslantMove()
{
    globalType::chessboardPointer ptr_X = workingChessboardPointer;
    if(u.fromY == u.toY) //horizontal movements
    {
        if(u.fromX>u.toX) //to left
            for(int x=u.fromX-1; x>u.toX; x--)
                if(ptr_X[u.fromY][x]!=' ')
                    return false;
        if(u.fromX<u.toX) //to right
            for(int x=u.fromX+1; x<u.toX; x++)
                if(ptr_X[u.fromY][x]!=' ')
                    return false;
        return true;
    }
    if(u.fromX == u.toX) //vertical movements
    {
        if(u.fromY>u.toY)//up
            for(int y=u.fromY-1; y>u.toY; y--)
                if(ptr_X[y][u.fromX]!=' ')
                    return false;
        if(u.fromY<u.toY)//down
            for(int y=u.fromY+1; y<u.toY; y++)
                if(ptr_X[y][u.fromX]!=' ')
                    return false;
        return true;
    }
    return false;
}
        bool Engine::isAllowedMovebyKing()
{
    globalType::chessboardPointer ptr_X = workingChessboardPointer;
    if(abs(u.fromX-u.toX)<=1 && abs(u.fromY-u.toY)<=1)//ordinary king's movement
    if(0<=u.toY-1                ? ptr_X[u.toY-1][u.toX  ] != 'K': true)
    if(0<=u.toY-1 && u.toX+1<=7  ? ptr_X[u.toY-1][u.toX+1] != 'K': true)
    if(              u.toX+1<=7  ? ptr_X[u.toY  ][u.toX+1] != 'K': true)
    if(u.toY+1<=7 && u.toX+1<=7  ? ptr_X[u.toY+1][u.toX+1] != 'K': true)
    if(u.toY+1<=7                ? ptr_X[u.toY+1][u.toX  ] != 'K': true)
    if(u.toY+1<=7 && 0<=u.toX-1  ? ptr_X[u.toY+1][u.toX-1] != 'K': true)
    if(              0<=u.toX-1  ? ptr_X[u.toY  ][u.toX-1] != 'K': true)
    if(u.toY+1<=7 && 0<=u.toX-1  ? ptr_X[u.toY+1][u.toX-1] != 'K': true)
        return true;
    if(u.fromY==u.toY && abs(u.fromX - u.toX) == 2 && movement.userKingMoved==false)//castle
    {
        if(color==0)//when user play black
        {
            if(u.toX==2 && movement.userLeftRookMoved ==false && ptr_X[0][1]==' ' && ptr_X[0][2]==' ' && ptr_X[0][3]==' ')//long castle
            if(!movement.isUserSquareCaptured(2, 0, ptr_X) && !movement.isUserSquareCaptured(3, 0, ptr_X) && !movement.isUserSquareCaptured(4, 0, ptr_X))
                return true;
            if(u.toX==6 && movement.userRightRookMoved==false && ptr_X[0][5]==' ' && ptr_X[0][6]==' ')//short castle
            if(!movement.isUserSquareCaptured(4, 0, ptr_X) && !movement.isUserSquareCaptured(5, 0, ptr_X) && !movement.isUserSquareCaptured(6, 0, ptr_X))
                return true;
        }
        if(color==1)//when user play white
        {
            if(u.toX==5 && movement.userRightRookMoved==false && ptr_X[0][4]==' ' && ptr_X[0][5]==' ' && ptr_X[0][6]==' ')//long castle
            if(!movement.isUserSquareCaptured(3, 0, ptr_X) && !movement.isUserSquareCaptured(4, 0, ptr_X) && !movement.isUserSquareCaptured(5, 0, ptr_X))
                return true;
            if(u.toX==1 && movement.userLeftRookMoved ==false && ptr_X[0][1]==' ' && ptr_X[0][2]==' ')//short castle
            if(!movement.isUserSquareCaptured(1, 0, ptr_X) && !movement.isUserSquareCaptured(2, 0, ptr_X) && !movement.isUserSquareCaptured(3, 0, ptr_X))
                return true;
        }
    }
    return false;
}
int Engine::makeMove                         (int userMoveCode)
{
    getEngineReadyForMove(userMoveCode);
    arrangeServiceAfterUserMove(userMoveCode);
    if( ! movement.gameOver)
        switch(globalType::gameStage)
        {
            case globalType::opening:     makeOpeningMove();    break;
            case globalType::middlegame:  makeMiddlegameMove(); break;
            case globalType::endgame:     makeEndgameMove();    break;
        }
    if( ! movement.gameOver)
        arrangeServiceAfterEngineMove();
    return engineMoveCoding();
}
    void Engine::getEngineReadyForMove       (int userMoveCode)
{
    movementNumber++;
    decipherUserMove(userMoveCode); // changes e.g. 4456 into u.fromY u.toY u.fromX u.toX
    markUserMoveOnChessboard(userMoveCode); // marks u.fromY u.toY u.fromX u.toX into comparativeChessboardPointer and workingChessboardPointer
}
        void Engine::markUserMoveOnChessboard(int userMoveCode)
{
    try
    {
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::runtime_error("User movement coordinates out of range.");
        if (comparativeChessboardPointer == nullptr || workingChessboardPointer == nullptr)
            throw std::runtime_error("Nullptr of the chessboard.");
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(userMoveCode == 10000)
        return;
    char movedPiece = workingChessboardPointer[u.fromY][u.fromX];

    if(movedPiece == 'p' && u.fromY == 4 && u.fromX != u.toX && workingChessboardPointer[u.toY][u.toX] == ' ') //maby en passant
        comparativeChessboardPointer[u.fromY][u.toX] = workingChessboardPointer[u.fromY][u.toX] = ' ';

    comparativeChessboardPointer[u.toY][u.toX]     = workingChessboardPointer[u.toY][u.toX]     = workingChessboardPointer[u.fromY][u.fromX];
    comparativeChessboardPointer[u.fromY][u.fromX] = workingChessboardPointer[u.fromY][u.fromX] = ' ';

    if(promotionCode)
        switch(promotionCode)
        {
            case 1: comparativeChessboardPointer[u.toY][u.toX] = workingChessboardPointer[u.toY][u.toX] = 'n'; break;
            case 2: comparativeChessboardPointer[u.toY][u.toX] = workingChessboardPointer[u.toY][u.toX] = 'b'; break;
            case 3: comparativeChessboardPointer[u.toY][u.toX] = workingChessboardPointer[u.toY][u.toX] = 'r'; break;
            case 4: comparativeChessboardPointer[u.toY][u.toX] = workingChessboardPointer[u.toY][u.toX] = 'q'; break;
            default: break;
        }

    if(movedPiece == 'k')//maby it comes to castles
    {
        if(u.fromX == 3 && u.toX == 1) // O-O of wfite
        {
            comparativeChessboardPointer[0][0] = workingChessboardPointer[0][0] = ' ';
            comparativeChessboardPointer[0][2] = workingChessboardPointer[0][2] = 'r';
        }
        if(u.fromX == 3 && u.toX == 5) // O-O-O of wfite
        {
            comparativeChessboardPointer[0][7] = workingChessboardPointer[0][7] = ' ';
            comparativeChessboardPointer[0][4] = workingChessboardPointer[0][4] = 'r';
        }
        if(u.fromX == 4 && u.toX == 2) // O-O-O of black
        {
            comparativeChessboardPointer[0][0] = workingChessboardPointer[0][0] = ' ';
            comparativeChessboardPointer[0][3] = workingChessboardPointer[0][3] = 'r';
        }
        if(u.fromX == 4 && u.toX == 6) // O-O of black
        {
            comparativeChessboardPointer[0][7] = workingChessboardPointer[0][7] = ' ';
            comparativeChessboardPointer[0][5] = workingChessboardPointer[0][5] = 'r';
        }
    }
}
    void Engine::arrangeServiceAfterUserMove (int userMoveCode)
{
    if(userMoveCode == 10000)
        return;
    setArrangements(workingChessboardPointer);
    if(isArrangementRepeatedThirdTime())
    {
        movement.gameOver = true;
        movement.gameOverStalemateByUser = true;
        delete[]workingChessboardPointer;
        workingChessboardPointer = nullptr;
    }
}
        void Engine::setArrangements(globalType::chessboardPointer ptr_X)
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
    uint64_t controlNumber{};

    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            if(ptr_X[i][j] != ' ')
            {
                controlNumber <<= 1;
                controlNumber  += 1;
            }
            else
                controlNumber <<= 1;
        }
    controlNumbersOfArrangements.push_back(controlNumber);
    arrangements.push_back(movement.copyChessboard(ptr_X));
}
        bool Engine::isArrangementRepeatedThirdTime()
{
    try
    {
        if(arrangements.size() == 0 || controlNumbersOfArrangements.size() == 0)
            throw std::length_error("Empty vector.");
    }
    catch(const std::length_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    globalType::chessboardPointer currentArrangement = arrangements.back();
    uint64_t currentControlNumber = controlNumbersOfArrangements.back();
    int occurrenceNumber{};

    for(int i=0; i<controlNumbersOfArrangements.size(); i++)
        if(controlNumbersOfArrangements[i] == currentControlNumber)
            if(compareChessboards(arrangements[i], currentArrangement))
                occurrenceNumber++;

    if(occurrenceNumber >= 3)
        return true;
    else
        return false;
}
            bool Engine::isControlNumberRepeatedThirdTime()
{
    try
    {
        if(controlNumbersOfArrangements.size() == 0)
            throw std::length_error("Empty vector.");
    }
    catch(const std::length_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    uint64_t currentControlNumber = controlNumbersOfArrangements.back();
    int occurrenceNumber{};

    for(auto& element: controlNumbersOfArrangements)
        if(currentControlNumber == element)
            occurrenceNumber++;
    if(occurrenceNumber >= 3)
        return true;
    else
        return false;
}
            bool Engine::compareChessboards(globalType::chessboardPointer ptr_A, globalType::chessboardPointer ptr_B)
{
    try
    {
        if(ptr_A == nullptr || ptr_B == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            if(ptr_A[i][j] == ptr_B[i][j])
                continue;
            else
                return false;
    return true;
}
    void Engine::arrangeServiceAfterEngineMove()
{
    setArrangements(workingChessboardPointer);
    if(isArrangementRepeatedThirdTime())
    {
        movement.gameOver = true;
        movement.gameOverStalemateByEngine = true;
    }
}
    void Engine::makeOpeningMove()noexcept
{
    if(color)
        blackOpeningMove();
    else
        whiteOpeningMove();
    if(movementNumber == 3)
        globalType::gameStage = globalType::middlegame;
}
        void Engine::blackOpeningMove()noexcept
{
    userPiecesMovedInOpening += workingChessboardPointer[u.toY][u.toX];
    switch(movementNumber)
    {
        case 1:
            if(((u.toX==2 || u.toX==4) && u.toY==3) || (userPiecesMovedInOpening[0]=='n' && u.toX==2)){
                if(randomChance(3))     { workingChessboardPointer[6][4]=' '; workingChessboardPointer[4][4]='P'; }
                else                    { workingChessboardPointer[6][2]=' '; workingChessboardPointer[4][2]='P'; }
            }
            else                        { workingChessboardPointer[6][3]=' '; workingChessboardPointer[4][3]='P'; }
            break;
        case 2:
            if(userPiecesMovedInOpening[1]=='q' || 4<=u.toY){
                globalType::gameStage = globalType::middlegame;
                makeMiddlegameMove();
                break;
            }
            else if(e.toX==4){
                if(randomChance(3))     { workingChessboardPointer[7][1]=' '; workingChessboardPointer[5][2]='N'; }
                else                    { workingChessboardPointer[6][3]=' '; workingChessboardPointer[5][3]='P'; }
            }
            else if(e.toX==2){
                if(randomChance(3))     { workingChessboardPointer[7][6]=' '; workingChessboardPointer[5][5]='N'; }
                else if(randomChance(2)){ workingChessboardPointer[6][3]=' '; workingChessboardPointer[5][3]='P'; }
                else                    { workingChessboardPointer[6][1]=' '; workingChessboardPointer[5][1]='P'; }
            }
            else                        { workingChessboardPointer[7][6]=' '; workingChessboardPointer[5][5]='N'; }
            break;
        case 3:
            if(userPiecesMovedInOpening[1]=='q' || 4<=u.toY){
                globalType::gameStage = globalType::middlegame;
                makeMiddlegameMove();
                break;
            }
            else if(workingChessboardPointer[7][6]=='N') { workingChessboardPointer[7][6]=' '; workingChessboardPointer[5][5]='N'; }
            else                                         { workingChessboardPointer[7][1]=' '; workingChessboardPointer[5][2]='N'; }
            break;
    }
}
        void Engine::whiteOpeningMove()noexcept
{
    if(1 < movementNumber)
        userPiecesMovedInOpening += workingChessboardPointer[u.toY][u.toX];
    switch(movementNumber)
    {
        case 1:
            if(randomChance(3))     { workingChessboardPointer[6][4]=' '; workingChessboardPointer[4][4]='P'; }
            else if(randomChance(3)){ workingChessboardPointer[6][3]=' '; workingChessboardPointer[4][3]='P'; }
            else if(randomChance(2)){ workingChessboardPointer[7][6]=' '; workingChessboardPointer[5][5]='N'; }
            else                    { workingChessboardPointer[7][1]=' '; workingChessboardPointer[5][2]='N'; }
            break;
        case 2:
            if(workingChessboardPointer[4][4]=='P'){
                if(randomChance(3)){ workingChessboardPointer[7][1]=' '; workingChessboardPointer[5][2]='N'; }
                else               { workingChessboardPointer[7][6]=' '; workingChessboardPointer[5][5]='N'; }
            }
            else if(workingChessboardPointer[4][3]=='p'){
                if(randomChance(3)){ workingChessboardPointer[7][6]=' '; workingChessboardPointer[5][5]='N'; }
                else               { workingChessboardPointer[7][1]=' '; workingChessboardPointer[5][2]='N'; }
            }
            else if(workingChessboardPointer[5][2]=='N')
                                   { workingChessboardPointer[7][6]=' '; workingChessboardPointer[5][5]='N'; }
            else                                        
                                   { workingChessboardPointer[7][1]=' '; workingChessboardPointer[5][2]='N'; }
            break;
        case 3:
            if(userPiecesMovedInOpening[1]=='q' || 4 <= u.toY){
                globalType::gameStage = globalType::middlegame;
                makeMiddlegameMove();
                break;
            }
            else if(workingChessboardPointer[6][4]=='p') { workingChessboardPointer[6][4]=' '; workingChessboardPointer[4][4]='P'; }
            else                                         { workingChessboardPointer[6][3]=' '; workingChessboardPointer[4][3]='P'; }
            break;
    }
}
        int Engine::randomChance(int optionsNumber)noexcept
{
    srand(static_cast<unsigned int>(time(nullptr)));
    return rand() % optionsNumber;
}
    void Engine::makeMiddlegameMove()
{
    movement.findNextMove(workingChessboardPointer);
    if( ! movement.gameOver)
        isItEndgameTime();
}
        void Engine::isItEndgameTime()noexcept
{
    int nrOfUserPawns   {};
    int nrOfUserKnights {};
    int nrOfUserBishops {};
    int nrOfUserRooks   {};
    int nrOfUserQueens  {};

    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            switch(workingChessboardPointer[i][j])
            {
                case 'p': nrOfUserPawns++;     break;
                case 'n': nrOfUserKnights++;   break;
                case 'b': nrOfUserBishops++;   break;
                case 'r': nrOfUserRooks++;     break;
                case 'q': nrOfUserQueens++;    break;
            }
    if(( nrOfUserRooks + nrOfUserQueens <= 1 ) && ( nrOfUserKnights + nrOfUserBishops + nrOfUserRooks + nrOfUserQueens <= 3 ) && ( nrOfUserPawns <= 3 ))
    {
        globalType::gameStage = globalType::endgame;
        setUserKingSideLocation();
        setUserKingCornerLocation();
    }
}
            void Engine::setUserKingSideLocation()noexcept
{
    //if(globalType::gameStage == globalType::middlegame)
       for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                if(workingChessboardPointer[i][j] == 'k')
                {
                    if( 1  <= j-i &&  7  <= j+i)  {globalType::userKingSideLocation = globalType::engineRightSide;  return;}
                    if(j-i <=  0  &&  8  <= j+i)  {globalType::userKingSideLocation = globalType::engineDownSide;   return;}
                    if(j-i <= -1  && j+i <=  7 )  {globalType::userKingSideLocation = globalType::engineLeftSide;   return;}
                    if( 0  <= j-i && j+i <=  6 )  {globalType::userKingSideLocation = globalType::engineUpSide;     return;}
                }
    /*if(globalType::gameStage == globalType::endgame)
       for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                if(workingChessboardPointer[i][j] == 'k')
                {
                    if(6 <= j)  {globalType::userKingSideLocation = globalType::engineRightSide;  return;}
                    if(6 <= i)  {globalType::userKingSideLocation = globalType::engineDownSide;   return;}
                    if(j <= 1)  {globalType::userKingSideLocation = globalType::engineLeftSide;   return;}
                    if(i <= 1)  {globalType::userKingSideLocation = globalType::engineUpSide;     return;}
                }*/
}
            void Engine::setUserKingCornerLocation()noexcept
{
    //if(globalType::gameStage == globalType::middlegame)
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                if(workingChessboardPointer[i][j] == 'k')
                {
                    if(i <= 3 && 4 <= j)  {globalType::userKingCornerLocation = globalType::engineUpRightCorner;   return;}
                    if(4 <= i && 4 <= j)  {globalType::userKingCornerLocation = globalType::engineDownRightCorner; return;}
                    if(4 <= i && j <= 3)  {globalType::userKingCornerLocation = globalType::engineDownLeftCorner;  return;}
                    if(i <= 3 && j <= 3)  {globalType::userKingCornerLocation = globalType::engineUpLeftCorner;    return;}
                }
    /*if(globalType::gameStage == globalType::endgame)
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                if(workingChessboardPointer[i][j] == 'k')
                {
                    if(i <= 2 && 5 <= j)  {globalType::userKingCornerLocation = globalType::engineUpRightCorner;   return;}
                    if(5 <= i && 5 <= j)  {globalType::userKingCornerLocation = globalType::engineDownRightCorner; return;}
                    if(5 <= i && j <= 2)  {globalType::userKingCornerLocation = globalType::engineDownLeftCorner;  return;}
                    if(i <= 2 && j <= 2)  {globalType::userKingCornerLocation = globalType::engineUpLeftCorner;    return;}
                }*/
}
    void Engine::makeEndgameMove()
{
    setKindOfEndgame();
    try
    {
        switch(globalType::choosenEndgame)
        {
            case globalType::rooksAndQueensMate: makeRooksAndQueensMateMove();    break;
            case globalType::rookOrQueenMate:    makeSingleRookOrQueenMateMove(); break;
            case globalType::unspecifiedMate:    makeUnspecifiedMateMove();       break;
            default: throw std::runtime_error("Wrong kind of endgame.");
        }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
        void Engine::setKindOfEndgame()noexcept
{
    //int nrOfEnginePawns   {};
    //int nrOfEngineKnights {};
    //int nrOfEngineBishops {};
    int nrOfEngineRooks   {};
    int nrOfEngineQueens  {};

    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            switch(workingChessboardPointer[i][j])
            {
                //case 'P': nrOfEnginePawns++;   break;
                //case 'N': nrOfEngineKnights++; break;
                //case 'B': nrOfEngineBishops++; break;
                case 'R': nrOfEngineRooks++;   break;
                case 'Q': nrOfEngineQueens++;  break;
            }

    if      ( 2 <= nrOfEngineRooks + nrOfEngineQueens ) {globalType::choosenEndgame = globalType::rooksAndQueensMate; return;}
    else if ( 1 == nrOfEngineRooks + nrOfEngineQueens ) {globalType::choosenEndgame = globalType::rookOrQueenMate;    return;}
    else                                                {globalType::choosenEndgame = globalType::unspecifiedMate;    return;}
}
        void Engine::makeRooksAndQueensMateMove()noexcept
{
    double dot1 = 0.1, dot04 = 0.04, dot01 = 0.01, dot004 = 0.004;

    globalType::userKingBehaviorPriority1    = &dot1;
    globalType::engineKnightBehaviorPriority = &dot01;
    globalType::engineBishopBehaviorPriority = &dot01;
    globalType::engineRookBehaviorPriority1  = &dot04;
    globalType::engineRookBehaviorPriority2  = &dot004;
    globalType::engineQueenBehaviorPriority1 = &dot04;
    globalType::engineQueenBehaviorPriority2 = &dot004;
    globalType::engineKingBehaviorPriority   = &dot01;

    globalType::userKingBehaviorPoints1    = &movement.goToSideOfUserKing;
    globalType::engineKnightBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineBishopBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineRookBehaviorPoints1  = &movement.separateUserKingFromRestOfBoard;
    globalType::engineRookBehaviorPoints2  = &movement.runSidewaysFromKing;
    globalType::engineQueenBehaviorPoints1 = &movement.separateUserKingFromRestOfBoard;
    globalType::engineQueenBehaviorPoints2 = &movement.runSidewaysFromKing;
    globalType::engineKingBehaviorPoints   = &movement.goOppositeDirectionOfUserKing;

    movement.findNextMove(workingChessboardPointer);
}
        void Engine::makeSingleRookOrQueenMateMove()noexcept
{
    double dot1 = 0.1, dot04 = 0.04, dot01 = 0.01, zero = 0.0;

    globalType::userKingBehaviorPriority1    = &dot1;
    globalType::engineKnightBehaviorPriority = &dot01;
    globalType::engineBishopBehaviorPriority = &dot01;
    globalType::engineRookBehaviorPriority1  = &dot1;
    globalType::engineRookBehaviorPriority2  = &zero;
    globalType::engineQueenBehaviorPriority1 = &dot1;
    globalType::engineQueenBehaviorPriority2 = &zero;
    globalType::engineKingBehaviorPriority   = &dot04;

    globalType::userKingBehaviorPoints1    = &movement.goToCornerOfUserKing;
    globalType::engineKnightBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineBishopBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineRookBehaviorPoints1  = &movement.followUserKingToCorner;
    globalType::engineRookBehaviorPoints2  = &movement.makeNothing;
    globalType::engineQueenBehaviorPoints1 = &movement.followUserKingToCorner;
    globalType::engineQueenBehaviorPoints2 = &movement.makeNothing;
    globalType::engineKingBehaviorPoints   = &movement.followUserKingToSide;

    movement.findNextMove(workingChessboardPointer);
}
        void Engine::makeUnspecifiedMateMove()noexcept
{
    double dot1 = 0.1, dot04 = 0.04, dot01 = 0.01, zero = 0.0;

    globalType::userKingBehaviorPriority1    = &dot1;//?
    globalType::engineKnightBehaviorPriority = &dot01;//?
    globalType::engineBishopBehaviorPriority = &dot01;//?
    globalType::engineRookBehaviorPriority1  = &zero;
    globalType::engineRookBehaviorPriority2  = &zero;
    globalType::engineQueenBehaviorPriority1 = &zero;
    globalType::engineQueenBehaviorPriority2 = &zero;
    globalType::engineKingBehaviorPriority   = &dot04;//?

    globalType::userKingBehaviorPoints1    = &movement.goToCornerOfUserKing;//?
    globalType::engineKnightBehaviorPoints = &movement.followUserKingToCorner;//?
    globalType::engineBishopBehaviorPoints = &movement.followUserKingToCorner;//?
    globalType::engineRookBehaviorPoints1  = &movement.makeNothing;
    globalType::engineRookBehaviorPoints2  = &movement.makeNothing;
    globalType::engineQueenBehaviorPoints1 = &movement.makeNothing;
    globalType::engineQueenBehaviorPoints2 = &movement.makeNothing;
    globalType::engineKingBehaviorPoints   = &movement.followUserKingToCorner;//?

    movement.findNextMove(workingChessboardPointer);
}
    int  Engine::engineMoveCoding()noexcept
{
    findEngineMove(); //  finds e.fromY e.toY e.fromX e.toX by comparing workingChessboardPointer and comparativeChessboardPointer
    int engineMoveCode = encodeEngineMove(); // changes e.fromY e.toY e.fromX e.toX into e.g. 1526
    markEngineMoveOnChessboard(); //          comparativeChessboardPointer = workingChessboardPointer
    return engineMoveCode + isItGameOver();
}
        void Engine::findEngineMove()noexcept 
{
    if(workingChessboardPointer == nullptr)
        return;
    bool castle = false;
    for(int i=0,x=0; i<8; i++) //checking whether castling has taken place
    {
        if(workingChessboardPointer[7][i] != comparativeChessboardPointer[7][i])
            x++;
        if(i==7 && x==4)
        castle = true;
    }
    if(castle == false)
    {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
            {
                if(workingChessboardPointer[i][j] == comparativeChessboardPointer[i][j]) //square where nothing has changed
                    continue;
                else if(workingChessboardPointer[i][j] == ' ') //square from which the engine moved
                {
                    e.fromX = j;
                    e.fromY = i;
                }
                else //square into which the engine moved
                {
                    e.toX = j;
                    e.toY = i;
                }
            }
    }
    if(castle == true)
    {
        e.fromY = e.toY = 7;
        if(color == false)
        {
            e.fromX = 4;
            if(workingChessboardPointer[7][2] == 'K')
                e.toX = 2;
            else
                e.toX = 6;
        }
        if(color == true)
        {
            e.fromX = 3;
            if(workingChessboardPointer[7][1] == 'K')
                e.toX = 1;
            else
                e.toX = 5;
        }
    }
}
        int  Engine::encodeEngineMove()noexcept
{
    if(workingChessboardPointer == nullptr)
        return 0;
    int promotionCode{};
    if(workingChessboardPointer[e.toY][e.toX] != comparativeChessboardPointer[e.fromY][e.fromX])// a pawn was promoted
        switch(workingChessboardPointer[e.toY][e.toX])
        {
            case 'N': promotionCode = 10000; break;
            case 'B': promotionCode = 20000; break;
            case 'R': promotionCode = 30000; break;
            case 'Q': promotionCode = 40000; break;
            default: break;
        }
    return e.fromX*1000+e.fromY*100+e.toX*10+e.toY*1 + promotionCode;
}
        void Engine::markEngineMoveOnChessboard()noexcept
{
    if(workingChessboardPointer == nullptr)
        return;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            comparativeChessboardPointer[i][j] = workingChessboardPointer[i][j];
}
        int  Engine::isItGameOver()noexcept
{
    if(movement.gameOverUserWin)
        return 100000;
    if(movement.gameOverStalemateByUser)
        return 200000;
    if(movement.gameOverEngineWin)
        return 300000;
    if(movement.gameOverStalemateByEngine)
        return 400000;
    return 0;
}