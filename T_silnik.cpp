#include <iostream>
#include <cmath>
#include "T_silnik.h"
#include "T_ruch.h"//?????????????
using namespace std;

using T_wsk_szachownica = char(*)[8];
extern bool kolor;

T_wsk_szachownica T_silnik::zainicjalizuj_szachownica_wyjsciowa()
{
    return new char[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                          {'p','p','p','p','p','p','p','p'},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {'P','P','P','P','P','P','P','P'},
                          {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};
}
bool T_silnik::czy_przeciwnik_moze_wykonac_takie_posuniecie(int posuniecie)
{
    rozszyfruj_posuniecie_przeciwnika(posuniecie);
    if(!czy_na_polu_wyjsciowym_jest_bierka_przeciwnika())
        return false;
    if(czy_na_polu_docelowym_jest_bierka_przeciwnika())
        return false;
    if(czy_ten_ruch_powoduje_wystawienie_swojego_krola_na_bicie())
        return false;
    if(!czy_ruch_jest_zgodny_z_zasadami())
        return false;
    return true;
}
    void T_silnik::rozszyfruj_posuniecie_przeciwnika(int posuniecie)
{
    naY = posuniecie % 10 - 1;
    posuniecie /=10;
    naX = posuniecie % 10 - 1;
    posuniecie /=10;
    zY  = posuniecie % 10 - 1;
    posuniecie /=10;
    zX  = posuniecie - 1;
}
    bool T_silnik::czy_na_polu_wyjsciowym_jest_bierka_przeciwnika()
{
    T_wsk_szachownica wsk_X = wsk_szachownica_wyjsciowa;
    if(wsk_X[zY][zX]<'g' && 'w'<wsk_X[zY][zX])
        return false;
    else
        return true;
}
    bool T_silnik::czy_na_polu_docelowym_jest_bierka_przeciwnika()
{
    T_wsk_szachownica wsk_X = wsk_szachownica_wyjsciowa;
    if(wsk_X[naY][naX]<'g' && 'w'<wsk_X[naY][naX])
        return false;
    else
        return true;
}
    bool T_silnik::czy_ten_ruch_powoduje_wystawienie_swojego_krola_na_bicie()
{
    T_wsk_szachownica wsk_X = wsk_szachownica_wyjsciowa;
    int potencjalnie_krol_przeciwnika_x;
    int potencjalnie_krol_przeciwnika_y;

    if(wsk_X[zY][zX]=='k') //czy przeciwnik chce sie ruszyc krolem
    {
        potencjalnie_krol_przeciwnika_x = naX;
        potencjalnie_krol_przeciwnika_y = naY;
    }
    else
    {
        potencjalnie_krol_przeciwnika_x = aktualny.polozenie_krola_przeciwnika_x;
        potencjalnie_krol_przeciwnika_y = aktualny.polozenie_krola_przeciwnika_y;
    }
    T_wsk_szachownica kopia = aktualny.skopiuj_szachownice(wsk_X);
    kopia[naY][naX]=kopia[zY][zX];
    kopia[zY][zX]=' ';
    if(aktualny.czy_pole_przeciwnika_jest_bite(potencjalnie_krol_przeciwnika_x, potencjalnie_krol_przeciwnika_y, kopia))
    {
        delete[]kopia;
        return true;
    }
    else
    {
        delete[]kopia;
        return false;
    }
}
    bool T_silnik::czy_ruch_jest_zgodny_z_zasadami()
{
    T_wsk_szachownica wsk_X = wsk_szachownica_wyjsciowa;
    switch(wsk_X[zY][zX])
    {
        case 'p':
            if(zY==1 && naY-zY==2 && zX==naX && wsk_X[zY+1][naX]==' ' && wsk_X[naY][naX]==' ') return true;//ruch o 2 do przodu
            if(naY-zY==1 && zX==naX && wsk_X[naY][naX]==' ')                           return true;//ruch o 1 do przodu
            if(naY-zY==1 && zX-naX==1 && 'G'<=wsk_X[naY][naX] && wsk_X[naY][naX]<='W') return true;//bicie
            if(naY-zY==1 && naX-zX==1 && 'G'<=wsk_X[naY][naX] && wsk_X[naY][naX]<='W') return true;//bicie
            return false;
        case 's':
            if((abs(zX-naX)==2 && abs(zY-naY)==1) || (abs(zX-naX)==1 && abs(zY-naY)==2))
                return true;
            return false;
        case 'g':
        case 'h':
            if(zX+zY==naX+naY) // ruch na skos (\)
            {
                if(zX<naX) //ruch na 4:30
                    for(int x=zX, y=zY; x<naX && y<naY; x++, y++)
                        if(wsk_X[y][x]!=' ')
                            return false;
                if(zX>naX) //ruch na 10:30
                    for(int x=zX, y=zY; x>naX && y>naY; x--, y--)
                        if(wsk_X[y][x]!=' ')
                            return false;
                return true;
            }
            if(zX-zY==naX-naY) // ruch na skos (/)
            {
                if(zX<naX) //ruch na 1:30
                    for(int x=zX, y=zY; x<naX && y>naY; x++, y--)
                        if(wsk_X[y][x]!=' ')
                            return false;
                if(zX>naX) //ruch na 7:30
                    for(int x=zX, y=zY; x>naX && y<naY; x--, y++)
                        if(wsk_X[y][x]!=' ')
                            return false;
                return true;
            }
        case 'w':
            if(zY==naY) //ruch poziomy
            {
                if(zX>naX) //ruch w lewo
                    for(int x=zX; x>naX; x--)
                        if(wsk_X[zY][x]!=' ')
                            return false;
                if(zX<naX) //ruch w prawo
                    for(int x=zX; x<naX; x++)
                        if(wsk_X[zY][x]!=' ')
                            return false;
                return true;
            }
            if(zX==naX) //ruch pionowy
            {
                if(zY>naY)//ruch do gory
                    for(int y=zY; y>naY; y--)
                        if(wsk_X[y][zX]!=' ')
                            return false;
                if(zY<naY)//ruch w dol
                    for(int y=zY; y<naY; y++)
                        if(wsk_X[y][zX]!=' ')
                            return false;
                return true;
            }

        case 'k':
            if(abs(zX-naX)<=1 && abs(zY-naY)<=1)//zwykly ruch krolem
                return true;
            if(zY==naY && aktualny.czy_krol_przeciwnika_sie_ruszyl==false)//roszada
                if(kolor==0)//przeciwnik gra czarnymi
                    if(naX==2 && aktualny.czy_lewa_wierza_przeciwnika_sie_ruszyla==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ' && wsk_X[0][3]==' ')//dluga roszada
                        return true;
                    if(naX==6 && aktualny.czy_prawa_wierza_przeciwnika_sie_ruszyla==false && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')//krotka roszada
                        return true;
                if(kolor==1)//przeciwnik gra bialymi
                    if(naX==5 && aktualny.czy_prawa_wierza_przeciwnika_sie_ruszyla==false && wsk_X[0][4]==' ' && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')//dluga roszada
                        return true;
                    if(naX==1 && aktualny.czy_lewa_wierza_przeciwnika_sie_ruszyla==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ')//krotka roszada
                        return true;
            return false;
    }
}
/*int T_silnik::odpowiedz_przeciwnikowi_na_takie_posuniecie(int posuniecie)
{


}*/
