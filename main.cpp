#include <iostream>
#include "T_szachownica.h"
#include "T_ruch.h"
#include "T_silnik.h"
using namespace std;
bool kolor=1;
using T_wsk_szachownica = char(*)[8];


int main(){
/*T_wsk_szachownica o=new char[8][8] {{'w','s','g','k','h','g','s','w'},
                                      {'p','p','p','p','p','p','p','p'},
                                      {' ',' ',' ',' ',' ',' ',' ',' '},
                                      {' ',' ',' ',' ',' ',' ',' ',' '},
                                      {' ',' ',' ',' ',' ',' ',' ',' '},
                                      {' ',' ',' ',' ',' ',' ',' ',' '},
                                      {'P','P','P','P','P','P','P','P'},
                                      {'W','S','G','K','H','G','S','W'}};*/

T_silnik x;





cout<<x.wykonaj_posunieice(4142)<<endl;
x.aktualny.wypisz_szachownice(x.wsk_szachownica_robocza);
cout<<"*************************************************"<<endl;
x.wykonaj_posunieice(1112);
//x.aktualny.wypisz_szachownice(x.wsk_szachownica_robocza);
return 0;
}















