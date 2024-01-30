#include "Game.h"
using namespace std;
using namespace globalType;

void Game::start(){//*3
//==============================================================================================================
    try{
        notice.setConsoleSize();
        globalType::readConfigFile();
        while(true)
            gameMenu();
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Game::gameMenu(){//*2
//==============================================================================================================
    try{
        vector<string> gameMenu{"1. zagraj", "2. ustawienia kolorow", "3. ustawienia jezykow", "4. wyjdz"};
        switch(notice.checkbox(gameMenu))
        {
            case 1:  runThePlay();                  break;
            case 2:  colorfullElementSettingMenu(); break;
            case 3:  languageSettingMenu();         break;
            case 4:  exitService();                 exit(0);
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
        void Game::runThePlay(){//1
//==============================================================================================================
    try{
        chessboard.deleteCheckbox(220, 53, pastFirstPlay);
        bool color;
        pastFirstPlay = true;
        userPiecesColorMenu(color);
        Play(color).playWithUser();
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Game::userPiecesColorMenu(bool &color){//0+
//==============================================================================================================
    try{
        vector<string> userPiecesColorMenu{"1. kolorem bialym", "2. kolorem czarnym", "3. kolorem losowym"};
        switch(notice.checkbox(userPiecesColorMenu))
       {
           case 1:  color = 1; break;
           case 2:  color = 0; break;
           case 3:  color = randomColor(); break;
           default: throw invalid_argument("Option selection error.");
       }
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
                int Game::randomColor()noexcept{
//==============================================================================================================
    srand(static_cast<unsigned int>(time(nullptr)));
    return rand() % 2;
}
        void Game::colorfullElementSettingMenu(){
//==============================================================================================================
    try{
        vector<string> colorfullElementMenu{"1. kolor zaznaczonego pola", "2. kolor menu/komunikatow", "3. kolor wybieranej opcji w menu", "4. kolor notacji", "5. wroc"};
        chessboard.deleteCheckbox(293, 53, pastFirstPlay);
        switch(notice.checkbox(colorfullElementMenu))
        {
            case 1:  colorSettingMenu(globalType::underlightedSquare);  break;
            case 2:  colorSettingMenu(globalType::menu);                break;
            case 3:  colorSettingMenu(globalType::chsenOption);         break;
            case 4:  colorSettingMenu(globalType::notation);            break;
            case 5:  break;
            default: throw invalid_argument("Option selection error.");
        }
        chessboard.deleteCheckbox(205, 53, pastFirstPlay);
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
            void Game::colorSettingMenu(globalType::Color &color){
//==============================================================================================================
        vector<string> colorsMenu{"1. czerwony", "2. zielony", "3. zolty", "4. niebieski", "5. wroc"};

        //chessboard.deleteCheckbox(293, 53, pastFirstPlay);

        switch(notice.checkbox(colorsMenu))
        {
            case 1:  color = static_cast<Color>(1); break;
            case 2:  color = static_cast<Color>(2); break;
            case 3:  color = static_cast<Color>(3); break;
            case 4:  color = static_cast<Color>(4); break;
            case 5:  break;
            default: throw invalid_argument("Option selection error.");
        }



}
        void Game::languageSettingMenu(){
//==============================================================================================================
    chessboard.deleteCheckbox(220, 53, pastFirstPlay);
    try{
        vector<string> languagMenu{"1. Polski", "2. English", "3. wroc"};
        switch(notice.checkbox(languagMenu))
       {
           case 1:  globalType::setLanguage = globalType::polish;  break;
           case 2:  globalType::setLanguage = globalType::english; break;
           case 3:  break;
           default: throw invalid_argument("Option selection error.");
       }
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
        void Game::exitService(){
//==============================================================================================================
    globalType::writeConfigFile();
    chessboard.deleteCheckbox(220, 53, pastFirstPlay);
    notice.communique("Do zobaczenia.");
}






















