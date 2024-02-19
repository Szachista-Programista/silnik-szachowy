#include "Notebook.h"

Notebook::Notebook(bool k): color{k}{//1
//==============================================================================================================
    try{
        currentChessboard  = loadPiecesArrangement();
        previousChessboard = loadPiecesArrangement();
        loadChars();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    globalType::chessboardPointer Notebook::loadPiecesArrangement(){//0+
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
    void Notebook::loadChars(){//0+
//==============================================================================================================
    try{
        std::ifstream reading;
        std::string charArray;
        reading.open("chars.txt");
        if (!reading.is_open())
            throw std::ifstream::failure("The file 'chars.txt' cannot be opened .");

        for(int i=0; i<globalType::numberOfChars; i++)
        {
            if (!getline(reading, charArray))
                throw std::ifstream::failure("Error reading character from 'chars.txt' file .");
            charArray.erase(0, 2);
            for(int j=0, k=0; j<globalType::letterHeight; j++, k++)
            {
                for(; charArray[k] != '$'; k++)
                {
                    if (k >= charArray.size())
                        throw std::ifstream::failure("Unexpected line ending in 'chars.txt' file.");
                    pixelArtCharacterArray[i][j] += charArray[k];
                }
            }
        }
        reading.close();
//#########################################################################
    }
    catch(const std::ifstream::failure &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
Notebook::~Notebook()noexcept{
//==============================================================================================================
    delete[]currentChessboard;
    delete[]previousChessboard;
}
void Notebook::generateAndWriteNotation(int moveCode){//*5
//==============================================================================================================
    try{
        updateParameterValues(moveCode);
        markMoveAndWriteNotation();
        updateEntireNotation();
        writeNotation();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Notebook::updateParameterValues(int moveCode){//1
//==============================================================================================================
    try{
        lastMoveNotation.clear();
        Notebook::moveCode = moveCode;
        semiMoveNumber++;
        moveNumber = (semiMoveNumber+1)/2;
        engineMove = color ^ semiMoveNumber%2;
        moveMarked = false;
        decipherMoveCode();
        updateKingsLocation();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Notebook::decipherMoveCode(){//0+
//==============================================================================================================
    try{
        moveToY = moveCode % 10;
        moveCode /=10;
        moveToX = moveCode % 10;
        moveCode /=10;
        moveFromY  = moveCode % 10;
        moveCode /=10;
        moveFromX  = moveCode % 10;
        moveCode /=10;
        additionalParameter = moveCode;
        if(6 <= additionalParameter && additionalParameter <= 9)
            gameOver = true;
        if (moveFromX < 0 || 7 < moveFromX || moveFromY < 0 || 7 < moveFromY || moveToX < 0 || 7 < moveToX || moveToY < 0 || 7 < moveToY)
            throw std::invalid_argument("Movement coordinates out of range.");
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
        void Notebook::updateKingsLocation()noexcept{
//==============================================================================================================
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
    void Notebook::markMoveAndWriteNotation(){//2
//==============================================================================================================
    try{
        if(additionalParameter == 6 || additionalParameter == 7)//the user has finished the game
            return;
        markCastle();
        markPawnPromotion();
        markEnPassant();
        markCommonMove();
        if(additionalParameter == 8)//the engine caused checkmate
             lastMoveNotation += '#';
        else if(isThereCheck())
            lastMoveNotation += '+';
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Notebook::markCastle()noexcept{
//==============================================================================================================
    if(currentChessboard[moveFromY][moveFromX] == (engineMove?'K':'k') && abs(moveFromX-moveToX)==2)//castle
    {
        moveMarked = true;
        currentChessboard[moveFromY][moveFromX]   = ' ';
        currentChessboard[moveToY][moveToX] = (engineMove?'K':'k');
        if(moveFromX == 3 && moveToX == 1) // O-O
        {
            currentChessboard[moveFromY][0] = ' ';
            currentChessboard[moveFromY][2] = (engineMove?'W':'w');
            lastMoveNotation = "O-O";
        }
        if(moveFromX == 3 && moveToX == 5) // O-O-O
        {
            currentChessboard[moveFromY][7] = ' ';
            currentChessboard[moveFromY][4] = (engineMove?'W':'w');
            lastMoveNotation = "O-O-O";
        }
        if(moveFromX == 4 && moveToX == 2) // O-O-O
        {
            currentChessboard[moveFromY][0] = ' ';
            currentChessboard[moveFromY][3] = (engineMove?'W':'w');
            lastMoveNotation = "O-O-O";
        }
        if(moveFromX == 4 && moveToX == 6) // O-O
        {
            currentChessboard[moveFromY][7] = ' ';
            currentChessboard[moveFromY][5] = (engineMove?'W':'w');
            lastMoveNotation = "O-O";
        }
    }
}
        void Notebook::markPawnPromotion()noexcept{
//==============================================================================================================
    if(1 <= additionalParameter && additionalParameter <= 4) //promotion
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
        switch(additionalParameter)
        {
            case 1: lastMoveNotation += globalType::getCommuniqueCotent({38})[0]; currentChessboard[moveToY][moveToX] = (engineMove?'S':'s'); break;
            case 2: lastMoveNotation += globalType::getCommuniqueCotent({39})[0]; currentChessboard[moveToY][moveToX] = (engineMove?'G':'g'); break;
            case 3: lastMoveNotation += globalType::getCommuniqueCotent({40})[0]; currentChessboard[moveToY][moveToX] = (engineMove?'W':'w'); break;
            case 4: lastMoveNotation += globalType::getCommuniqueCotent({41})[0]; currentChessboard[moveToY][moveToX] = (engineMove?'H':'h'); break;
            default: break;
        }
    }
}
        void Notebook::markEnPassant()noexcept{
//==============================================================================================================
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
        void Notebook::markCommonMove(){//1
//==============================================================================================================
    try{
        if( ! moveMarked)
        {
            char movedPiece =  currentChessboard[moveFromY][moveFromX];
            bool moveWithCapture  = (currentChessboard[moveToY][moveToX] == ' ')? false: true;

            switch(movedPiece)
            {
                case 'P': case 'p':
                    if(moveWithCapture)
                    {
                        lastMoveNotation += getCoordinates(moveFromX, moveFromY);
                        lastMoveNotation.pop_back();
                    }
                    break;
                case 'S': case 's':
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
                    break;
                }
                case 'G': case 'g':
                    lastMoveNotation += globalType::getCommuniqueCotent({39})[0];
                    break;
                case 'W': case 'w':
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
                    break;
                case 'H': case 'h':
                    lastMoveNotation += globalType::getCommuniqueCotent({41})[0];
                    break;
                case 'K': case 'k':
                    lastMoveNotation += globalType::getCommuniqueCotent({42})[0];
                    break;
                default: std::cout<<">"<<movedPiece<<"<"<<moveFromX<<moveFromY; throw std::invalid_argument("Wrong piece.");///////////////////////////////////////////////////////////////
            }
            if(moveWithCapture)
                lastMoveNotation += 'x';
            lastMoveNotation += getCoordinates(moveToX, moveToY);
            currentChessboard[moveToY][moveToX] = movedPiece;
            currentChessboard[moveFromY][moveFromX]   = ' ';
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
            std::string Notebook::getCoordinates(int x, int y){//0+
//==============================================================================================================
    try{
        if(x<0 || 7<x || y<0 || 7<y)
            throw std::invalid_argument("wrong coordinates.");
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
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
        bool Notebook::isThereCheck()noexcept{
//==============================================================================================================
    int x = ((!engineMove)? machineKingLocationX: userKingLocationX);
    int y = ((!engineMove)? machineKingLocationY: userKingLocationY);

    for(int i=1; 0<=y-i; i++)                                       //if check from 12:00 direction Rook/Queen
    {
        if(currentChessboard[y-i][x]==' ') continue;
        if(currentChessboard[y-i][x]==(engineMove? 'H': 'h') || currentChessboard[y-i][x]==(engineMove? 'W': 'w')) return true;
        else break;
    }
    for(int i=1; 0<=y-i && x+i<=7; i++)                             //if check from 01:30 direction Bishop/Queen
    {
        if(currentChessboard[y-i][x+i]==' ') continue;
        if(currentChessboard[y-i][x+i]==(engineMove? 'H': 'h') || currentChessboard[y-i][x+i]==(engineMove? 'G': 'g')) return true;
        else break;
    }
    for(int i=1; x+i<=7; i++)                                       //if check from 03:00 direction Rook/Queen
    {
        if(currentChessboard[y][x+i]==' ') continue;
        if(currentChessboard[y][x+i]==(engineMove? 'H': 'h') || currentChessboard[y][x+i]==(engineMove? 'W': 'w')) return true;
        else break;
    }
    for(int i=1; y+i<=7 && x+i<=7; i++)                             //if check from 04:30 direction Bishop/Queen
    {
        if(currentChessboard[y+i][x+i]==' ') continue;
        if(currentChessboard[y+i][x+i]==(engineMove? 'H': 'h') || currentChessboard[y+i][x+i]==(engineMove? 'G': 'g')) return true;
        else break;
    }
    for(int i=1; y+i<=7; i++)                                       //if check from 06:00 direction Rook/Queen
    {
        if(currentChessboard[y+i][x]==' ') continue;
        if(currentChessboard[y+i][x]==(engineMove? 'H': 'h') || currentChessboard[y+i][x]==(engineMove? 'W': 'w')) return true;
        else break;
    }
    for(int i=1; y+i<=7 && 0<=x-i; i++)                             //if check from 07:30 direction Bishop/Queen
    {
        if(currentChessboard[y+i][x-i]==' ') continue;
        if(currentChessboard[y+i][x-i]==(engineMove? 'H': 'h') || currentChessboard[y+i][x-i]==(engineMove? 'G': 'g')) return true;
        else break;
    }
    for(int i=1; 0<=x-i; i++)                                       //if check from 09:00 direction Rook/Queen
    {
        if(currentChessboard[y][x-i]==' ') continue;
        if(currentChessboard[y][x-i]==(engineMove? 'H': 'h') || currentChessboard[y][x-i]==(engineMove? 'W': 'w')) return true;
        else break;
    }
    for(int i=1; 0<=y-i && 0<=x-i; i++)                             //if check from 10:30 direction Bishop/Queen
    {
        if(currentChessboard[y-i][x-i]==' ') continue;
        if(currentChessboard[y-i][x-i]==(engineMove? 'H': 'h') || currentChessboard[y-i][x-i]==(engineMove? 'G': 'g')) return true;
        else break;
    }

    if(0<=y-2 && x+1<=7 && currentChessboard[y-2][x+1]==(engineMove? 'S': 's')) return true; //check from Knight
    if(0<=y-1 && x+2<=7 && currentChessboard[y-1][x+2]==(engineMove? 'S': 's')) return true;
    if(y+1<=7 && x+2<=7 && currentChessboard[y+1][x+2]==(engineMove? 'S': 's')) return true;
    if(y+2<=7 && x+1<=7 && currentChessboard[y+2][x+1]==(engineMove? 'S': 's')) return true;
    if(y+2<=7 && 0<=x-1 && currentChessboard[y+2][x-1]==(engineMove? 'S': 's')) return true;
    if(y+1<=7 && 0<=x-2 && currentChessboard[y+1][x-2]==(engineMove? 'S': 's')) return true;
    if(0<=y-1 && 0<=x-2 && currentChessboard[y-1][x-2]==(engineMove? 'S': 's')) return true;
    if(0<=y-2 && 0<=x-1 && currentChessboard[y-2][x-1]==(engineMove? 'S': 's')) return true;

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
    void Notebook::updateEntireNotation(){//2
//==============================================================================================================
    try{
        std::ostringstream note;
        if(gameOver)
            endgameService();
        if(semiMoveNumber % 2)
        {
            note<<std::right<<std::setw(3)<<moveNumber<<". "<<std::left<<std::setw(8)<<lastMoveNotation;
            if(additionalParameter == 8 || additionalParameter == 9)
                note<<std::left<<std::setw(7)<<gameResult;
            entireNotation.push_back(note.str());
        }
        else
        {
            note<<std::left<<std::setw(7)<<lastMoveNotation;
            entireNotation[moveNumber-1] += note.str();
            if(additionalParameter == 8 || additionalParameter == 9)
            {
                note.str("");
                note<<std::right<<std::setw(3)<<moveNumber + 1<<". "<<std::left<<std::setw(8)<<gameResult;
                entireNotation.push_back(note.str());
            }
        }
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Notebook::endgameService(){//1
//==============================================================================================================
    try{
        switch(additionalParameter)
        {
            case 6:// user win
                replacePlusWithHashtag();
                lastMoveNotation = (semiMoveNumber%2)? "0.1": "1.0";
                break;
            case 7:// stalemate by user
                lastMoveNotation = "1/2-1/2";
                break;
            case 8:// engine win
                gameResult = (semiMoveNumber%2)? "1.0": "0.1";
                break;
            case 9:// stalemate by engine
                gameResult = "1/2-1/2";
                break;
            default: throw std::invalid_argument("Wrong additional parameter.");
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
            void Notebook::replacePlusWithHashtag(){//0+
//==============================================================================================================
    try{
        int index_edytowanego_stringa = moveNumber - ((semiMoveNumber % 2)? 2: 1);
        int index_plusa = entireNotation[index_edytowanego_stringa].rfind('+');
        if (index_plusa == std::string::npos)
            throw std::runtime_error("The '+' sign was not found in the edited text fragment.");
        entireNotation[index_edytowanego_stringa][index_plusa] = '#';
//#########################################################################
    }
    catch(const std::runtime_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    void Notebook::writeNotation(){//*4
//==============================================================================================================
    try{
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
            if(additionalParameter == 8 || additionalParameter == 9)
            {
                additionalParameter = 0;
                semiMoveNumber++;
                moveNumber = (semiMoveNumber+1)/2;
                writeNotation();
            }
        }
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Notebook::clearNotationArray()noexcept{
//==============================================================================================================
    for(int i=0; i<globalType::letterHeight; i++)
        notationArray[i].clear();
}
        void Notebook::writePreviousNotation(int line, int column){//*3
//==============================================================================================================
    try{
        if(column<0 || 2<column)
            throw std::invalid_argument("Wrong column.");
        if(line<0 || 16<line)
            throw std::invalid_argument("Wrong line.");

        if(additionalParameter == 6)
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
        void Notebook::undoNotationColumns(){//*1
//==============================================================================================================
    try{
        copyNotationColumnArray(0, 1);
        copyNotationColumnArray(1, 2);
        emptyNotationColumnArray(2);
        writeNotationColumn(0);
        writeNotationColumn(1);
        clearNotationColumn(2);
        previousNotationColumn--;
        currentNotationColumn--;
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Notebook::copyNotationColumnArray(int copyIndex, int patternIndex){//0+
//==============================================================================================================
    try{
        if(copyIndex<0 || 1<copyIndex)
            throw std::invalid_argument("Wrong copyIndex.");
        if(patternIndex<1 || 2<patternIndex)
            throw std::invalid_argument("Wrong patternIndex.");
        for(int i=0; i<globalType::columnHeight; i++)
            notationColumnArray[copyIndex][i] = notationColumnArray[patternIndex][i];
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
            void Notebook::emptyNotationColumnArray(int columnNumber){//0+
//==============================================================================================================
    try{
        if(columnNumber<0 || 2<columnNumber)
            throw std::invalid_argument("Wrong column.");
        for(int i=0; i<globalType::columnHeight; i++)
            notationColumnArray[columnNumber][i].clear();
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
            void Notebook::writeNotationColumn(int columnNumber){//*0
//==============================================================================================================
    try{
        int x = globalType::chessboardwidth + columnNumber*globalType::columnWidth;
        for(int i=0; i<globalType::columnHeight; i++)
        {
            if(columnNumber == 1 && 164 < i)
                systemInfo::setConsoleColor(globalType::notation);
            systemInfo::setCursorPosition(x, i);
            std::cout<<notationColumnArray[columnNumber][i];
        }
        systemInfo::setConsoleColor(globalType::white);
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Notebook::clearNotationColumn(int columnNumber){//*0
//==============================================================================================================
    try{
        int x = globalType::chessboardwidth + columnNumber*globalType::columnWidth;
        for(int i=0; i<globalType::columnHeight; i++)
        {
            systemInfo::setCursorPosition(x, i);
            std::cout<<std::string(globalType::columnWidth, ' ');
        }
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Notebook::addNotationArrayContent(std::string content){//2
//==============================================================================================================
    try{
        for(auto cHar: content)
        {
            addCharToNotationArray(cHar);
            for(int i=0; i<globalType::letterHeight; i++)
                notationArray[i] += ' ';
        }
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Notebook::addCharToNotationArray(char cHar){//1
//==============================================================================================================
    try{
        int charIndex = getCharIndex(cHar);
        int charWidth = pixelArtCharacterArray[charIndex][0].size();

        for(int i=0; i<globalType::letterHeight; i++)
        {
            for(int j=0; j<charWidth; j++)
            {
                switch(pixelArtCharacterArray[charIndex][i][j])
                {
                    case ' ': notationArray[i] += ' '   ; break;
                    case 'X': notationArray[i] += '\333'; break;
                    case 'D': notationArray[i] += '\334'; break;
                    case 'G': notationArray[i] += '\337'; break;
                    default : throw std::invalid_argument("Error in 'chars.txt' file content.");
                }
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
                int Notebook::getCharIndex(char cHar){//0+
//==============================================================================================================
    try{

        if('A'<=cHar && cHar<='Z')
            return cHar-65;
        if('a'<=cHar && cHar<='z')
            return cHar-71;
        if('0'<=cHar && cHar<='9')
            return cHar+4;
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
            default: throw std::invalid_argument("Wrong char.");
        }
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
        void Notebook::writeNotationArray(int line, int column, bool backlight){//*0
//==============================================================================================================
    try{
        if(column<0 || 2<column)
            throw std::invalid_argument("Wrong column.");
        if(line<0 || 16<line)
            throw std::invalid_argument("Wrong line.");
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
        void Notebook::rewriteNotationToColumn()noexcept{
//==============================================================================================================
    for(int i=0; i<globalType::letterHeight; i++)
        notationColumnArray[currentNotationColumn][currentNotationLine * globalType::notationLineHeight + i] = notationArray[i];
}
std::string Notebook::getChessboardUpdateCode()noexcept{
//==============================================================================================================
    chessboardUpdateCode.clear();
    generateChessboardUpdateCode();
    return chessboardUpdateCode;
}
    void Notebook::generateChessboardUpdateCode()noexcept{
//==============================================================================================================
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            if(currentChessboard[i][j] != previousChessboard[i][j])
            {
                chessboardUpdateCode += std::to_string(j) + std::to_string(i) + currentChessboard[i][j];
                previousChessboard[i][j] = currentChessboard[i][j];
            }
}
std::string Notebook::saveGameInNotebook(){//0+
//==============================================================================================================
    try{
        auto now = std::chrono::system_clock::now();
        time_t moment_t = std::chrono::system_clock::to_time_t(now);
        std::ostringstream streamOut;
        streamOut << std::put_time(localtime(&moment_t), " %Y-%m-%d %H;%M;%S");
        std::string fileName = globalType::getCommuniqueCotent({44})[0] + streamOut.str();
        std::ofstream file("saved notations/" + fileName + ".txt");
        if (!file.is_open())
            throw std::ofstream::failure("The file could not be opened for writing.");
        file<<std::put_time(localtime(&moment_t), ">>> %Y-%m-%d %H:%M:%S <<<")<<std::endl;
        file<<"==========================="<<std::endl;
        for(auto line: entireNotation)
        file<<line<<std::endl;
        file.close();
        return "\"" + fileName + "\"";
//#########################################################################
    }
    catch(const std::ofstream::failure &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}





















