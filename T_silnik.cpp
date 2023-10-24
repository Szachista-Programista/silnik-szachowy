#include <iostream>
#include <cmath>
#include "T_silnik.h"
using namespace std;
using T_wsk_szachownica = char(*)[8];
extern bool kolor;

T_silnik::T_silnik()
{
    wsk_szachownica_wyjsciowa = zainicjalizuj_szachownice();
    wsk_szachownica_robocza   = zainicjalizuj_szachownice();
}
    T_wsk_szachownica T_silnik::zainicjalizuj_szachownice()
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
T_silnik::~T_silnik()
{
    delete[]wsk_szachownica_wyjsciowa;
    delete[]wsk_szachownica_robocza;
}
//********************************************************************************************
bool T_silnik::czy_urzytkownik_moze_wykonac_takie_posuniecie(int posuniecie_urzytkownika)/////////////////
{
    rozszyfruj_posuniecie_urzytkownika(posuniecie_urzytkownika);
    if( !czy_na_polu_wyjsciowym_jest_bierka_urzytkownika()           ){cout<<"brak na starcie"<<endl;return false;}
    if(  czy_na_polu_docelowym_jest_bierka_urzytkownika()            ){cout<<"bierka na docelowym"<<endl;return false;}
    if(  czy_ten_ruch_powoduje_wystawienie_swojego_krola_na_bicie() ){cout<<"bity krol"<<endl;return false;}
    if( !czy_ruch_jest_zgodny_z_zasadami()                          ){cout<<"nie zgodne z zasadami"<<endl;return false;}
    return true;
}
    void T_silnik::rozszyfruj_posuniecie_urzytkownika(int posuniecie_urzytkownika)
{
    u.naY = posuniecie_urzytkownika % 10;
    posuniecie_urzytkownika /=10;
    u.naX = posuniecie_urzytkownika % 10;
    posuniecie_urzytkownika /=10;
    u.zY  = posuniecie_urzytkownika % 10;
    posuniecie_urzytkownika /=10;
    u.zX  = posuniecie_urzytkownika;
}
    bool T_silnik::czy_na_polu_wyjsciowym_jest_bierka_urzytkownika()
{
    T_wsk_szachownica wsk_X = wsk_szachownica_robocza;
    if(wsk_X[u.zY][u.zX]<'g' && 'w'<wsk_X[u.zY][u.zX])
        return false;
    else
        return true;
}
    bool T_silnik::czy_na_polu_docelowym_jest_bierka_urzytkownika()
{
    T_wsk_szachownica wsk_X = wsk_szachownica_robocza;
    if(wsk_X[u.naY][u.naX]<'g' || 'w'<wsk_X[u.naY][u.naX])
        return false;
    else
        return true;
}
    bool T_silnik::czy_ten_ruch_powoduje_wystawienie_swojego_krola_na_bicie()
{
    T_wsk_szachownica wsk_X = wsk_szachownica_robocza;
    int potencjalnie_krol_przeciwnika_x;
    int potencjalnie_krol_przeciwnika_y;

    if(wsk_X[u.zY][u.zX]=='k') //czy przeciwnik chce sie ruszyc krolem
    {
        potencjalnie_krol_przeciwnika_x = u.naX;
        potencjalnie_krol_przeciwnika_y = u.naY;
    }
    else
    {
        potencjalnie_krol_przeciwnika_x = aktualny.polozenie_krola_przeciwnika_x;
        potencjalnie_krol_przeciwnika_y = aktualny.polozenie_krola_przeciwnika_y;
    }
    T_wsk_szachownica kopia = aktualny.skopiuj_szachownice(wsk_X);
    kopia[u.naY][u.naX]=kopia[u.zY][u.zX];
    kopia[u.zY][u.zX]=' ';
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
    T_wsk_szachownica wsk_X = wsk_szachownica_robocza;
    switch(wsk_X[u.zY][u.zX])
    {
        case 'p':
            if(u.zY==1 && u.naY-u.zY==2 && u.zX==u.naX && wsk_X[u.zY+1][u.naX]==' ' && wsk_X[u.naY][u.naX]==' ') return true;//ruch o 2 do przodu
            if(u.naY-u.zY==1 && u.zX==u.naX && wsk_X[u.naY][u.naX]==' ')                               return true;//ruch o 1 do przodu
            if(u.naY-u.zY==1 && u.zX-u.naX==1 && 'G'<=wsk_X[u.naY][u.naX] && wsk_X[u.naY][u.naX]<='W') {cout<<"e";return true;}//bicie
            if(u.naY-u.zY==1 && u.naX-u.zX==1 && 'G'<=wsk_X[u.naY][u.naX] && wsk_X[u.naY][u.naX]<='W') {cout<<"f";return true;}//bicie
            cout<<wsk_X[u.naY][u.naX];
            return false;
        case 's':
            if((abs(u.zX-u.naX)==2 && abs(u.zY-u.naY)==1) || (abs(u.zX-u.naX)==1 && abs(u.zY-u.naY)==2))
                return true;
            return false;
        case 'g':
        case 'h':
            if(u.zX-u.zY==u.naX-u.naY) // ruch na skos (\)
            {
                if(u.zX<u.naX) //ruch na 4:30
                    for(int x=u.zX+1, y=u.zY+1; x<u.naX && y<u.naY; x++, y++)
                        if(wsk_X[y][x]!=' ')
                            return false;
                if(u.zX>u.naX) //ruch na 10:30
                    for(int x=u.zX-1, y=u.zY-1; x>u.naX && y>u.naY; x--, y--)
                        if(wsk_X[y][x]!=' ')
                            return false;
                return true;
            }
            if(u.zX+u.zY==u.naX+u.naY) // ruch na skos (/)
            {
                if(u.zX<u.naX) //ruch na 1:30
                    for(int x=u.zX+1, y=u.zY-1; x<u.naX && y>u.naY; x++, y--)
                        if(wsk_X[y][x]!=' ')
                            return false;
                if(u.zX>u.naX) //ruch na 7:30
                    for(int x=u.zX-1, y=u.zY+1; x>u.naX && y<u.naY; x--, y++)
                        if(wsk_X[y][x]!=' ')
                            return false;
                return true;
            }
        case 'w':
            if(u.zY==u.naY) //ruch poziomy
            {
                if(u.zX>u.naX) //ruch w lewo
                    for(int x=u.zX-1; x>u.naX; x--)
                        if(wsk_X[u.zY][x]!=' ')
                            return false;
                if(u.zX<u.naX) //ruch w prawo
                    for(int x=u.zX+1; x<u.naX; x++)
                        if(wsk_X[u.zY][x]!=' ')
                            return false;
                return true;
            }
            if(u.zX==u.naX) //ruch pionowy
            {
                if(u.zY>u.naY)//ruch do gory
                    for(int y=u.zY-1; y>u.naY; y--)
                        if(wsk_X[y][u.zX]!=' ')
                            return false;
                if(u.zY<u.naY)//ruch w dol
                    for(int y=u.zY+1; y<u.naY; y++)
                        if(wsk_X[y][u.zX]!=' ')
                            return false;
                return true;
            }

        case 'k':
            if(abs(u.zX-u.naX)<=1 && abs(u.zY-u.naY)<=1)//zwykly ruch krolem
                return true;
            if(u.zY==u.naY && aktualny.czy_krol_przeciwnika_sie_ruszyl==false)//roszada
                if(kolor==0)//przeciwnik gra czarnymi
                    if(u.naX==2 && aktualny.czy_lewa_wierza_przeciwnika_sie_ruszyla ==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ' && wsk_X[0][3]==' ')//dluga roszada
                        return true;
                    if(u.naX==6 && aktualny.czy_prawa_wierza_przeciwnika_sie_ruszyla==false && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')//krotka roszada
                        return true;
                if(kolor==1)//przeciwnik gra bialymi
                    if(u.naX==5 && aktualny.czy_prawa_wierza_przeciwnika_sie_ruszyla==false && wsk_X[0][4]==' ' && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')//dluga roszada
                        return true;
                    if(u.naX==1 && aktualny.czy_lewa_wierza_przeciwnika_sie_ruszyla ==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ')//krotka roszada
                        return true;
            return false;
    }
}
//********************************************************************************************
int T_silnik::wykonaj_posunieice(int posuniecie_urzytkownika)
{
    if(posuniecie_urzytkownika != 10000) //niewykona sie kiedy maszyna wykonuje pierwszy ruch
    {
        rozszyfruj_posuniecie_urzytkownika(posuniecie_urzytkownika);
        nanies_posuniecie_urzytkownika_na_szachownice();
    }
    aktualny.oblicz_nastepny_ruch(wsk_szachownica_robocza);
    znajdz_posuniecie_maszyny();
    nanies_posuniecie_maszyny_na_szachownice();
    int posuniecie_maszyny = zaszyfruj_posuniecie_maszyny();
    return posuniecie_maszyny + czy_doszlo_do_wygranej();
}
    void T_silnik::nanies_posuniecie_urzytkownika_na_szachownice()
{
    wsk_szachownica_robocza[u.naY][u.naX] = wsk_szachownica_robocza[u.zY][u.zX];
    wsk_szachownica_robocza[u.zY][u.zX]   = ' ';

    wsk_szachownica_wyjsciowa[u.naY][u.naX] = wsk_szachownica_wyjsciowa[u.zY][u.zX];
    wsk_szachownica_wyjsciowa[u.zY][u.zX]   = ' ';
}
    void T_silnik::znajdz_posuniecie_maszyny()
{
    for(int i=0; i<7; i++)
        for(int j=0; j<7; j++)
        {
            if(wsk_szachownica_robocza[i][j] == wsk_szachownica_wyjsciowa[i][j]) //pole na ktorym nic sie nie zmienilo
                continue;
            else if(wsk_szachownica_robocza[i][j] == ' ') //pole z ktorego ruszyla sie maszyna
            {
                m.zX = j;
                m.zY = i;
            }
            else //pole na ktore ruszyla sie maszyna
            {
                m.naX = j;
                m.naY = i;
            }
        }
}
    void T_silnik::nanies_posuniecie_maszyny_na_szachownice()
{
    wsk_szachownica_wyjsciowa[m.naY][m.naX] = wsk_szachownica_wyjsciowa[m.zY][m.zX];
    wsk_szachownica_wyjsciowa[m.zY][m.zX]   = ' ';
}
    int T_silnik::zaszyfruj_posuniecie_maszyny()
{
    return m.zX*1000+m.zY*100+m.naX*10+m.naY*1;
}
    int T_silnik::czy_doszlo_do_wygranej()
{
    if(aktualny.koniec_gry_wygrana_maszyny)
        return 20000; //wygrywa maszyna
    if(aktualny.koniec_gry_wygrana_urzytkownika)
        return 30000; //wygrywa urzytkownik
    return 0;
}


































