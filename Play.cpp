#include "Play.h"

Play::Play(bool k)noexcept:
color{k}, chessboard{k}, engine{k}, notebook{k}
{}
void Play::playWithUser()
{
    notebook.clearNotationColumn(0);
    notebook.clearNotationColumn(1);
    notebook.clearNotationColumn(2);
    chessboard.writeChessboardArray();

    if(color == false) //engine starts the game
        engineMoveServive(10000);
    while(userMoveServive() && engineMoveServive(userMoveCode)){}
    notationSavingMenu();
}
    bool Play::userMoveServive()
{
    while(true)
    {
        if((userMoveCode = userSquareChosenCoordinates = loadCoordinates()) == -1)
            return false;
        currentChessboardUpdateCode = SquareUpdateCode = generateSquareUpdateCode();
        offPreviousEngineMoveUnderlight();
        correcSquareChosen = isChosenSquareCorrect();
        updateChessboard(SquareUpdateCode, true);
        if(!correcSquareChosen)
        {
            notice.communique(globalType::getCommuniqueCotent({19})[0], 1000);
            chessboard.deleteCommunique(300);
            updateChessboard(SquareUpdateCode, false);
            continue;
        }
        if(correcSquareChosen)
        {
            if((userSquareChosenCoordinates = loadCoordinates()) == -1)
                return false;
            userMoveCode = userMoveCode*100 + userSquareChosenCoordinates;
            currentChessboardUpdateCode += SquareUpdateCode = generateSquareUpdateCode();
            correctMovementMade = engine.canUserMakeSuchMove(userMoveCode);
            updateChessboard(SquareUpdateCode, true);
            if(!correctMovementMade)
            {
                notice.communique(globalType::getCommuniqueCotent({29})[0], 1000);
                chessboard.deleteCommunique(200);
                updateChessboard(currentChessboardUpdateCode, false);
                continue;
            }
            if(correctMovementMade)
            {
                if(isUserMakesPromotion())
                {
                    std::vector<std::string> promotionOptions = globalType::getCommuniqueCotent({30,31,32,33});
                    promotionCode = notice.checkbox(promotionOptions)*10000;
                    chessboard.deleteCheckbox(220, 60);
                    userMoveCode += promotionCode;
                }
                notebook.generateAndWriteNotation(userMoveCode);
                currentChessboardUpdateCode = notebook.getChessboardUpdateCode();
                updateChessboard(currentChessboardUpdateCode, true);
                previousChessboardUpdateCode = currentChessboardUpdateCode;
                return true;
            }
        }
    }
}
        int Play::loadCoordinates()
{
    std::string coordinates;
    char cHar;
    bool charLoaded = false;

    while(true)
    {
        cHar = systemInfo::getChar();
        if(endgameMenu(cHar))
            return -1;
        if('A'<=cHar && cHar<='H')
        {
            coordinates = cHar;
            if(cHar == 'A')
                chessboard.deleteCommunique(30);
            else
                chessboard.deleteCommunique(28);
            notice.communique(coordinates, 1);
            charLoaded = true;
            continue;
        }
        if('a'<=cHar && cHar<='h')
        {
            cHar = toupper(cHar);
            coordinates = cHar;
            if(cHar == 'A')
                chessboard.deleteCommunique(30);
            else
                chessboard.deleteCommunique(28);
            notice.communique(coordinates, 1);
            charLoaded = true;
            continue;
        }
        if('1'<=cHar && cHar<='8' && charLoaded == true)
        {
            coordinates += cHar;
            notice.communique(coordinates, 400);
            if(coordinates[0] == 'A')
                chessboard.deleteCommunique(40);
            else
                chessboard.deleteCommunique(38);
            break;
        }
    }
    if(color == true)
    {
        int coordX = 7-(coordinates[0]-'A');
        int coordY =    coordinates[1]-'1';
        return coordX*10 + coordY;
    }
    if(color == false)
    {
        int coordX =    coordinates[0]-'A';
        int coordY = 7-(coordinates[1]-'1');
        return  coordX*10 + coordY;
    }
}
            bool Play::endgameMenu(char cHar)
{
    if(cHar != 27)
        return false;
    bool continueLoop = true;
    try
    {
        while(continueLoop)
            switch(notice.checkbox(globalType::getCommuniqueCotent({20,1,21})))
           {
               case 1: chessboard.deleteCheckbox(205, 43); continueLoop = false; return false;
               case 2: colorfullElementSettingMenu(); break;
               case 3: chessboard.deleteCheckbox(205, 43); continueLoop = false; return true;
               default: throw std::runtime_error("Option selection error.");
           }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
                void Play::colorfullElementSettingMenu()
{
    try
    {
        switch(notice.checkbox(globalType::getCommuniqueCotent({4,5,6,7,8})))
        {
            case 1:  colorSettingMenu(globalType::underlightedSquare);   break;
            case 2:  colorSettingMenu(globalType::menu);                 break;
            case 3:  colorSettingMenu(globalType::chsenOption);          break;
            case 4:  colorSettingMenu(globalType::notation);             break;
            case 5:  break;
            default: throw std::runtime_error("Option selection error.");
        }
    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    chessboard.deleteCheckbox(293, 63);
}
                    void Play::colorSettingMenu(globalType::Color &color)
{
    chessboard.deleteCheckbox(293, 63);
    try
    {
        switch(notice.checkbox(globalType::getCommuniqueCotent({12,13,14,15,8})))
        {
            case 1:  color = static_cast<globalType::Color>(1); break;
            case 2:  color = static_cast<globalType::Color>(2); break;
            case 3:  color = static_cast<globalType::Color>(3); break;
            case 4:  color = static_cast<globalType::Color>(4); break;
            case 5:  break;
            default: throw std::runtime_error("Option selection error.");
        }

    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    globalType::writeConfigFile();
}
        std::string Play::generateSquareUpdateCode()noexcept
{
    int x     = userSquareChosenCoordinates / 10;
    int y     = userSquareChosenCoordinates % 10;
    char cHar = notebook.currentChessboard[y][x];
    return std::to_string(x) + std::to_string(y) + cHar;
}
        void Play::updateChessboard(std::string updateCode, bool underlight)
{
    try
    {
        if(updateCode.size()%3 != 0)
            throw std::invalid_argument("Wrong updateCode.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    std::string partialUpdateCode;
    int x,y;
    char cHar;
    int piece;
    bool pieceColor;
    while(updateCode.size())
    {
        partialUpdateCode = updateCode.substr(0, 3);
        updateCode.erase(0, 3);

        x = 7-(partialUpdateCode[0] - '0');
        y = 7-(partialUpdateCode[1] - '0');
        cHar = partialUpdateCode[2];
        piece = getPieceCode(cHar);
        pieceColor = getPieceColor(cHar);
        chessboard.updateSquare(x, y, piece, pieceColor, underlight);
    }
}
            int Play::getPieceCode  (char cHar)
{
    try
    {
        switch(cHar)
        {
            case ' ':           return 0;
            case 'P': case 'p': return 1;
            case 'N': case 'n': return 2;
            case 'B': case 'b': return 3;
            case 'R': case 'r': return 4;
            case 'Q': case 'q': return 5;
            case 'K': case 'k': return 6;
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
            bool Play::getPieceColor(char cHar)
{
    try
    {
        if(isalpha(cHar))
        {
            if(islower(cHar))
                return color;
            if(isupper(cHar))
                return !color;
        }
        else if(cHar == ' ')
            return !color;
        else
            throw std::invalid_argument("Wrong char.");
    }
    catch(const std::invalid_argument &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
        void Play::offPreviousEngineMoveUnderlight()
{
    if(engineMoveUnderlighted == true)
    {
        updateChessboard(previousChessboardUpdateCode, false);
        engineMoveUnderlighted = false;
    }
}
        bool Play::isChosenSquareCorrect()
{
    int x      = userSquareChosenCoordinates / 10;
    int y      = userSquareChosenCoordinates % 10;
    char piece = notebook.currentChessboard[y][x];
    return (getPieceColor(piece) == color);
}
        bool Play::isUserMakesPromotion()noexcept
{
    int fromX =  userMoveCode/1000;
    int fromY = (userMoveCode/ 100)%10;
    int toY   =  userMoveCode      %10;
    return (notebook.currentChessboard[fromY][fromX] == 'p' && toY == 7);
}
    bool Play::engineMoveServive(int moveCode)
{
    engineMoveCode = engine.makeMove(moveCode);
    notebook.generateAndWriteNotation(engineMoveCode);
    currentChessboardUpdateCode = notebook.getChessboardUpdateCode();
    updateChessboard(previousChessboardUpdateCode, false);
    updateChessboard(currentChessboardUpdateCode, true);
    engineMoveUnderlighted = true;
    previousChessboardUpdateCode = currentChessboardUpdateCode;
    return ( ! isItGameover());
}
        bool Play::isItGameover()
{
    int gameOverParameter = engineMoveCode / 100000;
    if(!gameOverParameter)
        return false;
    try
    {
        switch(gameOverParameter)
        {
            case 1:
                notice.communique(globalType::getCommuniqueCotent({22})[0]);
                break;
            case 2:
                notice.communique(globalType::getCommuniqueCotent({23})[0]);
                break;
            case 3:
                notice.communique(globalType::getCommuniqueCotent({24})[0]);
                break;
            case 4:
                notice.communique(globalType::getCommuniqueCotent({23})[0]);
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
    chessboard.deleteCommunique(300);
    return true;
}
    void Play::notationSavingMenu()
{
    notice.communique(globalType::getCommuniqueCotent({26})[0]);
    chessboard.deleteCommunique(415);
    try
    {
        switch(notice.checkbox(globalType::getCommuniqueCotent({27,28})))
        {
           case 1:
               chessboard.deleteCheckbox(210, 33, true);
               notice.communique(globalType::getCommuniqueCotent({43})[0], 700);
               notice.communique(notebook.saveGameInNotebook(), 1400);
               chessboard.deleteCommunique(320);
               break;
           case 2:  break;
           default: throw std::runtime_error("Option selection error.");
        }

    }
    catch(const std::runtime_error &e)
    {
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    chessboard.deleteCheckbox(210, 33);
}