#include <iostream>
#include "T_szachownica.h"
#include "T_silnik.h"
#include "T_ruch.h"

using namespace std;

bool kolor=0;
using T_wsk_szachownica = char(*)[8];



int main(){


T_wsk_szachownica wsk_X  = new char[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                                          {'p','p','p','p','p','p','p','p'},
                                          {' ',' ',' ',' ',' ',' ',' ',' '},
                                          {' ',' ',' ',' ',' ',' ',' ',' '},
                                          {' ',' ',' ',' ',' ',' ',' ',' '},
                                          {' ',' ',' ',' ',' ',' ',' ',' '},
                                          {'P','P','P','P','P','P','P','P'},
                                          {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};


T_szachownica x;

x.ustawienie_wielkosci_obrazu();
x.wypisz_tablica_szachownica();




return 0;
}















