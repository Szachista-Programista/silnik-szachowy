#include "Game.h"

int main(){

    try{
        Game game;
        game.start();
        }
    catch(globalType::errorType &e){
        std::cout<<e.errorMessage;
        }



    return 0;
}


