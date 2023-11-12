#include <iostream>
#include <windows.h>
#include "Szachownica.h"
#include "Ruch.h"
#include "Silnik.h"
#include "Notatnik.h"
#include "Komunikat.h"
#include "Gra.h"

using namespace std;
bool kolor=1;
using T_wsk_szachownica = char(*)[8];
// do³¹czyæ #include <iterator> tam gdzie jest end i begin


int main(){
Komunikat K;
Szachownica S(kolor);


K.ustawienie_wielkosci_obrazu();
//S.wypisz_tablica_szachownica();


vector<string> x{"1.iiiiiiiiiiiiiiiiiiiiiiiiiiiiiyyyyyyyyyyy","2.","3.","4."};


K.pole_wyboru(x);
cout<<K.szerokosc_pola;


return 0;
}





