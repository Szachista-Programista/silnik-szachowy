#include <iostream>
#include <windows.h>
#include "T_szachownica.h"
#include "T_ruch.h"
#include "T_silnik.h"
using namespace std;
bool kolor=1;
using T_wsk_szachownica = char(*)[8];

int znak;//////////////

int main(){


T_szachownica x;

x.ustawienie_wielkosci_obrazu();
x.wypisz_tablica_szachownica();


return 0;
}















