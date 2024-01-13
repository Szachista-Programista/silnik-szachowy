#include <cstdlib>
#include <ctime>

#include "Gra.h"




using namespace std;



void Gra::start(){
//==============================================================================================================
    komunikat.ustawienie_wielkosci_obrazu();
    while(true)
        menu_gry();




}
    void Gra::menu_gry(){
//==============================================================================================================
    vector<string> menu_gry{"1. Zagraj", "2. wyjdz"};
    switch(komunikat.pole_wyboru(menu_gry))
   {
       case 1:  uruchom_partie(); break;
       case 2:  exit(0);
   }
}
void Gra::uruchom_partie(){
//==============================================================================================================
    bool kolor;
    menu_koloru(kolor);
    Partia partia(kolor);
    partia.zagraj_z_urzytkownikiem();



}
    void Gra::menu_koloru(bool &kolor){
//==============================================================================================================
    vector<string> menu_koloru{"1. kolorem bialym", "2. kolorem czarnym", "3. kolorem losowym"};
    switch(komunikat.pole_wyboru(menu_koloru))
   {
       case 1:  kolor = 1;                  break;
       case 2:  kolor = 0;                  break;
       case 3:  kolor = losowanie_koloru(); break;
   }

}
    int Gra::losowanie_koloru(){
//==============================================================================================================
    srand(static_cast<unsigned int>(time(nullptr)));
    return std::rand() % 2;
}
