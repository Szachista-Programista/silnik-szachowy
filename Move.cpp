#include "Move.h"
using namespace std;
using namespace globalType;

Move::Move(bool k)noexcept: color{k}{
//==============================================================================================================
    movementGeneration = 0;

    machineKingLocationX = color?3:4;
    machineKingLocationY =         7;
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
void Move::prepareMove()noexcept{
//==============================================================================================================
    potentialMovementMaterialStatus.clear();
    engineMoveNumber = -1 ;
    worstMoveIndex   =  0 ;
    LeastMaterialStatusOfconsideredMovements = 100.0;
}
double Move::findNextMove(chessboardPointer &wsk_X){//7
//==============================================================================================================
    try{
        if(wsk_X == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
        verifyKingsLocation(wsk_X);
        checkIfRooksAndKingsMoved(wsk_X);
        checkIfEngineRookChecked(wsk_X);
        makeEngineMoves(wsk_X);
        return findBestMove(wsk_X);
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Move::verifyKingsLocation(chessboardPointer wsk_X){//0+
//==============================================================================================================
    try{
        if(wsk_X == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
        for(int i=7; i>=0; i--)
            for(int j=0; j<8; j++)
                if(wsk_X[i][j]=='K')
                {
                    machineKingLocationX = j;
                    machineKingLocationY = i;
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
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
    void Move::checkIfRooksAndKingsMoved(chessboardPointer wsk_X){//0+
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
        if(wsk_X[7][0] != 'W') engineLeftRookMoved  = true;
        if(wsk_X[7][7] != 'W') engineRightRookMoved = true;
        if(wsk_X[0][0] != 'w') userLeftRookMoved    = true;
        if(wsk_X[0][7] != 'w') userRightRookMoved   = true;
        if(color? wsk_X[7][3] != 'K': wsk_X[7][4] != 'K') engineKingMoved = true;
        if(color? wsk_X[0][3] != 'k': wsk_X[0][4] != 'k') userKingMoved   = true;
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
    void Move::checkIfEngineRookChecked(chessboardPointer wsk_X){//1
//==============================================================================================================
    try{
        engineKingChecked = checkIfEngineSquareCaptured(machineKingLocationX, machineKingLocationY, wsk_X);
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
    double Move::findBestMove(chessboardPointer &wsk_X){//r
//==============================================================================================================
    try{
        if(movementGeneration < lastMovementGeneration)  return beginningSearchingTreeService(wsk_X);
        if(movementGeneration == lastMovementGeneration) return endingSearchingTreeService();
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
        double Move::beginningSearchingTreeService(chessboardPointer &wsk_X){//r
//==============================================================================================================
    try{
        double bestUserMaterialStatusOfTheWorst = -100;
        int indexOfBestUserMaterialStatusOfTheWorst;
        for(int i=0; i<movements.size() && i<consideredEngineMovementsDepth[movementGeneration]; i++)
        {
            double worsUserMaterialStatus = 100;
            double currentUserMaterialStatus;
            for(int j=1; j<movements[i].size() && j<=consideredUserMovementsDepth[movementGeneration]; j++)
            {
                Move(this, currentUserMaterialStatus, movements[i][j], movementGeneration + 1);
                if(currentUserMaterialStatus < worsUserMaterialStatus)
                    worsUserMaterialStatus = currentUserMaterialStatus;
            }
            if(bestUserMaterialStatusOfTheWorst < worsUserMaterialStatus)
            {
                bestUserMaterialStatusOfTheWorst = worsUserMaterialStatus;
                indexOfBestUserMaterialStatusOfTheWorst = i;
            }
        }
        endingSearchingService(wsk_X, indexOfBestUserMaterialStatusOfTheWorst);
        resetMovements();
        return bestUserMaterialStatusOfTheWorst;
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Move::endingSearchingService(chessboardPointer &wsk_X, int &index){//2
//==============================================================================================================
    try{
        if(movementGeneration == 0)
        {
            checkIfGameFinishedByUser(wsk_X);
            delete[]wsk_X;
            wsk_X=nullptr;
            checkIfGameFinishedByEngine(wsk_X, index);
            if(wsk_X == nullptr && movements.size()!=0)
                wsk_X = copyChessboard(movements[index][0]);
        }
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
                void Move::checkIfGameFinishedByUser(chessboardPointer &wsk_X){//1
//==============================================================================================================
    try{
        if(movements.size()==0)
        {
            if(checkIfEngineSquareCaptured(machineKingLocationX, machineKingLocationY, wsk_X))
                gameOverUserWin = true;
            else
                gameOverStalemateByUser = true;
        }
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
                void Move::checkIfGameFinishedByEngine(chessboardPointer &wsk_X, int &index){//1
//==============================================================================================================
    try{
        for(int i=0, j=0; i<movements.size(); i++)
        {
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
                    break;
                }
                else if(j == movements.size())
                {
                    wsk_X = copyChessboard(movements[i][0]);
                    gameOverStalemateByEngine = true;
                    break;
                }
            }
        }
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
        double Move::endingSearchingTreeService()noexcept{
//==============================================================================================================
    double bestMaterialStatus = -100;
    for(auto element: potentialMovementMaterialStatus)
        if(bestMaterialStatus < element)
            bestMaterialStatus = element;
    resetMovements();
    return bestMaterialStatus;
}
            void Move::resetMovements()noexcept{
//==============================================================================================================
    for(auto oneDimensionalVector: movements)
    {
        for(auto singeMove: oneDimensionalVector)
            delete[]singeMove;
    }
    movements.clear();
}
        Move::Move(Move* pointer, double &materialStatus, chessboardPointer wsk_X, int iteration): movementGeneration {iteration}{//r
//==============================================================================================================
    try{
        rewriteKingsAndRooksMovesData(pointer);
        materialStatus = findNextMove(wsk_X);
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Move::rewriteKingsAndRooksMovesData(Move* wsk){//0+
//==============================================================================================================
    try{
        if (wsk == nullptr)
            throw invalid_argument("Nullptr of Move pointer.");
        engineKingMoved      = wsk->engineKingMoved;
        engineLeftRookMoved  = wsk->engineLeftRookMoved;
        engineRightRookMoved = wsk->engineRightRookMoved;
        userKingMoved        = wsk->userKingMoved;
        userLeftRookMoved    = wsk->userLeftRookMoved;
        userRightRookMoved   = wsk->userRightRookMoved;
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
void Move::makeEngineMoves(chessboardPointer wsk_X){//6
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
        vector<int>iterators = {3, 4, 2, 5, 1, 6, 0, 7};
        for(int i=0; i<8; i++)
            for (int j: iterators)
            {
                switch(wsk_X[i][j])
                {
                    case ' ': break;
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
                                engineMovesService(j,1,j,0,'W',wsk_X);//root
                                engineMovesService(j,1,j,0,'H',wsk_X);//queen
                            }
                            if(j!=0 && 'g'<=wsk_X[0][j-1] && wsk_X[0][j-1]<='w')// promotion by left capture with the creation of a
                            {
                                engineMovesService(j,1,j-1,0,'S',wsk_X);//knight
                                engineMovesService(j,1,j-1,0,'G',wsk_X);//bishop
                                engineMovesService(j,1,j-1,0,'W',wsk_X);//root
                                engineMovesService(j,1,j-1,0,'H',wsk_X);//queen
                            }
                            if(j!=7 && 'g'<=wsk_X[0][j+1] && wsk_X[0][j+1]<='w')// promotion by right capture with the creation of a
                            {
                                engineMovesService(j,1,j+1,0,'S',wsk_X);//knight
                                engineMovesService(j,1,j+1,0,'G',wsk_X);//bishop
                                engineMovesService(j,1,j+1,0,'W',wsk_X);//root
                                engineMovesService(j,1,j+1,0,'H',wsk_X);//queen
                            }
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
                    case 'W':                                                       //root issue
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
                    case 'K':                                                       //king issue
                    {
                        if(0<=i-1 && (wsk_X[i-1][j]<'G' || 'W'<wsk_X[i-1][j]))                    //movement towards 12:00
                        if(2<=abs(machineKingLocationX-userKingLocationX) || 2<=abs(machineKingLocationY-1-userKingLocationY))
                        {
                            machineKingLocationY--;
                            engineMovesService(j,i,j,i-1,'K',wsk_X);
                            machineKingLocationY++;
                        }
                        if(0<=i-1 && j+1<=7 && (wsk_X[i-1][j+1]<'G' || 'W'<wsk_X[i-1][j+1]))      //movement towards 1:30
                        if(2<=abs(machineKingLocationX+1-userKingLocationX) || 2<=abs(machineKingLocationY-1-userKingLocationY))
                        {
                            machineKingLocationX++;
                            machineKingLocationY--;
                            engineMovesService(j,i,j+1,i-1,'K',wsk_X);
                            machineKingLocationX--;
                            machineKingLocationY++;
                        }
                        if(j+1<=7 && (wsk_X[i][j+1]<'G' || 'W'<wsk_X[i][j+1]))                    //movement towards 3:00
                        if(2<=abs(machineKingLocationX+1-userKingLocationX) || 2<=abs(machineKingLocationY-userKingLocationY))
                        {
                            machineKingLocationX++;
                            engineMovesService(j,i,j+1,i,'K',wsk_X);
                            machineKingLocationX--;
                        }
                        if(i+1<=7 && j+1<=7 && (wsk_X[i+1][j+1]<'G' || 'W'<wsk_X[i+1][j+1]))      //movement towards 4:30
                        if(2<=abs(machineKingLocationX+1-userKingLocationX) || 2<=abs(machineKingLocationY+1-userKingLocationY))
                        {
                            machineKingLocationX++;
                            machineKingLocationY++;
                            engineMovesService(j,i,j+1,i+1,'K',wsk_X);
                            machineKingLocationX--;
                            machineKingLocationY--;
                        }
                        if(i+1<=7 && (wsk_X[i+1][j]<'G' || 'W'<wsk_X[i+1][j]))                    //movement towards 6:00
                        if(2<=abs(machineKingLocationX-userKingLocationX) || 2<=abs(machineKingLocationY+1-userKingLocationY))
                        {
                            machineKingLocationY++;
                            engineMovesService(j,i,j,i+1,'K',wsk_X);
                            machineKingLocationY--;
                        }
                        if(i+1<=7 && 0<=j-1 && (wsk_X[i+1][j-1]<'G' || 'W'<wsk_X[i+1][j-1]))      //movement towards 7:30
                        if(2<=abs(machineKingLocationX-1-userKingLocationX) || 2<=abs(machineKingLocationY+1-userKingLocationY))
                        {
                            machineKingLocationX--;
                            machineKingLocationY++;
                            engineMovesService(j,i,j-1,i+1,'K',wsk_X);
                            machineKingLocationX++;
                            machineKingLocationY--;
                        }
                        if(0<=j-1 && (wsk_X[i][j-1]<'G' || 'W'<wsk_X[i][j-1]))                    //movement towards 9:00
                        if(2<=abs(machineKingLocationX-1-userKingLocationX) || 2<=abs(machineKingLocationY-userKingLocationY))
                        {
                            machineKingLocationX--;
                            engineMovesService(j,i,j-1,i,'K',wsk_X);
                            machineKingLocationX++;
                        }
                        if(0<=i-1 && 0<=j-1 && (wsk_X[i-1][j-1]<'G' || 'W'<wsk_X[i-1][j-1]))      //movement towards 10:30
                        if(2<=abs(machineKingLocationX-1-userKingLocationX) || 2<=abs(machineKingLocationY-1-userKingLocationY))
                        {
                            machineKingLocationX--;
                            machineKingLocationY--;
                            engineMovesService(j,i,j-1,i-1,'K',wsk_X);
                            machineKingLocationX++;
                            machineKingLocationY++;
                        }
                        if(!color)                                                                      //castling when the user play black
                        {
                            if(engineKingMoved==false && engineLeftRookMoved==false && wsk_X[7][1]==' ' && wsk_X[7][2]==' ' && wsk_X[7][3]==' ')       //long castle
                            {
                                chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfEngineSquareCaptured(2, 7, cOpy)==false && checkIfEngineSquareCaptured(3, 7, cOpy)==false && checkIfEngineSquareCaptured(4, 7, cOpy)==false)
                                {
                                    cOpy[7][0]=' ';
                                    cOpy[7][4]=' ';
                                    cOpy[7][2]='K';
                                    cOpy[7][3]='W';
                                    machineKingLocationX-=2;
                                    makeEngineMovesIfAllowed(i, j, cOpy);
                                    machineKingLocationX+=2;
                                }
                                else delete[]cOpy;
                            }
                            if(engineKingMoved==false && engineRightRookMoved==false && wsk_X[7][5]==' ' && wsk_X[7][6]==' ')                          //short caste
                            {
                                chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfEngineSquareCaptured(4, 7, cOpy)==false && checkIfEngineSquareCaptured(5, 7, cOpy)==false && checkIfEngineSquareCaptured(6, 7, cOpy)==false)
                                {
                                    cOpy[7][7]=' ';
                                    cOpy[7][4]=' ';
                                    cOpy[7][6]='K';
                                    cOpy[7][5]='W';
                                    machineKingLocationX+=2;
                                    makeEngineMovesIfAllowed(i, j, cOpy);
                                    machineKingLocationX-=2;
                                }
                                else delete[]cOpy;
                            }
                        }
                        if(color)                                                                       //castling when the user play white
                        {
                            if(engineKingMoved==false && engineRightRookMoved==false && wsk_X[7][4]==' ' && wsk_X[7][5]==' ' && wsk_X[7][6]==' ')       //long castle
                            {
                                chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfEngineSquareCaptured(3, 7, cOpy)==false && checkIfEngineSquareCaptured(4, 7, cOpy)==false && checkIfEngineSquareCaptured(5, 7, cOpy)==false)
                                {
                                    cOpy[7][7]=' ';
                                    cOpy[7][3]=' ';
                                    cOpy[7][5]='K';
                                    cOpy[7][4]='W';
                                    machineKingLocationX+=2;
                                    makeEngineMovesIfAllowed(i, j, cOpy);
                                    machineKingLocationX-=2;
                                }
                                else delete[]cOpy;
                            }
                            if(engineKingMoved==false && engineLeftRookMoved==false && wsk_X[7][1]==' ' && wsk_X[7][2]==' ')                          //short caste
                            {
                                chessboardPointer cOpy = copyChessboard(wsk_X);
                                if(checkIfEngineSquareCaptured(1, 7, cOpy)==false && checkIfEngineSquareCaptured(2, 7, cOpy)==false && checkIfEngineSquareCaptured(3, 7, cOpy)==false)
                                {
                                    cOpy[7][0]=' ';
                                    cOpy[7][3]=' ';
                                    cOpy[7][1]='K';
                                    cOpy[7][2]='W';
                                    machineKingLocationX-=2;
                                    makeEngineMovesIfAllowed(i, j, cOpy);
                                    machineKingLocationX+=2;
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
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Move::engineMovesService(int fromX, int fromY, int toX, int toY, char piece, chessboardPointer wsk_X){//5
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
        if(fromX<0 || 7<fromX || fromY<0 || 7<fromY || toX<0 || 7<toX || toY<0 || 7<toY)
            throw invalid_argument("Chessboard coordinates out of range.");
        chessboardPointer cOpy = copyChessboard(wsk_X);
        cOpy[fromY][fromX] = ' ';
        cOpy[toY][toX] = piece;
        makeEngineMovesIfAllowed(fromY, fromX, cOpy);
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Move::makeEngineMovesIfAllowed(int &y, int &x, chessboardPointer cOpy){//4
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
        if(engineKingChecked==false && x!=machineKingLocationX && y!=machineKingLocationY && (machineKingLocationX+machineKingLocationY!=x+y) && (machineKingLocationX-machineKingLocationY!=x-y))
        {//whether the king stands on the square line from which the move took place
            engineMoveNumber++;
            movements.push_back(vector<chessboardPointer>());
            movements[engineMoveNumber].push_back(cOpy);
            makeUserMoves(cOpy);
            discardWorstEngineMove();
        }
        else if(checkIfEngineSquareCaptured(machineKingLocationX, machineKingLocationY, cOpy)==false)
        {//If it's there, isn't it captured after all?
            engineMoveNumber++;
            movements.push_back(vector<chessboardPointer>());
            movements[engineMoveNumber].push_back(cOpy);
            makeUserMoves(cOpy);
            discardWorstEngineMove();
        }
        else delete[]cOpy;
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
            bool Move::checkIfEngineSquareCaptured(const int &x,const int &y, chessboardPointer cOpy){//0+
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
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
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
            void Move::discardWorstEngineMove(){//1
//==============================================================================================================
    try{
        double LeastMaterialStatusOfconsideredMovements = 100.0;
        for(int i=1; i < movements[engineMoveNumber].size(); i++) //finding the most profitable potential user move
        {
            LeastMaterialStatusOfconsideredMovements = min(LeastMaterialStatusOfconsideredMovements, countMaterialStatus(movements[engineMoveNumber][i]));
        }
        if(consideredEngineMovementsDepth[movementGeneration] >= movements.size()) //a situation in which the number of calculated moves does not exceed the limit
        {
            if(LeastMaterialStatusOfconsideredMovements < LeastMaterialStatusOfconsideredMovements)
            {
                LeastMaterialStatusOfconsideredMovements = LeastMaterialStatusOfconsideredMovements;
                worstMoveIndex = engineMoveNumber;
            }
            potentialMovementMaterialStatus.push_back(LeastMaterialStatusOfconsideredMovements);
        }
        else  //a situation in which the number of calculated moves exceeds the limit and the worst move must be rejected
        {
            if(LeastMaterialStatusOfconsideredMovements <= LeastMaterialStatusOfconsideredMovements)//if we discard the current move
            {
                for(auto element: movements[engineMoveNumber])
                    delete[]element;
                movements.pop_back();
                engineMoveNumber--;
            }
            else//if we reject any of the previous moves
            {
                potentialMovementMaterialStatus[worstMoveIndex] = LeastMaterialStatusOfconsideredMovements;
                for(auto element: movements[worstMoveIndex])
                    delete[]element;
                movements[worstMoveIndex] = movements[engineMoveNumber];
                movements.pop_back();
                engineMoveNumber--;
                worstMoveIndex = 0;//looking for from the beginning which move is potentially the worst
                for(int i=0; i<consideredEngineMovementsDepth[movementGeneration]; i++)
                    if(potentialMovementMaterialStatus[i] < potentialMovementMaterialStatus[worstMoveIndex])
                        worstMoveIndex = i;
            }
        }
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
void Move::makeUserMoves(chessboardPointer wsk_X){//3
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
        userKingChecked = checkIfUserSquareCaptured(userKingLocationX, userKingLocationY, wsk_X);
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
            {
                switch(wsk_X[i][j])
                {
                    case ' ': break;
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
                                userMovesService(j,6,j,7,'w',wsk_X); //root
                                userMovesService(j,6,j,7,'h',wsk_X); //queen
                            }
                            if(j!=0 && 'G'<=wsk_X[7][j-1] && wsk_X[7][j-1]<='W')//promotion by left capture with the creation of a
                            {
                                userMovesService(j,6,j-1,7,'s',wsk_X); //knight
                                userMovesService(j,6,j-1,7,'g',wsk_X); //bishop
                                userMovesService(j,6,j-1,7,'w',wsk_X); //root
                                userMovesService(j,6,j-1,7,'h',wsk_X); //queen
                            }
                            if(j!=7 && 'G'<=wsk_X[7][j+1] && wsk_X[7][j+1]<='W')//promotion by right capture with the creation of a
                            {
                                userMovesService(j,6,j+1,7,'s',wsk_X); //knight
                                userMovesService(j,6,j+1,7,'g',wsk_X); //bishop
                                userMovesService(j,6,j+1,7,'w',wsk_X); //root
                                userMovesService(j,6,j+1,7,'h',wsk_X); //queen
                            }
                        }
                        break;
                    }
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
                    case 'w':                                                       //root issue
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
                    case 'k':                                                       //queen issue
                    {
                        if(0<=i-1 && (wsk_X[i-1][j]<'g' || 'w'<wsk_X[i-1][j]))                    //movement towards 12:00
                        if(2<=abs(machineKingLocationX-userKingLocationX) || 2<=abs(machineKingLocationY-1-userKingLocationY))
                        {
                            chessboardPointer cOpy = copyChessboard(wsk_X);
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
                        if(2<=abs(machineKingLocationX+1-userKingLocationX) || 2<=abs(machineKingLocationY-1-userKingLocationY))
                        {
                            chessboardPointer cOpy = copyChessboard(wsk_X);
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
                        if(2<=abs(machineKingLocationX+1-userKingLocationX) || 2<=abs(machineKingLocationY-userKingLocationY))
                        {
                            chessboardPointer cOpy = copyChessboard(wsk_X);
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
                        if(2<=abs(machineKingLocationX+1-userKingLocationX) || 2<=abs(machineKingLocationY+1-userKingLocationY))
                        {
                            chessboardPointer cOpy = copyChessboard(wsk_X);
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
                        if(2<=abs(machineKingLocationX-userKingLocationX) || 2<=abs(machineKingLocationY+1-userKingLocationY))
                        {
                            chessboardPointer cOpy = copyChessboard(wsk_X);
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
                        if(2<=abs(machineKingLocationX-1-userKingLocationX) || 2<=abs(machineKingLocationY+1-userKingLocationY))
                        {
                            chessboardPointer cOpy = copyChessboard(wsk_X);
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
                        if(2<=abs(machineKingLocationX-1-userKingLocationX) || 2<=abs(machineKingLocationY-userKingLocationY))
                        {
                            chessboardPointer cOpy = copyChessboard(wsk_X);
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
                        if(2<=abs(machineKingLocationX-1-userKingLocationX) || 2<=abs(machineKingLocationY-1-userKingLocationY))
                        {
                            chessboardPointer cOpy = copyChessboard(wsk_X);
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
                                chessboardPointer cOpy = copyChessboard(wsk_X);
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
                                chessboardPointer cOpy = copyChessboard(wsk_X);
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
                                chessboardPointer cOpy = copyChessboard(wsk_X);
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
                                chessboardPointer cOpy = copyChessboard(wsk_X);
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
    discardWorstUserMove();
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Move::userMovesService(int fromX, int fromY, int toX, int toY, char bierka, chessboardPointer wsk_X){//2
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
        if(fromX<0 || 7<fromX || fromY<0 || 7<fromY || toX<0 || 7<toX || toY<0 || 7<toY)
            throw invalid_argument("Chessboard coordinates out of range.");
        chessboardPointer cOpy = copyChessboard(wsk_X);
        cOpy[fromY][fromX] = ' ';
        cOpy[toY][toX] = bierka;
        makeUserMovesIfAllowed(fromY, fromX, cOpy);
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Move::makeUserMovesIfAllowed(int &y, int &x, chessboardPointer cOpy){//1
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
        if(userKingChecked==false && x!=userKingLocationX && y!=userKingLocationY && (userKingLocationX+userKingLocationY!=x+y) && (userKingLocationX-userKingLocationY!=x-y))
            movements[engineMoveNumber].push_back(cOpy);
        else if(checkIfUserSquareCaptured(userKingLocationX, userKingLocationY, cOpy)==false)
            movements[engineMoveNumber].push_back(cOpy);
        else delete[]cOpy;
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
            bool Move::checkIfUserSquareCaptured(const int &x, const int &y, chessboardPointer cOpy){//0+
//==============================================================================================================
    try{
        if (x < 0 || 7 < x || y < 0 || 7 < y)
            throw invalid_argument("Chessboard coordinates out of range.");
        if (cOpy == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
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
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
        void Move::discardWorstUserMove(){//1
//==============================================================================================================
    try{
        int userMovementsNumber = movements[engineMoveNumber].size()-1;
        vector<double>materialStatus;
        for(int i=0; i<userMovementsNumber; i++)                                                  //calculating the user's material status
            materialStatus.push_back(countMaterialStatus(movements[engineMoveNumber][i+1]));

        int biggestMaterialStatusMoveIndex;
        while(userMovementsNumber > consideredUserMovementsDepth[movementGeneration])
        {
            biggestMaterialStatusMoveIndex = 0;
            for(int i=1; i<userMovementsNumber; i++)        //looking for move with the highest material status
                if(materialStatus[biggestMaterialStatusMoveIndex] < materialStatus[i])
                    biggestMaterialStatusMoveIndex = i;
            materialStatus.erase(materialStatus.begin() + biggestMaterialStatusMoveIndex);
            delete[]movements[engineMoveNumber][biggestMaterialStatusMoveIndex+1];
            movements[engineMoveNumber].erase(movements[engineMoveNumber].begin() + biggestMaterialStatusMoveIndex + 1);
            userMovementsNumber--;
        }
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
double Move::countMaterialStatus(const chessboardPointer wsk_X){//0+
//==============================================================================================================
    try{
        if (wsk_X == nullptr)
            throw invalid_argument("Nullptr of the chessboard.");
        double materialStatus  = 0.0;
        double pawnValue       = 1.0;
        double sideKnightValue = 2.9;
        double knightValue     = 3.0;
        double sideBishopValue = 2.9;
        double bishopValue     = 3.0;
        double rootValue       = 5.0;
        double queenValue      = 7.0;

        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
            {
                switch(wsk_X[i][j])
                {
                    case 'p': materialStatus += (i==6)? -2*pawnValue : -pawnValue; break;
                    case 's': materialStatus += (i==0 || i==7 || j==0 || j==7)? -sideKnightValue : -knightValue; break;
                    case 'g': materialStatus += (i==0 || i==7 || j==0 || j==7)? -sideBishopValue : -bishopValue; break;
                    case 'w': materialStatus += -rootValue;  break;
                    case 'h': materialStatus += -queenValue; break;

                    case 'P': materialStatus += (i==1)? 2*pawnValue : pawnValue; break;
                    case 'S': materialStatus += (i==0 || i==7 || j==0 || j==7)? sideKnightValue : knightValue; break;
                    case 'G': materialStatus += (i==0 || i==7 || j==0 || j==7)? sideBishopValue : bishopValue; break;
                    case 'W': materialStatus +=  rootValue;  break;
                    case 'H': materialStatus +=  queenValue; break;
                    default: break;
                }
            }
        return materialStatus;
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
chessboardPointer Move::copyChessboard(const chessboardPointer oryginal){//0+
//==============================================================================================================
    try{
        if (oryginal == nullptr)
            throw invalid_argument("Attempting to copy the nullptr address of the chessboard.");
        chessboardPointer cOpy = new char[8][8];
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                cOpy[i][j] = oryginal[i][j];
        return cOpy;
//#########################################################################
    }
    catch(const bad_alloc &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}


























