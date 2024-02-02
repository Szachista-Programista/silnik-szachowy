#include "Play.h"

Play::Play(bool k)noexcept: color{k}, chessboard{k}, engine{k}, notebook{k}{
//==============================================================================================================
}
void Play::playWithUser(){//*5
//==============================================================================================================
    try{
        notebook.clearNotationColumn(0);
        notebook.clearNotationColumn(1);
        notebook.clearNotationColumn(2);
        chessboard.writeChessboardArray();

        if(color == false) //engine starts the game
            engineMoveServive(10000);
        while(userMoveServive() && engineMoveServive(userMoveCode)){}
        notationSavingMenu();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    bool Play::userMoveServive(){//*4
//==============================================================================================================
    try{
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
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        int Play::loadCoordinates(){//*3
//==============================================================================================================
    try{
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
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            bool Play::endgameMenu(char cHar){//*2
//==============================================================================================================
    try{
        if(cHar != 27)
            return false;
        std::vector<std::string> endgameMenu = globalType::getCommuniqueCotent({20,1,21});
        bool continueLoop = true;

        while(continueLoop)
            switch(notice.checkbox(endgameMenu))
           {
               case 1: chessboard.deleteCheckbox(205, 43); continueLoop = false; return false;
               case 2: colorfullElementSettingMenu(); break;
               case 3: chessboard.deleteCheckbox(205, 43); continueLoop = false; return true;
               default: throw std::invalid_argument("Option selection error.");
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
                void Play::colorfullElementSettingMenu(){//*1
//==============================================================================================================
    try{
        std::vector<std::string> colorfullElementMenu = globalType::getCommuniqueCotent({4,5,6,7,8});
        switch(notice.checkbox(colorfullElementMenu))
        {
            case 1:  colorSettingMenu(globalType::underlightedSquare);   break;
            case 2:  colorSettingMenu(globalType::menu);                 break;
            case 3:  colorSettingMenu(globalType::chsenOption);          break;
            case 4:  colorSettingMenu(globalType::notation);             break;
            case 5:  break;
            default: throw std::invalid_argument("Option selection error.");
        }
        chessboard.deleteCheckbox(293, 63);
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
                    void Play::colorSettingMenu(globalType::Color &color){//*0
//==============================================================================================================
    try{
        std::vector<std::string> colorsMenu = globalType::getCommuniqueCotent({12,13,14,15,8});
        chessboard.deleteCheckbox(293, 63);
        switch(notice.checkbox(colorsMenu))
        {
            case 1:  color = static_cast<globalType::Color>(1); break;
            case 2:  color = static_cast<globalType::Color>(2); break;
            case 3:  color = static_cast<globalType::Color>(3); break;
            case 4:  color = static_cast<globalType::Color>(4); break;
            case 5:  break;
            default: throw std::invalid_argument("Option selection error.");
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
        std::string Play::generateSquareUpdateCode()noexcept{
//==============================================================================================================
    int x     = userSquareChosenCoordinates / 10;
    int y     = userSquareChosenCoordinates % 10;
    char cHar = notebook.currentChessboard[y][x];
    return std::to_string(x) + std::to_string(y) + cHar;
}
        void Play::updateChessboard(std::string updateCode, bool underlight){//1
//==============================================================================================================
    try{
        std::string partialUpdateCode;
        int x;
        int y;
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
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            int Play::getPieceCode(char cHar){//0+
//==============================================================================================================
    try{
        switch(cHar)
        {
            case ' ':           return 0;
            case 'P': case 'p': return 1;
            case 'S': case 's': return 2;
            case 'G': case 'g': return 3;
            case 'W': case 'w': return 4;
            case 'H': case 'h': return 5;
            case 'K': case 'k': return 6;
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
            bool Play::getPieceColor(char cHar){//0+
//==============================================================================================================
    try{
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
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
        void Play::offPreviousEngineMoveUnderlight(){//2
//==============================================================================================================
    try{
        if(engineMoveUnderlighted == true)
        {
            updateChessboard(previousChessboardUpdateCode, false);
            engineMoveUnderlighted = false;
        }
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        bool Play::isChosenSquareCorrect(){//1
//==============================================================================================================
    try{
        int x      = userSquareChosenCoordinates / 10;
        int y      = userSquareChosenCoordinates % 10;
        char piece = notebook.currentChessboard[y][x];
        return (getPieceColor(piece) == color);
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        bool Play::isUserMakesPromotion()noexcept{
//==============================================================================================================
    int fromX =  userMoveCode/1000;
    int fromY = (userMoveCode/ 100)%10;
    int toY   =  userMoveCode      %10;
    return (notebook.currentChessboard[fromY][fromX] == 'p' && toY == 7);
}
    bool Play::engineMoveServive(int moveCode){//*1
//==============================================================================================================
    try{
        endgineMoveCode = engine.makeMove(moveCode);
        notebook.generateAndWriteNotation(endgineMoveCode);
        currentChessboardUpdateCode = notebook.getChessboardUpdateCode();
        updateChessboard(previousChessboardUpdateCode, false);
        updateChessboard(currentChessboardUpdateCode, true);
        engineMoveUnderlighted = true;
        previousChessboardUpdateCode = currentChessboardUpdateCode;
        return !isItGameover();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        bool Play::isItGameover(){//*0
//==============================================================================================================
    try{
        int additionalParameter = endgineMoveCode / 10000;
        if( 0<= additionalParameter && additionalParameter <= 4)
            return false;
        switch(additionalParameter)
        {
            case 6:
                notice.communique(globalType::getCommuniqueCotent({22})[0]);
                break;
            case 7:
                notice.communique(globalType::getCommuniqueCotent({23})[0]);
                break;
            case 8:
                notice.communique(globalType::getCommuniqueCotent({24})[0]);
                break;
            case 9:
                notice.communique(globalType::getCommuniqueCotent({25})[0]);
                break;
            default: throw std::invalid_argument("Wrong additional parameter.");
        }
        chessboard.deleteCommunique(300);
        return true;
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
    void Play::notationSavingMenu(){//*0
//==============================================================================================================
    try{
        notice.communique(globalType::getCommuniqueCotent({26})[0]);
        chessboard.deleteCommunique(415);
        std::vector<std::string> notationSavingMenu = globalType::getCommuniqueCotent({27,28});
        switch(notice.checkbox(notationSavingMenu))
        {
           case 1:  notebook.saveGameInNotebook(); break;
           case 2:  break;
           default: throw std::invalid_argument("Option selection error.");
        }
        chessboard.deleteCheckbox(210, 33);
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

















