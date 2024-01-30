#include "Play.h"
using namespace std;
using namespace globalType;

Play::Play(bool k)noexcept: color{k}, chessboard{k}, engine{k}, notebook{k}{
//==============================================================================================================
}
void Play::playWithUser(){//*4
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
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
    bool Play::userMoveServive(){//*3
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
                notice.communique("to pole nie nalezy do ciebie", 1000);//this square does not belong to you
                chessboard.deleteCommunique(240);
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
                    notice.communique("nieprawidlowy ruch", 1000);//wrong move
                    chessboard.deleteCommunique(170);
                    updateChessboard(currentChessboardUpdateCode, false);
                    continue;
                }
                if(correctMovementMade)
                {
                    if(isUserMakesPromotion())
                    {
                        vector<string> promotionOptions{"1. Skoczek", "2. Goniec", "3. Wieza", "4. Hetman"};
                        EngineMoveunderlighted = notice.checkbox(promotionOptions)*10000;
                        chessboard.deleteCheckbox(220, 60);
                        userMoveCode += EngineMoveunderlighted;
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
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
        int Play::loadCoordinates(){//*2
//==============================================================================================================
    try{
        string coordinates;
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
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
            bool Play::endgameMenu(char cHar){//*1
//==============================================================================================================
    try{
        if(cHar != 27)
            return false;
        vector<string> endgameMenu{"1. kontynuuj", "2. ustawienia colorow", "3. zakoncz partie"};
        bool continueLoop = true;
        while(continueLoop)
            switch(notice.checkbox(endgameMenu))
           {
               case 1: chessboard.deleteCheckbox(205, 43); continueLoop = false; return false;
               case 2: settingMenu(); break;
               case 3: chessboard.deleteCheckbox(205, 43); continueLoop = false; return true;
               default: throw invalid_argument("Option selection error.");
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
                void Play::settingMenu(){//*0
//==============================================================================================================
    try{
        vector<string> colorfullElementMenu{"1. color zaznaczonego pola", "2. color menu/errorMessageow", "3. color wybieranej opcji w menu", "4. color notacji"};
        vector<string> colorsMenu{"1. czerwony", "2. zielony", "3. zolty", "4. niebieski"};

        int chosenElement = notice.checkbox(colorfullElementMenu);
        chessboard.deleteCheckbox(293, 53);

        globalType::Color chosenColor = static_cast<globalType::Color>(notice.checkbox(colorsMenu));
        switch(chosenElement)
       {
           case 1:  globalType::underlightedSquare = chosenColor; break;
           case 2:  globalType::menu               = chosenColor; break;
           case 3:  globalType::chsenOption        = chosenColor; break;
           case 4:  globalType::notation           = chosenColor; break;
           default: throw invalid_argument("Option selection error.");
       }
       chessboard.deleteCheckbox(205, 53);
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
        string Play::generateSquareUpdateCode()noexcept{
//==============================================================================================================
    int x     = userSquareChosenCoordinates / 10;
    int y     = userSquareChosenCoordinates % 10;
    char cHar = notebook.currentChessboard[y][x];
    return to_string(x) + to_string(y) + cHar;
}
        void Play::updateChessboard(string updateCode, bool underlight){//1
//==============================================================================================================
    try{
        string partialUpdateCode;
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
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
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
            default: throw invalid_argument("Wrong char.");
        }
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
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
            throw invalid_argument("Wrong char.");
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
        void Play::offPreviousEngineMoveUnderlight(){//2
//==============================================================================================================
    try{
        if(EngineMoveunderlighted == true)
        {
            updateChessboard(previousChessboardUpdateCode, false);
            EngineMoveunderlighted = false;
        }
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
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
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
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
        engineMoveCode = engine.makeMove(moveCode);
        notebook.generateAndWriteNotation(engineMoveCode);
        currentChessboardUpdateCode = notebook.getChessboardUpdateCode();
        updateChessboard(previousChessboardUpdateCode, false);
        updateChessboard(currentChessboardUpdateCode, true);
        EngineMoveunderlighted = true;
        previousChessboardUpdateCode = currentChessboardUpdateCode;
        return !isItGameover();
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
        bool Play::isItGameover(){//*0
//==============================================================================================================
    try{
        int additionalParameter = engineMoveCode / 10000;
        if(!additionalParameter)
            return false;
        string information;
        switch(additionalParameter)
        {
            case 6:
                information = "WYGRALES...";
                break;
            case 7:
                information = "spowodowales pata...";
                break;
            case 8:
                information = "PRZEGRALES...";
                break;
            case 9:
                information = "maszyna spowodowala pata...";
                break;
            default: throw invalid_argument("Wrong additional parameter.");
        }
        notice.communique(information);
        chessboard.deleteCommunique(260);
        return true;
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
    void Play::notationSavingMenu(){//*0
//==============================================================================================================
    try{
        notice.communique("czy zapisac notacje w notebooku...");
        chessboard.deleteCommunique(300);
        vector<string> notationSavingMenu{"1. tak", "2. nie"};
        switch(notice.checkbox(notationSavingMenu))
        {
           case 1:  notebook.saveGameInNotebook(); break;
           case 2:  break;
           default: throw invalid_argument("Option selection error.");
        }
        chessboard.deleteCheckbox(210, 40);
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

















