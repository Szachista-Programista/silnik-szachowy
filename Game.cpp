#include "Game.h"

Game::Game()
{
    globalType::readConfigFile();
    globalType::readCommuniqueFile();
}
void Game::start()
{
    notice.setConsoleSize();
    while(true)
        gameMenu();
}
    void Game::gameMenu()
{
    try
    {
        switch(notice.checkbox(globalType::getCommuniqueCotent({0,1,2,3})))
        {
            case 1:  userPiecesColorMenu();         break;
            case 2:  colorfullElementSettingMenu(); break;
            case 3:  languageSettingMenu();         break;
            case 4:  exitService();                 exit(0);
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
        void Game::userPiecesColorMenu()
{
    try
    {
        switch(notice.checkbox(globalType::getCommuniqueCotent({9,10,11,34})))
       {
           case 1:  pastFirstPlay = true;  Play(1).playWithUser();              break;
           case 2:  pastFirstPlay = true;  Play(0).playWithUser();              break;
           case 3:  pastFirstPlay = true;  Play(randomColor()).playWithUser();  break;
           case 4:                                                              break;
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
            int Game::randomColor()noexcept
{
    srand(static_cast<unsigned int>(time(nullptr)));
    return rand() % 2;
}
        void Game::colorfullElementSettingMenu()
{
    try
    {
        switch(notice.checkbox(globalType::getCommuniqueCotent({4,5,6,7,8})))
        {
            case 1:  colorSettingMenu(globalType::underlightedSquare);  break;
            case 2:  colorSettingMenu(globalType::menu);                break;
            case 3:  colorSettingMenu(globalType::chsenOption);         break;
            case 4:  colorSettingMenu(globalType::notation);            break;
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
    chessboard.deleteCheckbox(293, 63, pastFirstPlay);
}
            void Game::colorSettingMenu(globalType::Color &color)
{
    chessboard.deleteCheckbox(293, 63, pastFirstPlay);
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
        void Game::languageSettingMenu()
{
    chessboard.deleteCheckbox(220, 53, pastFirstPlay);
    try
    {
        switch(notice.checkbox(globalType::getCommuniqueCotent({16,17,35})))
       {
           case 1:  globalType::setLanguage = globalType::polish;  break;
           case 2:  globalType::setLanguage = globalType::english; break;
           case 3:  break;
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
        void Game::exitService()
{
    chessboard.deleteCheckbox(220, 53, pastFirstPlay);
    notice.communique(globalType::getCommuniqueCotent({18})[0],1);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    systemInfo::setCursorPosition(0, globalType::chessboardHeight);
}