#include "Notebook.h"

Notebook::Notebook(bool k): color{k}
{
    currentChessboard  = loadPiecesArrangement();
    previousChessboard = loadPiecesArrangement();
    loadChars();
}
    globalType::chessboardPointer Notebook::loadPiecesArrangement()
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
                    if(i==0)
                    {
                        if(j==3)
                            chessboard[i][j] = color?'k':'q';
                        if(j==4)
                            chessboard[i][j] = color?'q':'k';
                    }
                    if(i==7)
                    {
                        if(j==3)
                            chessboard[i][j] = color?'K':'Q';
                        if(j==4)
                            chessboard[i][j] = color?'Q':'K';
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
    void Notebook::loadChars()
{
    std::ifstream reading;
    std::string charArray;
    try
    {
        reading.open("chars.txt");
        if (!reading.is_open())
            throw std::ifstream::failure("The file 'chars.txt' cannot be opened .");

        for(int i=0; i<globalType::numberOfChars; i++)
        {
            if (!getline(reading, charArray))
                throw std::ifstream::failure("Error reading character from 'chars.txt' file .");
            charArray.erase(0, 2);
            for(int j=0, k=0; j<globalType::letterHeight; j++, k++)
                for(; charArray[k] != '$'; k++)
                {
                    if (k >= charArray.size())
                        throw std::ifstream::failure("Unexpected line ending in 'chars.txt' file.");
                    pixelArtCharacterArray[i][j] += charArray[k];
                }
        }
    }
    catch(const std::ifstream::failure &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    reading.close();
}
Notebook::~Notebook()noexcept
{
    delete[]currentChessboard;
    delete[]previousChessboard;
}
void Notebook::generateAndWriteNotation  (int moveCode)
{
    updateParameterValues(moveCode);
    markMoveAndWriteNotation();
    updateEntireNotation();
    writeNotation();
}
    void Notebook::updateParameterValues (int moveCode)
{
    lastMoveNotation.clear();
    Notebook::moveCode = moveCode;
    semiMoveNumber++;
    moveNumber = (semiMoveNumber+1)/2;
    engineMove = color ^ semiMoveNumber%2;
    moveMarked = false;
    decipherMoveCode();
    updateKingsLocation();
}
        void Notebook::decipherMoveCode()
{
    moveToY = moveCode % 10;
    moveCode /=10;
    moveToX = moveCode % 10;
    moveCode /=10;
    moveFromY  = moveCode % 10;
    moveCode /=10;
    moveFromX  = moveCode % 10;
    moveCode /=10;
    promotionParameter = moveCode % 10;
    moveCode /=10;
    gameOverParameter  = moveCode;
    if(gameOverParameter)
        gameOver = true;
    try
    {
        if (moveFromX < 0 || 7 < moveFromX || moveFromY < 0 || 7 < moveFromY || moveToX < 0 || 7 < moveToX || moveToY < 0 || 7 < moveToY)
            throw std::runtime_error("Movement coordinates out of range.");
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
        void Notebook::updateKingsLocation()noexcept
{
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            if(currentChessboard[i][j]=='K')
            {
                machineKingLocationX = j;
                machineKingLocationY = i;
            }
            if(currentChessboard[i][j]=='k')
            {
                userKingLocationX = j;
                userKingLocationY = i;
            }
        }
}
    void Notebook::markMoveAndWriteNotation()
{
    if(gameOverParameter == 1 || gameOverParameter == 2)//the user has finished the game
        return;
    markCastle();
    markPawnPromotion();
    markEnPassant();
    markCommonMove();
    if(gameOverParameter == 3)//the engine caused checkmate
            lastMoveNotation += '#';
    else if(isThereCheck())
        lastMoveNotation += '+';
}
        void Notebook::markCastle()noexcept
{
    if(currentChessboard[moveFromY][moveFromX] == (engineMove?'K':'k') && abs(moveFromX-moveToX)==2)//castle
    {
        moveMarked = true;
        currentChessboard[moveFromY][moveFromX]   = ' ';
        currentChessboard[moveToY][moveToX] = (engineMove?'K':'k');
        if(moveFromX == 3 && moveToX == 1) // O-O
        {
            currentChessboard[moveFromY][0] = ' ';
            currentChessboard[moveFromY][2] = (engineMove?'R':'r');
            lastMoveNotation = "O-O";
        }
        if(moveFromX == 3 && moveToX == 5) // O-O-O
        {
            currentChessboard[moveFromY][7] = ' ';
            currentChessboard[moveFromY][4] = (engineMove?'R':'r');
            lastMoveNotation = "O-O-O";
        }
        if(moveFromX == 4 && moveToX == 2) // O-O-O
        {
            currentChessboard[moveFromY][0] = ' ';
            currentChessboard[moveFromY][3] = (engineMove?'R':'r');
            lastMoveNotation = "O-O-O";
        }
        if(moveFromX == 4 && moveToX == 6) // O-O
        {
            currentChessboard[moveFromY][7] = ' ';
            currentChessboard[moveFromY][5] = (engineMove?'R':'r');
            lastMoveNotation = "O-O";
        }
    }
}
        void Notebook::markPawnPromotion()noexcept
{
    if(promotionParameter) //promotion
    {
        moveMarked = true;
        currentChessboard[moveFromY][moveFromX] = ' ';
        if(currentChessboard[moveToY][moveToX] != ' ')//promotion with capture
        {
            lastMoveNotation += getCoordinates(moveFromY, moveFromY);
            lastMoveNotation.pop_back();
            lastMoveNotation += 'x';
        }
        lastMoveNotation += getCoordinates(moveToY, moveToY);
        lastMoveNotation += '=';
        switch(promotionParameter)
        {
            case 1: lastMoveNotation += globalType::getCommuniqueCotent({38})[0]; currentChessboard[moveToY][moveToX] = (engineMove?'N':'n'); break;
            case 2: lastMoveNotation += globalType::getCommuniqueCotent({39})[0]; currentChessboard[moveToY][moveToX] = (engineMove?'B':'b'); break;
            case 3: lastMoveNotation += globalType::getCommuniqueCotent({40})[0]; currentChessboard[moveToY][moveToX] = (engineMove?'R':'r'); break;
            case 4: lastMoveNotation += globalType::getCommuniqueCotent({41})[0]; currentChessboard[moveToY][moveToX] = (engineMove?'Q':'q'); break;
            default: break;
        }
    }
}
        void Notebook::markEnPassant()noexcept
{
    if(currentChessboard[moveFromY][moveFromX] == (engineMove?'P':'p') && currentChessboard[moveToY][moveToX] == ' ' && abs(moveFromX-moveToX) == 1)
    {
        moveMarked = true;
        lastMoveNotation += getCoordinates(moveFromX, moveFromY);
        lastMoveNotation.pop_back();
        lastMoveNotation += 'x';
        lastMoveNotation += getCoordinates(moveToX, moveToY);
        currentChessboard[moveFromY][moveFromX]   = ' ';
        currentChessboard[moveToY][moveToX] = (engineMove?'P':'p');
        currentChessboard[moveFromY][moveToX]  = ' ';
    }
}
        void Notebook::markCommonMove()
{
    if( ! moveMarked)
    {
        char movedPiece =  currentChessboard[moveFromY][moveFromX];
        bool moveWithCapture  = (currentChessboard[moveToY][moveToX] == ' ')? false: true;
        try
        {
            switch(movedPiece)
            {
                case 'P': case 'p':
                    if(moveWithCapture)
                    {
                        lastMoveNotation += getCoordinates(moveFromX, moveFromY);
                        lastMoveNotation.pop_back();
                    }
                    break;
                case 'N': case 'n':
                {
                    markKnightMove(movedPiece);
                    break;
                }
                case 'B': case 'b':
                    lastMoveNotation += globalType::getCommuniqueCotent({39})[0];
                    break;
                case 'R': case 'r':
                    markRookMove(movedPiece);
                    break;
                case 'Q': case 'q':
                    lastMoveNotation += globalType::getCommuniqueCotent({41})[0];
                    break;
                case 'K': case 'k':
                    lastMoveNotation += globalType::getCommuniqueCotent({42})[0];
                    break;
                default: throw std::runtime_error("Wrong piece.");
            }
        }
        catch(const std::runtime_error &e)
        {
            globalType::errorType x;
            x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
            throw x;
        }
        if(moveWithCapture)
            lastMoveNotation += 'x';
        lastMoveNotation += getCoordinates(moveToX, moveToY);
        currentChessboard[moveToY][moveToX] = movedPiece;
        currentChessboard[moveFromY][moveFromX]   = ' ';
    }
}
            void Notebook::markKnightMove(char movedPiece)
{
    lastMoveNotation += globalType::getCommuniqueCotent({38})[0];

    int secondKnightX,
        secondKnightY;
    bool isThereSecondKnight = false,
            isThereTwinKnight = false;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            if(!(i == moveFromY && j == moveFromX) && currentChessboard[i][j] == movedPiece)
            {
                isThereSecondKnight = true;
                secondKnightX = j;
                secondKnightY = i;
            }
    if(isThereSecondKnight)
    {
        if((abs(moveToX-secondKnightX) == 1 && abs(moveToY-secondKnightY) == 2)
        || (abs(moveToX-secondKnightX) == 2 && abs(moveToY-secondKnightY) == 1))
            isThereTwinKnight = true;

        if(isThereTwinKnight)
        {
            if(moveFromX == secondKnightX)
            {
                lastMoveNotation += getCoordinates(moveFromX, moveFromY);
                lastMoveNotation.erase(1,1);
            }
            else
            {
                lastMoveNotation += getCoordinates(moveFromX, moveFromY);
                lastMoveNotation.pop_back();
            }
        }
    }
}
            void Notebook::markRookMove(char movedPiece)
{
    lastMoveNotation += globalType::getCommuniqueCotent({40})[0];
    if(moveFromY == moveToY)  //horizontal movement
        for(int i=moveToX+((moveFromX<moveToX)? +1: -1); 0<=i && i<=7; (moveFromX<moveToX)? i++: i--)
        {
            if(currentChessboard[moveFromY][i] == ' ')
                continue;
            if(currentChessboard[moveFromY][i] == movedPiece)
            {
                lastMoveNotation += getCoordinates(moveFromX, moveFromY);
                lastMoveNotation.pop_back();
            }
            else
                break;
        }
    if(moveFromX == moveToX)  //vertical movement
        for(int i=moveToY+((moveFromY<moveToY)? +1: -1); 0<=i && i <=7; (moveFromY<moveToY)? i++: i--)
        {
            if(currentChessboard[i][moveFromX] == ' ')
                continue;
            if(currentChessboard[i][moveFromX] == movedPiece)
            {
                lastMoveNotation += getCoordinates(moveFromX, moveFromY);
                lastMoveNotation.erase(1,1);
            }
            else
                break;
        }
}
            std::string Notebook::getCoordinates(int x, int y)
{
    try
    {
        if(x<0 || 7<x || y<0 || 7<y)
            throw std::invalid_argument("wrong coordinates.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    std::string coordinates;
    if(color)
    {
        coordinates += 'a'+abs(x-7);
        coordinates += std::to_string(y+1);
    }
    if(!color)
    {
        coordinates += 'a'+x;
        coordinates += std::to_string(abs(y-8));
    }
    return coordinates;
}
        bool Notebook::isThereCheck()noexcept
{
    int x = ((!engineMove)? machineKingLocationX: userKingLocationX);
    int y = ((!engineMove)? machineKingLocationY: userKingLocationY);

    for(int i=1; 0<=y-i; i++)                                       //if check from 12:00 direction Rook/Queen
    {
        if(currentChessboard[y-i][x]==' ') continue;
        if(currentChessboard[y-i][x]==(engineMove? 'Q': 'q') || currentChessboard[y-i][x]==(engineMove? 'R': 'r')) return true;
        else break;
    }
    for(int i=1; 0<=y-i && x+i<=7; i++)                             //if check from 01:30 direction Bishop/Queen
    {
        if(currentChessboard[y-i][x+i]==' ') continue;
        if(currentChessboard[y-i][x+i]==(engineMove? 'Q': 'q') || currentChessboard[y-i][x+i]==(engineMove? 'B': 'b')) return true;
        else break;
    }
    for(int i=1; x+i<=7; i++)                                       //if check from 03:00 direction Rook/Queen
    {
        if(currentChessboard[y][x+i]==' ') continue;
        if(currentChessboard[y][x+i]==(engineMove? 'Q': 'q') || currentChessboard[y][x+i]==(engineMove? 'R': 'r')) return true;
        else break;
    }
    for(int i=1; y+i<=7 && x+i<=7; i++)                             //if check from 04:30 direction Bishop/Queen
    {
        if(currentChessboard[y+i][x+i]==' ') continue;
        if(currentChessboard[y+i][x+i]==(engineMove? 'Q': 'q') || currentChessboard[y+i][x+i]==(engineMove? 'B': 'b')) return true;
        else break;
    }
    for(int i=1; y+i<=7; i++)                                       //if check from 06:00 direction Rook/Queen
    {
        if(currentChessboard[y+i][x]==' ') continue;
        if(currentChessboard[y+i][x]==(engineMove? 'Q': 'q') || currentChessboard[y+i][x]==(engineMove? 'R': 'r')) return true;
        else break;
    }
    for(int i=1; y+i<=7 && 0<=x-i; i++)                             //if check from 07:30 direction Bishop/Queen
    {
        if(currentChessboard[y+i][x-i]==' ') continue;
        if(currentChessboard[y+i][x-i]==(engineMove? 'Q': 'q') || currentChessboard[y+i][x-i]==(engineMove? 'B': 'b')) return true;
        else break;
    }
    for(int i=1; 0<=x-i; i++)                                       //if check from 09:00 direction Rook/Queen
    {
        if(currentChessboard[y][x-i]==' ') continue;
        if(currentChessboard[y][x-i]==(engineMove? 'Q': 'q') || currentChessboard[y][x-i]==(engineMove? 'R': 'r')) return true;
        else break;
    }
    for(int i=1; 0<=y-i && 0<=x-i; i++)                             //if check from 10:30 direction Bishop/Queen
    {
        if(currentChessboard[y-i][x-i]==' ') continue;
        if(currentChessboard[y-i][x-i]==(engineMove? 'Q': 'q') || currentChessboard[y-i][x-i]==(engineMove? 'B': 'b')) return true;
        else break;
    }

    if(0<=y-2 && x+1<=7 && currentChessboard[y-2][x+1]==(engineMove? 'N': 'n')) return true; //check from Knight
    if(0<=y-1 && x+2<=7 && currentChessboard[y-1][x+2]==(engineMove? 'N': 'n')) return true;
    if(y+1<=7 && x+2<=7 && currentChessboard[y+1][x+2]==(engineMove? 'N': 'n')) return true;
    if(y+2<=7 && x+1<=7 && currentChessboard[y+2][x+1]==(engineMove? 'N': 'n')) return true;
    if(y+2<=7 && 0<=x-1 && currentChessboard[y+2][x-1]==(engineMove? 'N': 'n')) return true;
    if(y+1<=7 && 0<=x-2 && currentChessboard[y+1][x-2]==(engineMove? 'N': 'n')) return true;
    if(0<=y-1 && 0<=x-2 && currentChessboard[y-1][x-2]==(engineMove? 'N': 'n')) return true;
    if(0<=y-2 && 0<=x-1 && currentChessboard[y-2][x-1]==(engineMove? 'N': 'n')) return true;

    if(engineMove && y<=5)//check from Pawn
    {
        if(1<=x && currentChessboard[y+1][x-1] == 'P') return true;
        if(x<=6 && currentChessboard[y+1][x+1] == 'P') return true;
    }
    if(!engineMove && 2<=y)
    {
        if(1<=x && currentChessboard[y-1][x-1] == 'p') return true;
        if(x<=6 && currentChessboard[y-1][x+1] == 'p') return true;
    }
    return false;
}
    void Notebook::updateEntireNotation()
{
    std::ostringstream note;
    if(gameOver)
        endgameService();
    if(semiMoveNumber % 2)
    {
        note<<std::right<<std::setw(3)<<moveNumber<<". "<<std::left<<std::setw(8)<<lastMoveNotation;
        if(gameOverParameter == 3 || gameOverParameter == 4)
            note<<std::left<<std::setw(7)<<gameResult;
        entireNotation.push_back(note.str());
    }
    else
    {
        note<<std::left<<std::setw(7)<<lastMoveNotation;
        entireNotation[moveNumber-1] += note.str();
        if(gameOverParameter == 3 || gameOverParameter == 4)
        {
            note.str("");
            note<<std::right<<std::setw(3)<<moveNumber + 1<<". "<<std::left<<std::setw(8)<<gameResult;
            entireNotation.push_back(note.str());
        }
    }
}
        void Notebook::endgameService()
{
    try
    {
        switch(gameOverParameter)
        {
            case 1:// user win
                replacePlusWithHashtag();
                lastMoveNotation = (semiMoveNumber%2)? "0.1": "1.0";
                break;
            case 2:// stalemate by user
                lastMoveNotation = "1/2-1/2";
                break;
            case 3:// engine win
                gameResult = (semiMoveNumber%2)? "1.0": "0.1";
                break;
            case 4:// stalemate by engine
                gameResult = "1/2-1/2";
                break;
            default: throw std::runtime_error("Wrong gameOverParameter.");
        }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
            void Notebook::replacePlusWithHashtag()
{
    int index_edytowanego_stringa = moveNumber - ((semiMoveNumber % 2)? 2: 1);
    int plusIndex = entireNotation[index_edytowanego_stringa].rfind('+');
    try
    {
        if (plusIndex == std::string::npos)
            throw std::runtime_error("The '+' sign was not found in the edited text fragment.");
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    entireNotation[index_edytowanego_stringa][plusIndex] = '#';
}
    void Notebook::writeNotation()
{
    clearNotationArray();
    if(semiMoveNumber%2)
        writePreviousNotation(previousNotationLine, previousNotationColumn);
    addNotationArrayContent(entireNotation[moveNumber-1]);
    writeNotationArray(currentNotationLine, currentNotationColumn, true);
    if(!(semiMoveNumber%2))
    {
        rewriteNotationToColumn();
        previousNotationColumn = currentNotationColumn;
        previousNotationLine  = currentNotationLine;
        if(currentNotationLine < 15)
            currentNotationLine++;
        else
        {
            currentNotationLine = 0;
            currentNotationColumn++;
            if(currentNotationColumn == 3)
                undoNotationColumns();
        }
        if(gameOverParameter == 3 || gameOverParameter == 4)
        {
            gameOverParameter = 0;
            semiMoveNumber++;
            moveNumber = (semiMoveNumber+1)/2;
            writeNotation();
        }
    }
}
        void Notebook::clearNotationArray()noexcept
{
    for(int i=0; i<globalType::letterHeight; i++)
        notationArray[i].clear();
}
        void Notebook::writePreviousNotation(int line, int column)
{
    try
    {
        if(column<0 || 2<column)
            throw std::invalid_argument("Wrong column.");
        if(line<0 || 16<line)
            throw std::invalid_argument("Wrong line.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(gameOverParameter == 1)
    {
        addNotationArrayContent(entireNotation[moveNumber-2]);
        writeNotationArray(line, column, false);
        clearNotationArray();
        return;
    }
    int x = globalType::chessboardwidth + column*globalType::columnWidth;
    int y = line * globalType::notationLineHeight;
    for(int i=0; i<globalType::letterHeight; i++)
    {
        systemInfo::setCursorPosition(x, y+i);
        std::cout<<notationColumnArray[previousNotationColumn][y+i];
    }
}
        void Notebook::undoNotationColumns()
{
    copyNotationColumnArray(0, 1);
    copyNotationColumnArray(1, 2);
    emptyNotationColumnArray(2);
    writeNotationColumn(0);
    writeNotationColumn(1);
    clearNotationColumn(2);
    previousNotationColumn--;
    currentNotationColumn--;
}
            void Notebook::copyNotationColumnArray(int copyIndex, int patternIndex)
{
    try
    {
        if(copyIndex<0 || 1<copyIndex)
            throw std::invalid_argument("Wrong copyIndex.");
        if(patternIndex<1 || 2<patternIndex)
            throw std::invalid_argument("Wrong patternIndex.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    for(int i=0; i<globalType::columnHeight; i++)
        notationColumnArray[copyIndex][i] = notationColumnArray[patternIndex][i];
}
            void Notebook::emptyNotationColumnArray(int columnNumber)
{
    try
    {
        if(columnNumber<0 || 2<columnNumber)
            throw std::invalid_argument("Wrong column.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    for(int i=0; i<globalType::columnHeight; i++)
        notationColumnArray[columnNumber][i].clear();
}
            void Notebook::writeNotationColumn(int columnNumber)
{
    try
    {
        if(columnNumber<0 || 2<columnNumber)
            throw std::invalid_argument("Wrong column.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    int x = globalType::chessboardwidth + columnNumber*globalType::columnWidth;
    for(int i=0; i<globalType::columnHeight; i++)
    {
        if(columnNumber == 1 && 164 < i)
            systemInfo::setConsoleColor(globalType::notation);
        systemInfo::setCursorPosition(x, i);
        std::cout<<notationColumnArray[columnNumber][i];
    }
    systemInfo::setConsoleColor(globalType::white);
}
            void Notebook::clearNotationColumn(int columnNumber)
{
    try
    {
        if(columnNumber<0 || 2<columnNumber)
            throw std::invalid_argument("Wrong column.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    int x = globalType::chessboardwidth + columnNumber*globalType::columnWidth;
    for(int i=0; i<globalType::columnHeight; i++)
    {
        systemInfo::setCursorPosition(x, i);
        std::cout<<std::string(globalType::columnWidth, ' ');
    }
}
        void Notebook::addNotationArrayContent(std::string content)
{
    for(auto& cHar: content)
    {
        addCharToNotationArray(cHar);
        for(int i=0; i<globalType::letterHeight; i++)
            notationArray[i] += ' ';
    }
}
            void Notebook::addCharToNotationArray(char cHar)
{
    int charIndex = getCharIndex(cHar);
    int charWidth = pixelArtCharacterArray[charIndex][0].size();
    try
    {
        for(int i=0; i<globalType::letterHeight; i++)
            for(int j=0; j<charWidth; j++)
                switch(pixelArtCharacterArray[charIndex][i][j])
                {
                    case ' ': notationArray[i] += ' '   ; break;
                    case 'X': notationArray[i] += '\333'; break;
                    case 'D': notationArray[i] += '\334'; break;
                    case 'G': notationArray[i] += '\337'; break;
                    default : throw std::runtime_error("Error in 'chars.txt' file content.");
                }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
                int Notebook::getCharIndex(char cHar)
{
    if('A'<=cHar && cHar<='Z')
        return cHar-65;
    if('a'<=cHar && cHar<='z')
        return cHar-71;
    if('0'<=cHar && cHar<='9')
        return cHar+4;
    try
    {
        switch(cHar)
        {
            case '.': return 62;
            case '+': return 63;
            case '-': return 64;
            case '=': return 65;
            case '#': return 66;
            case ' ': return 67;
            case '/': return 68;
            case '!': return 69;
            case '?': return 70;
            case '"': return 71;
            case ':': return 72;
            case ';': return 73;
            default: throw std::runtime_error("Wrong char.");
        }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
        void Notebook::writeNotationArray(int line, int column, bool backlight)
{
    try
    {
        if(column<0 || 2<column)
            throw std::invalid_argument("Wrong column.");
        if(line<0 || 16<line)
            throw std::invalid_argument("Wrong line.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    int x = globalType::chessboardwidth + column*globalType::columnWidth;
    int y = line * globalType::notationLineHeight;
    if(backlight)
        systemInfo::setConsoleColor(globalType::notation);
    for(int i=0; i<globalType::letterHeight; i++)
    {
        systemInfo::setCursorPosition(x, y+i);
        std::cout<<notationArray[i];
    }
    if(backlight)
        systemInfo::setConsoleColor(globalType::white);
}
        void Notebook::rewriteNotationToColumn()noexcept
{
    for(int i=0; i<globalType::letterHeight; i++)
        notationColumnArray[currentNotationColumn][currentNotationLine * globalType::notationLineHeight + i] = notationArray[i];
}
std::string Notebook::getChessboardUpdateCode()noexcept
{
    chessboardUpdateCode.clear();
    generateChessboardUpdateCode();
    return chessboardUpdateCode;
}
    void Notebook::generateChessboardUpdateCode()noexcept
{
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            if(currentChessboard[i][j] != previousChessboard[i][j])
            {
                chessboardUpdateCode += std::to_string(j) + std::to_string(i) + currentChessboard[i][j];
                previousChessboard[i][j] = currentChessboard[i][j];
            }
}
std::string Notebook::saveGameInNotebook()
{
    auto now = std::chrono::system_clock::now();
    time_t moment_t = std::chrono::system_clock::to_time_t(now);
    std::ostringstream streamOut;
    streamOut << std::put_time(localtime(&moment_t), " %Y-%m-%d %H;%M;%S");
    std::string fileName = globalType::getCommuniqueCotent({44})[0] + streamOut.str();
    std::ofstream file("saved notations/" + fileName + ".txt");
    try
    {
        if (!file.is_open())
            throw std::ofstream::failure("The file could not be opened for writing.");
    }
    catch(const std::ofstream::failure &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    file<<std::put_time(localtime(&moment_t), ">>> %Y-%m-%d %H:%M:%S <<<")<<std::endl;
    file<<"==========================="<<std::endl;
    for(auto& line: entireNotation)
    file<<line<<std::endl;
    file.close();
    return "\"" + fileName + "\"";
}
