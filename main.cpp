#include <iostream>
#include "T_szachownica.h"
#include "T_silnik.h"
#include "T_ruch.h"

using namespace std;

bool kolor=0;
using T_wsk_szachownica = char(*)[8];

T_wsk_szachownica wsk_X  = new char[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                                          {'p','p','p','p','p','p','p','p'},
                                          {' ',' ',' ',' ',' ',' ',' ',' '},
                                          {' ',' ',' ',' ',' ',' ',' ',' '},
                                          {' ',' ',' ',' ',' ',' ',' ',' '},
                                          {' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'P','P','P','P','P','P','P','P'},
                                          {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};




int main(){

/*
T_szachownica fff;
fff.ustawienie_wielkosci_obrazu();
fff.wypisz_tablica_szachownica();
*/



//*

T_ruch x;


x.wypisz_szachownice(wsk_X);

x.oblicz_nastepny_ruch(wsk_X);

x.wypisz_szachownice(wsk_X);


//*/

return 0;
}















