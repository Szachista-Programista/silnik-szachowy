#include "Game.h"

Game::Game(){
    try{
        globalType::readConfigFile();
        globalType::readCommuniqueFile();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
void Game::start(){//*3
//==============================================================================================================
    try{
        notice.setConsoleSize();
        while(true)
            gameMenu();
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Game::gameMenu(){//*2
//==============================================================================================================
    try{
        std::vector<std::string> gameMenu = globalType::getCommuniqueCotent({0,1,2,3});
        switch(notice.checkbox(gameMenu))
        {
            case 1:  userPiecesColorMenu();         break;
            case 2:  colorfullElementSettingMenu(); break;
            case 3:  languageSettingMenu();         break;
            case 4:  exitService();                 exit(0);
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
        void Game::userPiecesColorMenu(){//*0+
//==============================================================================================================
    try{
        std::vector<std::string> userPiecesColorMenu = globalType::getCommuniqueCotent({9,10,11,34});
        switch(notice.checkbox(userPiecesColorMenu))
       {
           case 1:  pastFirstPlay = true;  Play(1).playWithUser();              break;
           case 2:  pastFirstPlay = true;  Play(0).playWithUser();              break;
           case 3:  pastFirstPlay = true;  Play(randomColor()).playWithUser();  break;
           case 4:                                                              break;
           default: throw std::invalid_argument("Option selection error.");
       }
//#########################################################################
    }
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
            int Game::randomColor()noexcept{
//==============================================================================================================
    srand(static_cast<unsigned int>(time(nullptr)));
    return rand() % 2;
}
        void Game::colorfullElementSettingMenu(){//*1
//==============================================================================================================
    try{
        std::vector<std::string> colorfullElementMenu = globalType::getCommuniqueCotent({4,5,6,7,8});
        switch(notice.checkbox(colorfullElementMenu))
        {
            case 1:  colorSettingMenu(globalType::underlightedSquare);  break;
            case 2:  colorSettingMenu(globalType::menu);                break;
            case 3:  colorSettingMenu(globalType::chsenOption);         break;
            case 4:  colorSettingMenu(globalType::notation);            break;
            case 5:  break;
            default: throw std::invalid_argument("Option selection error.");
        }
        chessboard.deleteCheckbox(293, 63, pastFirstPlay);
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
            void Game::colorSettingMenu(globalType::Color &color){//*0
//==============================================================================================================
    try{
        std::vector<std::string> colorsMenu = globalType::getCommuniqueCotent({12,13,14,15,8});
        chessboard.deleteCheckbox(293, 63, pastFirstPlay);
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
        void Game::languageSettingMenu(){//*0
//==============================================================================================================
    try{
        chessboard.deleteCheckbox(220, 53, pastFirstPlay);
        std::vector<std::string> languagMenu = globalType::getCommuniqueCotent({16,17,35});
        switch(notice.checkbox(languagMenu))
       {
           case 1:  globalType::setLanguage = globalType::polish;  break;
           case 2:  globalType::setLanguage = globalType::english; break;
           case 3:  break;
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
        void Game::exitService(){//*0
//==============================================================================================================
    try{
        globalType::writeConfigFile();
        chessboard.deleteCheckbox(220, 53, pastFirstPlay);
        notice.communique(globalType::getCommuniqueCotent({18})[0],1);
        systemInfo::setCursorPosition(0, globalType::chessboardHeight);
//#########################################################################
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}






















