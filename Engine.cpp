#include "Engine.h"

Engine::Engine(bool k): color{k}, movement{k}{//1
//==============================================================================================================
    try{
        globalType::gameStage        = globalType::middlegame;//opening;
        comparativeChessboardPointer = loadPiecesArrangement();
        workingChessboardPointer     = loadPiecesArrangement();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    globalType::chessboardPointer Engine::loadPiecesArrangement(){//0+
//==============================================================================================================
    try{
        std::ifstream reading;
        std::string line;
        globalType::chessboardPointer chessboard = new char[8][8];
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
                    if(i==0)
                    {
                        if(j==3)
                            chessboard[i][j] = color?'k':'h';
                        if(j==4)
                            chessboard[i][j] = color?'h':'k';
                    }
                    if(i==7)
                    {
                        if(j==3)
                            chessboard[i][j] = color?'K':'H';
                        if(j==4)
                            chessboard[i][j] = color?'H':'K';
                    }
                }
                else
                    chessboard[i][j] = line[j];
        }
        reading.close();
        return chessboard;
//#########################################################################
    }
    catch(const std::ifstream::failure &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(const std::bad_alloc &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
Engine::~Engine()noexcept{
//==============================================================================================================
    delete[]comparativeChessboardPointer;
    delete[]workingChessboardPointer;
}
bool Engine::canUserMakeSuchMove(int userMoveCode){//*1
//==============================================================================================================
    try{
        decipherUserMove(userMoveCode);
        if(!isPieceOfUserOnStartingSquare()   )return false;
        if( isPieceOfUserOnFinalSquare()      )return false;
        if( isThisMoveExposesKingToCapture()  )return false;
        if(!ifAllowedMove()                   )return false;
        return true;
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Engine::decipherUserMove(int userMoveCode){//0+
//==============================================================================================================
    try{
        if(userMoveCode == 10000)
            u.toY = u.toX = u.fromY  = u.fromX  = promotionCode = 0;
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
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::invalid_argument("User movement coordinates out of range.");
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    bool Engine::isPieceOfUserOnStartingSquare(){//0+
//==============================================================================================================
    try{
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::invalid_argument("User movement coordinates out of range.");
        if (workingChessboardPointer == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        globalType::chessboardPointer wsk_X = workingChessboardPointer;
        if(wsk_X[u.fromY][u.fromX]<'g' && 'w'<wsk_X[u.fromY][u.fromX])
            return false;
        else
            return true;
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    bool Engine::isPieceOfUserOnFinalSquare(){//0+
//==============================================================================================================
    try{
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::invalid_argument("User movement coordinates out of range.");
        if (workingChessboardPointer == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        globalType::chessboardPointer wsk_X = workingChessboardPointer;
        if(wsk_X[u.toY][u.toX]<'g' || 'w'<wsk_X[u.toY][u.toX])
            return false;
        else
            return true;
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    bool Engine::isThisMoveExposesKingToCapture(){//*0
//==============================================================================================================
    try{
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::invalid_argument("User movement coordinates out of range.");
        if (workingChessboardPointer == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        globalType::chessboardPointer wsk_X = workingChessboardPointer;
        int potentiallyUserKingLocationX;
        int potentiallyUserKingLocationY;

        if(wsk_X[u.fromY][u.fromX]=='k') //is it move of user's king
        {
            potentiallyUserKingLocationX = u.toX;
            potentiallyUserKingLocationY = u.toY;
        }
        else
        {
            potentiallyUserKingLocationX = movement.userKingLocationX;
            potentiallyUserKingLocationY = movement.userKingLocationY;
        }
        globalType::chessboardPointer cOpy = movement.copyChessboard(wsk_X);
        cOpy[u.toY][u.toX]=cOpy[u.fromY][u.fromX];
        cOpy[u.fromY][u.fromX]=' ';
        if(movement.checkIfUserSquareCaptured(potentiallyUserKingLocationX, potentiallyUserKingLocationY, cOpy))
        {
            delete[]cOpy;
            return true;
        }
        else
        {
            delete[]cOpy;
            return false;
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
    bool Engine::ifAllowedMove(){//*0
//==============================================================================================================
    try{
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::invalid_argument("User movement coordinates out of range.");
        if (e.fromX < 0 || 7 < e.fromX || e.fromY < 0 || 7 < e.fromY || e.toX < 0 || 7 < e.toX || e.toY < 0 || 7 < e.toY)
            throw std::invalid_argument("Engine movement coordinates out of range.");
        if (workingChessboardPointer == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
        globalType::chessboardPointer wsk_X = workingChessboardPointer;
        switch(wsk_X[u.fromY][u.fromX])
        {
            case 'p':
                if(u.fromY==1 && u.toY-u.fromY==2 && u.fromX==u.toX && wsk_X[u.fromY+1][u.toX]==' ' && wsk_X[u.toY][u.toX]==' ') return true;//move 2 forward
                if(u.toY-u.fromY==1 && u.fromX==u.toX && wsk_X[u.toY][u.toX]==' ')                                               return true;//move 1 forward
                if(u.toY-u.fromY==1 && u.fromX-u.toX==1 && 'G'<=wsk_X[u.toY][u.toX] && wsk_X[u.toY][u.toX]<='W')                 return true;//capture <<
                if(u.toY-u.fromY==1 && u.toX-u.fromX==1 && 'G'<=wsk_X[u.toY][u.toX] && wsk_X[u.toY][u.toX]<='W')                 return true;//capture >>
                if(wsk_X[e.toY][e.toX] == 'P' && e.fromY-e.toY == 2 && e.toX == u.toX && u.fromY == e.toY)
                {
                    if(u.toY-u.fromY==1 && u.fromX-u.toX==1 && ' ' == wsk_X[u.toY][u.toX]) return true;//En passant <<
                    if(u.toY-u.fromY==1 && u.toX-u.fromX==1 && ' ' == wsk_X[u.toY][u.toX]) return true;//En passant >>
                }
                return false;
            case 's':
                if((abs(u.fromX-u.toX)==2 && abs(u.fromY-u.toY)==1) || (abs(u.fromX-u.toX)==1 && abs(u.fromY-u.toY)==2))
                    return true;
                return false;
            case 'g':
            case 'h':
                if(u.fromX-u.fromY==u.toX-u.toY) // slant movements (\)
                {
                    if(u.fromX<u.toX) //movement towards 04:30
                        for(int x=u.fromX+1, y=u.fromY+1; x<u.toX && y<u.toY; x++, y++)
                            if(wsk_X[y][x]!=' ')
                                return false;
                    if(u.fromX>u.toX) //movement towards 10:30
                        for(int x=u.fromX-1, y=u.fromY-1; x>u.toX && y>u.toY; x--, y--)
                            if(wsk_X[y][x]!=' ')
                                return false;
                    return true;
                }
                if(u.fromX+u.fromY==u.toX+u.toY) // slant movements (/)
                {
                    if(u.fromX<u.toX) //movement towards 01:30
                        for(int x=u.fromX+1, y=u.fromY-1; x<u.toX && y>u.toY; x++, y--)
                            if(wsk_X[y][x]!=' ')
                                return false;
                    if(u.fromX>u.toX) //movement towards 07:30
                        for(int x=u.fromX-1, y=u.fromY+1; x>u.toX && y<u.toY; x--, y++)
                            if(wsk_X[y][x]!=' ')
                                return false;
                    return true;
                }
            case 'w':
                if(u.fromY == u.toY) //horizontal movements
                {
                    if(u.fromX>u.toX) //to left
                        for(int x=u.fromX-1; x>u.toX; x--)
                            if(wsk_X[u.fromY][x]!=' ')
                                return false;
                    if(u.fromX<u.toX) //to right
                        for(int x=u.fromX+1; x<u.toX; x++)
                            if(wsk_X[u.fromY][x]!=' ')
                                return false;
                    return true;
                }
                if(u.fromX == u.toX) //vertical movements
                {
                    if(u.fromY>u.toY)//up
                        for(int y=u.fromY-1; y>u.toY; y--)
                            if(wsk_X[y][u.fromX]!=' ')
                                return false;
                    if(u.fromY<u.toY)//down
                        for(int y=u.fromY+1; y<u.toY; y++)
                            if(wsk_X[y][u.fromX]!=' ')
                                return false;
                    return true;
                }
                return false;
            case 'k':
                if(abs(u.fromX-u.toX)<=1 && abs(u.fromY-u.toY)<=1)//ordinary king's movement
                if(0<=u.toY-1                ? wsk_X[u.toY-1][u.toX  ] != 'K': true)
                if(0<=u.toY-1 && u.toX+1<=7  ? wsk_X[u.toY-1][u.toX+1] != 'K': true)
                if(              u.toX+1<=7  ? wsk_X[u.toY  ][u.toX+1] != 'K': true)
                if(u.toY+1<=7 && u.toX+1<=7  ? wsk_X[u.toY+1][u.toX+1] != 'K': true)
                if(u.toY+1<=7                ? wsk_X[u.toY+1][u.toX  ] != 'K': true)
                if(u.toY+1<=7 && 0<=u.toX-1  ? wsk_X[u.toY+1][u.toX-1] != 'K': true)
                if(              0<=u.toX-1  ? wsk_X[u.toY  ][u.toX-1] != 'K': true)
                if(u.toY+1<=7 && 0<=u.toX-1  ? wsk_X[u.toY+1][u.toX-1] != 'K': true)
                    return true;
                if(u.fromY==u.toY && abs(u.fromX - u.toX) == 2 && movement.userKingMoved==false)//castle
                {
                    if(color==0)//when user play black
                    {
                        if(u.toX==2 && movement.userLeftRookMoved ==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ' && wsk_X[0][3]==' ')//long castle
                        if(!movement.checkIfUserSquareCaptured(2, 0, wsk_X) && !movement.checkIfUserSquareCaptured(3, 0, wsk_X) && !movement.checkIfUserSquareCaptured(4, 0, wsk_X))
                            return true;
                        if(u.toX==6 && movement.userRightRookMoved==false && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')//short castle
                        if(!movement.checkIfUserSquareCaptured(4, 0, wsk_X) && !movement.checkIfUserSquareCaptured(5, 0, wsk_X) && !movement.checkIfUserSquareCaptured(6, 0, wsk_X))
                            return true;
                    }
                    if(color==1)//when user play white
                    {
                        if(u.toX==5 && movement.userRightRookMoved==false && wsk_X[0][4]==' ' && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')//long castle
                        if(!movement.checkIfUserSquareCaptured(3, 0, wsk_X) && !movement.checkIfUserSquareCaptured(4, 0, wsk_X) && !movement.checkIfUserSquareCaptured(5, 0, wsk_X))
                            return true;
                        if(u.toX==1 && movement.userLeftRookMoved ==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ')//short castle
                        if(!movement.checkIfUserSquareCaptured(1, 0, wsk_X) && !movement.checkIfUserSquareCaptured(2, 0, wsk_X) && !movement.checkIfUserSquareCaptured(3, 0, wsk_X))
                            return true;
                    }
                }
            return false;
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
int Engine::makeMove(int userMoveCode){//2
//==============================================================================================================
    try{
        getEngineReadyForMove(userMoveCode);
        switch(globalType::gameStage)
        {
            case globalType::opening:     makeOpeningMove();    break;
            case globalType::middlegame:  makeMiddlegameMove(); break;
            case globalType::endgame:     makeEndgameMove();    break;
        }
        return engineMoveCoding();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Engine::getEngineReadyForMove(int userMoveCode){//1
//==============================================================================================================
    try{
        movementNumber++;
        decipherUserMove(userMoveCode); // changes e.g. 4456 into u.fromY u.toY u.fromX u.toX
        markUserMoveOnChessboard(userMoveCode); // marks u.fromY u.toY u.fromX u.toX into comparativeChessboardPointer and workingChessboardPointer
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Engine::markUserMoveOnChessboard(int userMoveCode){//0+
//==============================================================================================================
    try{
        if (u.fromX < 0 || 7 < u.fromX || u.fromY < 0 || 7 < u.fromY || u.toX < 0 || 7 < u.toX || u.toY < 0 || 7 < u.toY)
            throw std::invalid_argument("User movement coordinates out of range.");
        if (comparativeChessboardPointer == nullptr || workingChessboardPointer == nullptr)
            throw std::invalid_argument("Nullptr of the chessboard.");
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
                case 1: comparativeChessboardPointer[u.toY][u.toX] = workingChessboardPointer[u.toY][u.toX] = 's'; break;
                case 2: comparativeChessboardPointer[u.toY][u.toX] = workingChessboardPointer[u.toY][u.toX] = 'g'; break;
                case 3: comparativeChessboardPointer[u.toY][u.toX] = workingChessboardPointer[u.toY][u.toX] = 'w'; break;
                case 4: comparativeChessboardPointer[u.toY][u.toX] = workingChessboardPointer[u.toY][u.toX] = 'h'; break;
                default: break;
            }

        if(movedPiece == 'k')//maby it comes to castles
        {
            if(u.fromX == 3 && u.toX == 1) // O-O of wfite
            {
                comparativeChessboardPointer[0][0] = workingChessboardPointer[0][0] = ' ';
                comparativeChessboardPointer[0][2] = workingChessboardPointer[0][2] = 'w';
            }
            if(u.fromX == 3 && u.toX == 5) // O-O-O of wfite
            {
                comparativeChessboardPointer[0][7] = workingChessboardPointer[0][7] = ' ';
                comparativeChessboardPointer[0][4] = workingChessboardPointer[0][4] = 'w';
            }
            if(u.fromX == 4 && u.toX == 2) // O-O-O of black
            {
                comparativeChessboardPointer[0][0] = workingChessboardPointer[0][0] = ' ';
                comparativeChessboardPointer[0][3] = workingChessboardPointer[0][3] = 'w';
            }
            if(u.fromX == 4 && u.toX == 6) // O-O of black
            {
                comparativeChessboardPointer[0][7] = workingChessboardPointer[0][7] = ' ';
                comparativeChessboardPointer[0][5] = workingChessboardPointer[0][5] = 'w';
            }
        }
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    void Engine::makeOpeningMove()noexcept{
//==============================================================================================================
    if(color)
        blackOpeningMove();
    else
        whiteOpeningMove();
    if(movementNumber == 3)
        globalType::gameStage = globalType::middlegame;
}
        void Engine::blackOpeningMove()noexcept{
//==============================================================================================================
    userPiecesMovedInOpening += workingChessboardPointer[u.toY][u.toX];
    switch(movementNumber)
    {
        case 1:
            if(((u.toX==2 || u.toX==4) && u.toY==3) || (userPiecesMovedInOpening[0]=='s' && u.toX==2)){
                if(randomChance(3)){
                    workingChessboardPointer[6][4]=' ';
                    workingChessboardPointer[4][4]='P';
                }
                else{
                    workingChessboardPointer[6][2]=' ';
                    workingChessboardPointer[4][2]='P';
                }
            }
            else{
                workingChessboardPointer[6][3]=' ';
                workingChessboardPointer[4][3]='P';
            }
            break;
        case 2:
            if(userPiecesMovedInOpening[1]=='h' || 4<=u.toY){
                globalType::gameStage = globalType::middlegame;
                makeMiddlegameMove();
                break;
            }
            else if(e.toX==4){
                if(randomChance(3)){
                    workingChessboardPointer[7][1]=' ';
                    workingChessboardPointer[5][2]='S';
                }
                else{
                    workingChessboardPointer[6][3]=' ';
                    workingChessboardPointer[5][3]='P';
                }
            }
            else if(e.toX==2){
                if(randomChance(3)){
                    workingChessboardPointer[7][6]=' ';
                    workingChessboardPointer[5][5]='S';
                }
                else if(randomChance(2)){
                    workingChessboardPointer[6][3]=' ';
                    workingChessboardPointer[5][3]='P';
                }
                else{
                    workingChessboardPointer[6][1]=' ';
                    workingChessboardPointer[5][1]='P';
                }
            }
            else{
                workingChessboardPointer[7][6]=' ';
                workingChessboardPointer[5][5]='S';
            }
            break;
        case 3:
            if(userPiecesMovedInOpening[1]=='h' || 4<=u.toY){
                globalType::gameStage = globalType::middlegame;
                makeMiddlegameMove();
                break;
            }
            else if(workingChessboardPointer[7][6]=='S'){
                workingChessboardPointer[7][6]=' ';
                workingChessboardPointer[5][5]='S';
            }
            else{
                workingChessboardPointer[7][1]=' ';
                workingChessboardPointer[5][2]='S';
            }
            break;
    }
}
        void Engine::whiteOpeningMove()noexcept{
//==============================================================================================================
    if(1 < movementNumber)
        userPiecesMovedInOpening += workingChessboardPointer[u.toY][u.toX];
    switch(movementNumber)
    {
        case 1:
            if(randomChance(3)){
                    workingChessboardPointer[6][4]=' ';
                    workingChessboardPointer[4][4]='P';
            }
            else if(randomChance(3)){
                    workingChessboardPointer[6][3]=' ';
                    workingChessboardPointer[4][3]='P';
            }
            else if(randomChance(2)){
                    workingChessboardPointer[7][6]=' ';
                    workingChessboardPointer[5][5]='S';
            }
            else{
                    workingChessboardPointer[7][1]=' ';
                    workingChessboardPointer[5][2]='S';
            }
            break;
        case 2:
            if(workingChessboardPointer[4][4]=='P'){
                if(randomChance(3))
                {
                    workingChessboardPointer[7][1]=' ';
                    workingChessboardPointer[5][2]='S';
                }
                else
                {
                    workingChessboardPointer[7][6]=' ';
                    workingChessboardPointer[5][5]='S';
                }
            }
            else if(workingChessboardPointer[4][3]=='p')
            {
                if(randomChance(3)){
                    workingChessboardPointer[7][6]=' ';
                    workingChessboardPointer[5][5]='S';
                }
                else{
                    workingChessboardPointer[7][1]=' ';
                    workingChessboardPointer[5][2]='S';
                }
            }
            else if(workingChessboardPointer[5][2]=='S'){
                    workingChessboardPointer[7][6]=' ';
                    workingChessboardPointer[5][5]='S';
            }
            else{
                    workingChessboardPointer[7][1]=' ';
                    workingChessboardPointer[5][2]='S';
            }
            break;
        case 3:
            if(userPiecesMovedInOpening[1]=='h' || 4<=u.toY){
                globalType::gameStage = globalType::middlegame;
                makeMiddlegameMove();
                break;
            }
            else if(workingChessboardPointer[6][4]=='p'){
                    workingChessboardPointer[6][4]=' ';
                    workingChessboardPointer[4][4]='P';
            }
            else{
                    workingChessboardPointer[6][3]=' ';
                    workingChessboardPointer[4][3]='P';
            }
            break;
    }

}
        int Engine::randomChance(int optionsNumber)noexcept{
//==============================================================================================================
    srand(static_cast<unsigned int>(time(nullptr)));
    return rand() % optionsNumber;
}
    void Engine::makeMiddlegameMove(){//*0
//==============================================================================================================
    try{
        //Chessboard y(color);//#####
        //Notice z;//#####

        /*#####/double x = /#####*/
        movement.findNextMove(workingChessboardPointer);
        //z.communique(std::to_string(x), 1500);//#####
        //y.deleteCommunique(100);//#####
        if( ! movement.gameOver)
            isItEndgameTime();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Engine::isItEndgameTime()noexcept{
//==============================================================================================================
    bool isItEndgame = true;
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if('g' <= workingChessboardPointer[i][j] && workingChessboardPointer[i][j] <= 'w' && workingChessboardPointer[i][j] != 'k')
                {
                    isItEndgame = false;
                    break;
                }
            }
            if(isItEndgame == false)
                break;
        }
    if(isItEndgame == true)
    {
        setUserKingSideLocation();
        setUserKingCornerLocation();
        globalType::gameStage = globalType::endgame;
        setKindOfEndgame();
    }
}
            void Engine::setKindOfEndgame()noexcept{
//==============================================================================================================
    int nrOfPawns   = 0;
    int nrOfKnights = 0;
    int nrOfBishops = 0;
    int nrOfRooks   = 0;
    int nrOfQueens  = 0;

    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            switch(workingChessboardPointer[i][j])
            {
                case 'P': nrOfPawns++;   break;
                case 'S': nrOfKnights++; break;
                case 'G': nrOfBishops++; break;
                case 'W': nrOfRooks++;   break;
                case 'H': nrOfQueens++;  break;
            }
    if(nrOfRooks >= 2){
        globalType::choosenEndgame = globalType::twoRooksMate;
        return;
    }
    else if(nrOfQueens >= 2){
        globalType::choosenEndgame = globalType::twoQueensMate;
        return;
    }
    else if(nrOfRooks == 1 && nrOfQueens == 1){
        globalType::choosenEndgame = globalType::queenAndRookMate;
        return;
    }
    else if(nrOfRooks == 1){
        globalType::choosenEndgame = globalType::rookMate;
        return;
    }
    else if(nrOfQueens == 1){
        globalType::choosenEndgame = globalType::queenMate;
        return;
    }
   // else

}
            void Engine::setUserKingSideLocation(){
//==============================================================================================================
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
            void Engine::setUserKingCornerLocation(){
//==============================================================================================================
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
    void Engine::makeEndgameMove(){//????
//==============================================================================================================
    switch(globalType::choosenEndgame)
    {
        case globalType::twoRooksMate:     makeTwoRooksMateMove();     break;
        case globalType::twoQueensMate:    makeTwoQueensMateMove();    break;
        case globalType::queenAndRookMate: makeQueenAndRookMateMove(); break;
        case globalType::rookMate:         makeRookMateMove();         break;
        case globalType::queenMate:        makeQueenMateMove();        break;
    }
}
        void Engine::makeTwoRooksMateMove(){
//==============================================================================================================
    double A = 0.1, B = 0.04, C = 0.005, D = 0.001, zero = 0.0;

    globalType::userKingBehaviorPriority1    = &A;
    //globalType::userKingBehaviorPriority2    = &zero;
    globalType::engineKnightBehaviorPriority = &C;
    globalType::engineBishopBehaviorPriority = &C;
    globalType::engineRookBehaviorPriority1  = &B;
    globalType::engineRookBehaviorPriority2  = &D;
    globalType::engineQueenBehaviorPriority1 = &C;
    globalType::engineQueenBehaviorPriority2 = &zero;
    globalType::engineKingBehaviorPriority   = &C;

    globalType::userKingBehaviorPoints1    = &movement.goToSideOfUserKing;
    //globalType::userKingBehaviorPoints2    = &movement.makeNothing;
    globalType::engineKnightBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineBishopBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineRookBehaviorPoints1  = &movement.separateUserKingFromRestOfBoard;
    globalType::engineRookBehaviorPoints2  = &movement.runSidewaysFromKing;
    globalType::engineQueenBehaviorPoints1 = &movement.goOppositeDirectionOfUserKing;
    globalType::engineQueenBehaviorPoints2 = &movement.makeNothing;
    globalType::engineKingBehaviorPoints   = &movement.goOppositeDirectionOfUserKing;

    movement.findNextMove(workingChessboardPointer);
}
        void Engine::makeTwoQueensMateMove(){
//==============================================================================================================
    double A = 0.1, B = 0.04, C = 0.005, D = 0.001, zero = 0.0;

    globalType::userKingBehaviorPriority1    = &A;
    //globalType::userKingBehaviorPriority2    = &zero;
    globalType::engineKnightBehaviorPriority = &C;
    globalType::engineBishopBehaviorPriority = &C;
    globalType::engineRookBehaviorPriority1  = &C;
    globalType::engineRookBehaviorPriority2  = &zero;
    globalType::engineQueenBehaviorPriority1 = &B;
    globalType::engineQueenBehaviorPriority2 = &D;
    globalType::engineKingBehaviorPriority   = &C;

    globalType::userKingBehaviorPoints1    = &movement.goToSideOfUserKing;
    //globalType::userKingBehaviorPoints2    = &movement.makeNothing;
    globalType::engineKnightBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineBishopBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineRookBehaviorPoints1  = &movement.goOppositeDirectionOfUserKing;
    globalType::engineRookBehaviorPoints2  = &movement.makeNothing;
    globalType::engineQueenBehaviorPoints1 = &movement.separateUserKingFromRestOfBoard;
    globalType::engineQueenBehaviorPoints2 = &movement.runSidewaysFromKing;
    globalType::engineKingBehaviorPoints   = &movement.goOppositeDirectionOfUserKing;

    movement.findNextMove(workingChessboardPointer);
}
        void Engine::makeQueenAndRookMateMove(){
//==============================================================================================================
    double A = 0.1, B = 0.04, C = 0.005, D = 0.001, zero = 0.0;

    globalType::userKingBehaviorPriority1    = &A;
    //globalType::userKingBehaviorPriority2    = &zero;
    globalType::engineKnightBehaviorPriority = &C;
    globalType::engineBishopBehaviorPriority = &C;
    globalType::engineRookBehaviorPriority1  = &B;
    globalType::engineRookBehaviorPriority2  = &D;
    globalType::engineQueenBehaviorPriority1 = &B;
    globalType::engineQueenBehaviorPriority2 = &D;
    globalType::engineKingBehaviorPriority   = &C;

    globalType::userKingBehaviorPoints1    = &movement.goToSideOfUserKing;
    //globalType::userKingBehaviorPoints2    = &movement.makeNothing;
    globalType::engineKnightBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineBishopBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineRookBehaviorPoints1  = &movement.separateUserKingFromRestOfBoard;
    globalType::engineRookBehaviorPoints2  = &movement.runSidewaysFromKing;
    globalType::engineQueenBehaviorPoints1 = &movement.separateUserKingFromRestOfBoard;
    globalType::engineQueenBehaviorPoints2 = &movement.runSidewaysFromKing;
    globalType::engineKingBehaviorPoints   = &movement.goOppositeDirectionOfUserKing;

    movement.findNextMove(workingChessboardPointer);
}
        void Engine::makeRookMateMove(){
//==============================================================================================================
    double dot1 = 0.1, dot08 = 0.08, dot04 = 0.04, dot005 = 0.005, zero = 0.0;

    globalType::userKingBehaviorPriority1    = &dot1;
    //globalType::userKingBehaviorPriority2    = &zero;
    globalType::engineKnightBehaviorPriority = &dot005;
    globalType::engineBishopBehaviorPriority = &dot005;
    globalType::engineRookBehaviorPriority1  = &dot08;
    globalType::engineRookBehaviorPriority2  = &zero;
    globalType::engineQueenBehaviorPriority1 = &dot005;
    globalType::engineQueenBehaviorPriority2 = &zero;
    globalType::engineKingBehaviorPriority   = &dot04;

    globalType::userKingBehaviorPoints1    = &movement.goToCornerOfUserKing;
    //globalType::userKingBehaviorPoints2    = &movement.makeNothing;
    globalType::engineKnightBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineBishopBehaviorPoints = &movement.goOppositeDirectionOfUserKing;
    globalType::engineRookBehaviorPoints1  = &movement.followUserKingToCorner;
    globalType::engineRookBehaviorPoints2  = &movement.makeNothing;
    globalType::engineQueenBehaviorPoints1 = &movement.goOppositeDirectionOfUserKing;
    globalType::engineQueenBehaviorPoints2 = &movement.makeNothing;
    globalType::engineKingBehaviorPoints   = &movement.followUserKingToSide;

        Chessboard y(color);//#####
        Notice z;//#####
        /*#####*/double x = /*#####*/movement.findNextMove(workingChessboardPointer);
        z.communique(std::to_string(x), 500);//#####
        y.deleteCommunique(100);//#####

}
        void Engine::makeQueenMateMove(){
//==============================================================================================================
}
int  Engine::engineMoveCoding()noexcept{
//==============================================================================================================
    findEngineMove(); //  finds e.fromY e.toY e.fromX e.toX by comparing workingChessboardPointer and comparativeChessboardPointer
    int engineMoveCode = encodeEngineMove(); // changes e.fromY e.toY e.fromX e.toX into e.g. 1526
    markEngineMoveOnChessboard(); //          comparativeChessboardPointer = workingChessboardPointer
    return engineMoveCode + isItGameOver();
}
        void Engine::findEngineMove()noexcept{
//==============================================================================================================
    if(workingChessboardPointer == nullptr)
        return;
    bool czy_to_roszada = false;
    for(int i=0,x=0; i<8; i++) //checking whether castling has taken place
    {
        if(workingChessboardPointer[7][i] != comparativeChessboardPointer[7][i])
            x++;
        if(i==7 && x==4)
        czy_to_roszada = true;
    }
    if(czy_to_roszada == false)
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
    if(czy_to_roszada == true)
    {
        e.fromY = e.toY = 7;
        if(color == 0)
        {
            e.fromX = 4;
            if(workingChessboardPointer[7][2] == 'K')
                e.toX = 2;
            else
                e.toX = 6;
        }
        if(color == 1)
        {
            e.fromX = 3;
            if(workingChessboardPointer[7][1] == 'K')
                e.toX = 1;
            else
                e.toX = 5;
        }
    }
}
        int  Engine::encodeEngineMove()noexcept{
//==============================================================================================================
    if(workingChessboardPointer == nullptr)
        return 0;
    int promotionCode = 0;
    if(workingChessboardPointer[e.toY][e.toX] != comparativeChessboardPointer[e.fromY][e.fromX])// a pawn was promoted
        switch(workingChessboardPointer[e.toY][e.toX])
        {
            case 'S': promotionCode = 10000; break;
            case 'G': promotionCode = 20000; break;
            case 'W': promotionCode = 30000; break;
            case 'H': promotionCode = 40000; break;
            default: break;
        }
    return e.fromX*1000+e.fromY*100+e.toX*10+e.toY*1 + promotionCode;
}
        void Engine::markEngineMoveOnChessboard()noexcept{
//==============================================================================================================
    if(workingChessboardPointer == nullptr)
        return;
    char movedPiece;
    movedPiece = workingChessboardPointer[e.fromY][e.fromX];

    comparativeChessboardPointer[e.toY][e.toX] = workingChessboardPointer[e.toY][e.toX];
    comparativeChessboardPointer[e.fromY][e.fromX]   = ' ';

    if(movedPiece == 'K')//if it comes to castle
    {
        if(e.fromX == 3 && e.toX == 1) // O-O by black
        {
            comparativeChessboardPointer[7][0] == ' ';
            comparativeChessboardPointer[7][2] == 'W';
        }
        if(e.fromX == 3 && e.toX == 5) // O-O-O by black
        {
            comparativeChessboardPointer[7][7] == ' ';
            comparativeChessboardPointer[7][4] == 'W';
        }
        if(e.fromX == 4 && e.toX == 2) // O-O-O by white
        {
            comparativeChessboardPointer[7][0] == ' ';
            comparativeChessboardPointer[7][3] == 'W';
        }
        if(e.fromX == 4 && e.toX == 6) // O-O by white
        {
            comparativeChessboardPointer[7][7] == ' ';
            comparativeChessboardPointer[7][5] == 'W';
        }
    }
}
        int  Engine::isItGameOver()noexcept{
//==============================================================================================================
    if(movement.gameOverUserWin)
        return 60000;
    if(movement.gameOverStalemateByUser)
        return 70000;
    if(movement.gameOverEngineWin)
        return 80000;
    if(movement.gameOverStalemateByEngine)
        return 90000;
    return 0;
}























