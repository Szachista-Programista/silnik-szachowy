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


T_silnik x;


int posuniecie;
x.aktualny.wypisz_szachownice(x.wsk_szachownica_robocza);

for(;;)
{
    cin>>posuniecie;
    x.wykonaj_posunieice(posuniecie);
    system("cls");

    x.aktualny.wypisz_szachownice(x.wsk_szachownica_robocza);
}





return 0;
}















