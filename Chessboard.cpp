#include "Chessboard.h"

Chessboard::Chessboard(bool k): color{k}
{
    loadPiecesArray();
    loadChessboardArray();
}
    void Chessboard::loadPiecesArray()
{
    std::ifstream reading;
    try
    {
        reading.open("pieces.txt");
        if (!reading.is_open())
            throw std::ifstream::failure("The file 'pieces.txt' cannot be opened.");
        for(int i=0; i<7; i++)
            if (!getline(reading, piecesArray[i]))
                throw std::ifstream::failure("Error reading character from 'pieces.txt' file.");
    }
    catch(const std::ifstream::failure &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    reading.close();
}
    void Chessboard::loadChessboardArray()
{
    loadChessboard();
    loadCoodinates();
    loadPieces();
}
        void Chessboard::loadChessboard()noexcept
{
    std::string whiteSquare (squareWidth, '\333');
    std::string blackSquare (squareWidth,    ' ');
    std::string outerFrame  (20,    ' ');
    std::string innerFrame  (4,  '\333');
    std::string outerFooter (globalType::chessboardwidth, ' ');

    std::string innerFooter      = outerFrame + std::string(376,'\333') + outerFrame;
    std::string wbwbwbwbSquares  = outerFrame + innerFrame + whiteSquare + blackSquare + whiteSquare + blackSquare + whiteSquare + blackSquare + whiteSquare + blackSquare + innerFrame + outerFrame;
    std::string bwbwbwbwSquares  = outerFrame + innerFrame + blackSquare + whiteSquare + blackSquare + whiteSquare + blackSquare + whiteSquare + blackSquare + whiteSquare + innerFrame + outerFrame;

    for(int i=0; i<10; i++){
        outerFooter.copy(chessboardArray[i],    globalType::chessboardwidth);
        outerFooter.copy(chessboardArray[i+166],globalType::chessboardwidth);
    }
    for(int i=10; i<10+2; i++)
    {
        innerFooter.copy(chessboardArray[i],    globalType::chessboardwidth);
        innerFooter.copy(chessboardArray[i+154],globalType::chessboardwidth);
    }
    for(int i=0; i<8*squareHeight; i+=2*squareHeight){
        for(int j=topMargin;              j<topMargin+squareHeight;   j++) wbwbwbwbSquares.copy(chessboardArray[j+i],globalType::chessboardwidth);
        for(int j=topMargin+squareHeight; j<topMargin+2*squareHeight; j++) bwbwbwbwSquares.copy(chessboardArray[j+i],globalType::chessboardwidth);
    }
}
        void Chessboard::loadCoodinates()
{
    std::ifstream reading;
    std::string coordinates[numberOfCoordinatesChars];
    try
    {
        reading.open("coordinatesChars.txt");
        if (!reading.is_open())
            throw std::ifstream::failure("The file 'coordinatesChars.txt' cannot be opened.");
        for(int i=0; i<16; i++)
            if (!getline(reading, coordinates[i]))
                throw std::ifstream::failure("Error reading character from 'coordinatesChars.txt' file.");
    }
    catch(const std::ifstream::failure &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    reading.close();
    for(int x=43, i=color?8:15 ; x<=43+7*squareWidth; x+=squareWidth, color?i++:i--)
    {
        loadSingeCoordinateChar(x, 3,   coordinates[i]);
        loadSingeCoordinateChar(x, 168, coordinates[i]);
    }
    for(int y=squareHeight, i=color?7:0 ; y<=8*squareHeight; y+=squareHeight, color?i--:i++)
    {
        loadSingeCoordinateChar(6,   y, coordinates[i]);
        loadSingeCoordinateChar(402, y, coordinates[i]);
    }
}
            void Chessboard::loadSingeCoordinateChar(int coordX, int coordY, std::string cHar)
{
    try
    {
        if(coordX<0 || globalType::chessboardwidth<coordX || coordY<0 || globalType::chessboardHeight<coordY)
            throw std::invalid_argument("Wrong coordinates.");
        //if(cHar.size() != 40)
            //throw std::invalid_argument("Error in line length of 'coordinatesChars.txt' file.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    try
    {
        for(int i=0, y=coordY; i<40; y++ )
            for(int x=coordX; x<coordX+8; i++, x++)
                switch(cHar[i])
                {
                    case 'X': chessboardArray[y][x] = '\333'; break;
                    case 'D': chessboardArray[y][x] = '\334'; break;
                    case 'G': chessboardArray[y][x] = '\337'; break;
                    case ' ':                                 break;
                    default : throw std::runtime_error("Error in 'coordinatesChars.txt' file content.");
                }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
        void Chessboard::loadPieces()
{
    globalType::chessboardPointer statringPositions = loadPiecesArrangement();
    try{
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
                switch (statringPositions[i][j])
                {
                    case 'p': loadSinglePiece(7-j,7-i , pawn,    color); break;
                    case 'P': loadSinglePiece(7-j,7-i , pawn,   !color); break;
                    case 'n': loadSinglePiece(7-j,7-i , knight,  color); break;
                    case 'N': loadSinglePiece(7-j,7-i , knight, !color); break;
                    case 'b': loadSinglePiece(7-j,7-i , bishop,  color); break;
                    case 'B': loadSinglePiece(7-j,7-i , bishop, !color); break;
                    case 'r': loadSinglePiece(7-j,7-i , rook,    color); break;
                    case 'R': loadSinglePiece(7-j,7-i , rook,   !color); break;
                    case 'q': loadSinglePiece(7-j,7-i , queen,   color); break;
                    case 'Q': loadSinglePiece(7-j,7-i , queen,  !color); break;
                    case 'k': loadSinglePiece(7-j,7-i , king,    color); break;
                    case 'K': loadSinglePiece(7-j,7-i , king,   !color); break;
                    case ' ': loadSinglePiece(7-j,7-i , no,      color); break;
                    default: throw std::runtime_error("Error in starting positions.");
                }
        delete[]statringPositions;
    }
    catch(const std::runtime_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
            globalType::chessboardPointer Chessboard::loadPiecesArrangement()
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
            void Chessboard::loadSinglePiece(int squareCoordX, int squareCoordY, int piece, bool pieceColor, bool underlight)
{
    try
    {
        if(squareCoordX<0 || 7<squareCoordX || squareCoordY<0 || 7<squareCoordY)
            throw std::invalid_argument("Wrong coordinates.");
        if(piece<0 || 6<piece || squareCoordY<0 || 7<squareCoordY)
            throw std::invalid_argument("Wrong piece.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    int coordX = (squareCoordX)*squareWidth+sideMargin;
    int coordY = (squareCoordY)*squareHeight+topMargin;
    bool squareColor;
    if(!underlight)
        squareColor = !((squareCoordX+squareCoordY)%2);
    else
        squareColor = true;
    try
    {
        for(int i=0, y=coordY; i<874; y++)
            for(int x=coordX; x<coordX+squareWidth; i++, x++)
                switch(piecesArray[piece][i])
                {
                    case 'T': chessboardArray[y][x] = squareColor  ?'\333'                   :' '   ;                   break;
                    case 'G': chessboardArray[y][x] = squareColor  ?' '                      :'\333';                   break;
                    case 'S': chessboardArray[y][x] = pieceColor   ?'\333'                   :' '   ;                   break;
                    case '1': chessboardArray[y][x] = squareColor  ?(pieceColor?'\334':' '):(pieceColor?'\333':'\337'); break;
                    case '2': chessboardArray[y][x] = squareColor  ?(pieceColor?'\337':' '):(pieceColor?'\333':'\334'); break;
                    case '3': chessboardArray[y][x] = squareColor  ?'\334'                   :'\337' ;                  break;
                    case '4': chessboardArray[y][x] = squareColor  ?'\337'                   :'\334' ;                  break;
                    default :  throw std::runtime_error("Error in 'pieces.txt' file content.");
                }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
void Chessboard::deleteCommunique(int communiqueWidth)
{
    try
    {
        if(communiqueWidth<0 || globalType::chessboardwidth<communiqueWidth)
            throw std::invalid_argument("Wrong width of the communique.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    int communiqueBeginningX = (globalType::chessboardwidth-communiqueWidth)/2;
    refreshChessboardPartially(communiqueBeginningX, 81, communiqueBeginningX + communiqueWidth - 1, 93);
}
void Chessboard::deleteCheckbox(int Width, int Height, bool deleteByChessboardWrite)
{
    int checkboxBeginningX = (globalType::chessboardwidth -Width)/2;
    int checkboxBeginningY = (globalType::chessboardHeight-Height) /2;

    for(int i=0; i<Height; i++)
    {
        systemInfo::setCursorPosition(checkboxBeginningX, checkboxBeginningY + i);

        if(deleteByChessboardWrite)
            for(int j=0; j<Width; j++)
                std::cout<<chessboardArray[checkboxBeginningY + i][checkboxBeginningX + j];
        else
            std::cout<<std::string(Width,' ');
    }
}
void Chessboard::writeChessboardArray()
{
    systemInfo::setCursorPosition(0, 0);
    for(int i=0; i<globalType::chessboardHeight; i++)
    {
        for(int j=0; j<globalType::chessboardwidth; j++)
            std::cout<<chessboardArray[i][j];
        std::cout<<std::endl;
    }
}
void Chessboard::updateSquare(int squareCoordX, int squareCoordY, int piece, bool pieceColor, bool underlight)
{
    try
    {
        if(squareCoordX<0 || 7<squareCoordX || squareCoordY<0 || 7<squareCoordY)
            throw std::invalid_argument("Wrong coordinates.");
        if(piece<0 || 6<piece || squareCoordY<0 || 7<squareCoordY)
            throw std::invalid_argument("Wrong piece.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    loadSinglePiece(squareCoordX, squareCoordY, piece, pieceColor, underlight);
    writePiece(squareCoordX, squareCoordY, underlight);
}
    void Chessboard::writePiece(int squareCoordX, int squareCoordY, bool underlight)
{
    try
    {
        if(squareCoordX<0 || 7<squareCoordX || squareCoordY<0 || 7<squareCoordY)
            throw std::invalid_argument("Wrong coordinates.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    if(underlight)
        systemInfo::setConsoleColor(globalType::underlightedSquare);
    int coordX = (squareCoordX)*squareWidth  +sideMargin;
    int coordY = (squareCoordY)*squareHeight +topMargin;
    refreshChessboardPartially(coordX, coordY, coordX+squareWidth-1, coordY+squareHeight-1);
    systemInfo::setConsoleColor(globalType::white);
}
        void Chessboard::refreshChessboardPartially(int fromX, int fromY, int toX, int toY)
{
    try
    {
        if(fromX<0 || globalType::chessboardwidth<fromX || fromY<0 || globalType::chessboardHeight<fromY || toX<0 || globalType::chessboardwidth<toX || toY<0 || globalType::chessboardHeight<toY || fromX>toX || fromY>toY)
            throw std::invalid_argument("Wrong coordinates.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    for(int i=fromY; i<=toY; i++)
    {
        systemInfo::setCursorPosition(fromX, i);
        for(int j=fromX; j<=toX; j++)
            std::cout<<chessboardArray[i][j];
    }
    systemInfo::setCursorPosition(0, 180);
}