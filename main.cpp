#include "Game.h"


int main(){
    try{
        Game game;
        game.start();
        }
    catch(globalType::errorType &e){
        std::cout<<e.errorMessage;
        }

/*
Notice n;
n.setConsoleSize();
n.communique("ABCDEFGHIJKLMNOPQXYZ");
n.communique("abcdefghijklmnopqxyz");
n.communique("0123456789.+-=# /!?\":");
*/

    return 0;
}