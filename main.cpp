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



int main(){


Gra g(kolor);
g.zagraj_z_urzytkownikiem();


return 0;
}





