#include "Game.h"


int main(){

    try{
        Game Game;
        Game.start();
        }
    catch(globalType::errorType &e){
        std::cout<<e.errorMessage;
        }


    return 0;
}
