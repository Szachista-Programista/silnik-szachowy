#include <iostream>
#include <cmath>
#include "Silnik.h"
using namespace std;
using T_wsk_szachownica = char(*)[8];
extern bool kolor;


Silnik::Silnik(bool k): kolor{k}, aktualny{k}{
//==============================================================================================================
    wsk_szachownica_wyjsciowa = zainicjalizuj_szachownice();
    wsk_szachownica_robocza   = zainicjalizuj_szachownice();
}
    T_wsk_szachownica Silnik::zainicjalizuj_szachownice(){//@@@
//==============================================================================================================
    /*return new char[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                          {'p','p','p','p','p','p','p','p'},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {'P','P','P','P','P','P','P','P'},
                          {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};*/
    return new char[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                          {'p','p','p','p','p','p','p','p'},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {'P','P','P','P','P','P','P','P'},
                          {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};
    /*return new char[8][8]{{'k',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ','w',' ',' ',' ',' ',' '},
                          {' ','w',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {'K',' ',' ',' ',' ',' ',' ',' '}};*/
}
Silnik::~Silnik(){
//==============================================================================================================
    delete[]wsk_szachownica_wyjsciowa;
    delete[]wsk_szachownica_robocza;
}
bool Silnik::czy_urzytkownik_moze_wykonac_takie_posuniecie(int posuniecie_urzytkownika){
//==============================================================================================================
    rozszyfruj_posuniecie_urzytkownika(posuniecie_urzytkownika);
    if(!czy_na_polu_wyjsciowym_jest_bierka_urzytkownika()          )return false;
    if( czy_na_polu_docelowym_jest_bierka_urzytkownika()           )return false;
    if( czy_ten_ruch_powoduje_wystawienie_swojego_krola_na_bicie() )return false;
    if(!czy_ruch_jest_zgodny_z_zasadami()                          )return false;
    return true;
}
    void Silnik::rozszyfruj_posuniecie_urzytkownika(int posuniecie_urzytkownika){
//==============================================================================================================
    if(posuniecie_urzytkownika == 10000)
        u.naY = u.naX = u.zY  = u.zX  = kod_promocji = 0;
    else
    {
    u.naY = posuniecie_urzytkownika % 10;
    posuniecie_urzytkownika /=10;
    u.naX = posuniecie_urzytkownika % 10;
    posuniecie_urzytkownika /=10;
    u.zY  = posuniecie_urzytkownika % 10;
    posuniecie_urzytkownika /=10;
    u.zX  = posuniecie_urzytkownika % 10;
    posuniecie_urzytkownika /=10;
    kod_promocji = posuniecie_urzytkownika;
    }
}
    bool Silnik::czy_na_polu_wyjsciowym_jest_bierka_urzytkownika(){
//==============================================================================================================
    T_wsk_szachownica wsk_X = wsk_szachownica_robocza;
    if(wsk_X[u.zY][u.zX]<'g' && 'w'<wsk_X[u.zY][u.zX])
        return false;
    else
        return true;
}
    bool Silnik::czy_na_polu_docelowym_jest_bierka_urzytkownika(){
//==============================================================================================================
    T_wsk_szachownica wsk_X = wsk_szachownica_robocza;
    if(wsk_X[u.naY][u.naX]<'g' || 'w'<wsk_X[u.naY][u.naX])
        return false;
    else
        return true;
}
    bool Silnik::czy_ten_ruch_powoduje_wystawienie_swojego_krola_na_bicie(){
//==============================================================================================================
    T_wsk_szachownica wsk_X = wsk_szachownica_robocza;
    int potencjalnie_krol_urzytkownika_x;
    int potencjalnie_krol_urzytkownika_y;

    if(wsk_X[u.zY][u.zX]=='k') //czy urzytkownik chce sie ruszyc krolem
    {
        potencjalnie_krol_urzytkownika_x = u.naX;
        potencjalnie_krol_urzytkownika_y = u.naY;
    }
    else
    {
        potencjalnie_krol_urzytkownika_x = aktualny.polozenie_krola_urzytkownika_x;
        potencjalnie_krol_urzytkownika_y = aktualny.polozenie_krola_urzytkownika_y;
    }
    T_wsk_szachownica kopia = aktualny.skopiuj_szachownice(wsk_X);
    kopia[u.naY][u.naX]=kopia[u.zY][u.zX];
    kopia[u.zY][u.zX]=' ';
    if(aktualny.czy_pole_urzytkownika_jest_bite(potencjalnie_krol_urzytkownika_x, potencjalnie_krol_urzytkownika_y, kopia))
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
    bool Silnik::czy_ruch_jest_zgodny_z_zasadami(){
//==============================================================================================================
    T_wsk_szachownica wsk_X = wsk_szachownica_robocza;
    switch(wsk_X[u.zY][u.zX])
    {
        case 'p':
            if(u.zY==1 && u.naY-u.zY==2 && u.zX==u.naX && wsk_X[u.zY+1][u.naX]==' ' && wsk_X[u.naY][u.naX]==' ') return true;//ruch o 2 do przodu
            if(u.naY-u.zY==1 && u.zX==u.naX && wsk_X[u.naY][u.naX]==' ')                                         return true;//ruch o 1 do przodu
            if(u.naY-u.zY==1 && u.zX-u.naX==1 && 'G'<=wsk_X[u.naY][u.naX] && wsk_X[u.naY][u.naX]<='W')           return true;//bicie <<
            if(u.naY-u.zY==1 && u.naX-u.zX==1 && 'G'<=wsk_X[u.naY][u.naX] && wsk_X[u.naY][u.naX]<='W')           return true;//bicie >>
            if(wsk_X[m.naY][m.naX] == 'P' && m.zY-m.naY == 2 && m.naX == u.naX && u.zY == m.naY)
            {
                if(u.naY-u.zY==1 && u.zX-u.naX==1 && ' ' == wsk_X[u.naY][u.naX])           return true;//bicie w przelocie <<
                if(u.naY-u.zY==1 && u.naX-u.zX==1 && ' ' == wsk_X[u.naY][u.naX])           return true;//bicie w przelocie >>
            }
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
            if(0<=u.naY-1                ? wsk_X[u.naY-1][u.naX  ] != 'K': true)
            if(0<=u.naY-1 && u.naX+1<=7  ? wsk_X[u.naY-1][u.naX+1] != 'K': true)
            if(              u.naX+1<=7  ? wsk_X[u.naY  ][u.naX+1] != 'K': true)
            if(u.naY+1<=7 && u.naX+1<=7  ? wsk_X[u.naY+1][u.naX+1] != 'K': true)
            if(u.naY+1<=7                ? wsk_X[u.naY+1][u.naX  ] != 'K': true)
            if(u.naY+1<=7 && 0<=u.naX-1  ? wsk_X[u.naY+1][u.naX-1] != 'K': true)
            if(              0<=u.naX-1  ? wsk_X[u.naY  ][u.naX-1] != 'K': true)
            if(u.naY+1<=7 && 0<=u.naX-1  ? wsk_X[u.naY+1][u.naX-1] != 'K': true)
                return true;

            if(u.zY==u.naY && abs(u.zX - u.naX) == 2 && aktualny.czy_krol_urzytkownika_sie_ruszyl==false)//roszada
            {
                if(kolor==0)//urzytkownik gra czarnymi
                {
                    if(u.naX==2 && aktualny.czy_lewa_wierza_urzytkownika_sie_ruszyla ==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ' && wsk_X[0][3]==' ')//dluga roszada
                    if(!aktualny.czy_pole_urzytkownika_jest_bite(2, 0, wsk_X) && !aktualny.czy_pole_urzytkownika_jest_bite(3, 0, wsk_X) && !aktualny.czy_pole_urzytkownika_jest_bite(4, 0, wsk_X))
                        return true;
                    if(u.naX==6 && aktualny.czy_prawa_wierza_urzytkownika_sie_ruszyla==false && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')//krotka roszada
                    if(!aktualny.czy_pole_urzytkownika_jest_bite(4, 0, wsk_X) && !aktualny.czy_pole_urzytkownika_jest_bite(5, 0, wsk_X) && !aktualny.czy_pole_urzytkownika_jest_bite(6, 0, wsk_X))
                        return true;
                }
                if(kolor==1)//urzytkownik gra bialymi
                {
                    if(u.naX==5 && aktualny.czy_prawa_wierza_urzytkownika_sie_ruszyla==false && wsk_X[0][4]==' ' && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')//dluga roszada
                    if(!aktualny.czy_pole_urzytkownika_jest_bite(3, 0, wsk_X) && !aktualny.czy_pole_urzytkownika_jest_bite(4, 0, wsk_X) && !aktualny.czy_pole_urzytkownika_jest_bite(5, 0, wsk_X))
                        return true;
                    if(u.naX==1 && aktualny.czy_lewa_wierza_urzytkownika_sie_ruszyla ==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ')//krotka roszada
                    if(!aktualny.czy_pole_urzytkownika_jest_bite(1, 0, wsk_X) && !aktualny.czy_pole_urzytkownika_jest_bite(2, 0, wsk_X) && !aktualny.czy_pole_urzytkownika_jest_bite(3, 0, wsk_X))
                        return true;
                }
            }
            return false;
    }
}
int Silnik::wykonaj_posuniecie(int posuniecie_urzytkownika){
//==============================================================================================================
    przygotuj_maszyne_na_ruch(posuniecie_urzytkownika);
    switch(stan_gry)
    {
        case debiut:        wykonaj_ruch_debiutowy();      break;
        case gra_wlasciwa:  wykonaj_ruch_gry_srodkowej();  break;
        case koncowka:      wykonaj_koncowke();            break;
        default:                                           break;
    }
    return oblicz_posuniecie_maszyny();
}
    void Silnik::przygotuj_maszyne_na_ruch(int posuniecie_urzytkownika){
//==============================================================================================================
    rozszyfruj_posuniecie_urzytkownika(posuniecie_urzytkownika); // u.zY u.naY u.zX u.naX <<<<< 4456
    nanies_posuniecie_urzytkownika_na_szachownice(posuniecie_urzytkownika); // wsk_szachownica_wyjsciowa ===== wsk_szachownica_robocza <<<<< u.zY u.naY u.zX u.naX
    }
        void Silnik::nanies_posuniecie_urzytkownika_na_szachownice(int posuniecie_urzytkownika){
//==============================================================================================================
    if(posuniecie_urzytkownika == 10000)
        return;

    char ruszana_bierka = wsk_szachownica_robocza[u.zY][u.zX];

    if(ruszana_bierka == 'p' && u.zY == 4 && u.zX != u.naX && wsk_szachownica_robocza[u.naY][u.naX] == ' ') //bicie w przelocie
        wsk_szachownica_wyjsciowa[u.zY][u.naX]   = wsk_szachownica_robocza[u.zY][u.naX]   = ' ';
    wsk_szachownica_wyjsciowa[u.naY][u.naX] = wsk_szachownica_robocza[u.naY][u.naX] = wsk_szachownica_robocza[u.zY][u.zX];
    wsk_szachownica_wyjsciowa[u.zY][u.zX]   = wsk_szachownica_robocza[u.zY][u.zX]   = ' ';

    if(kod_promocji)
        switch(kod_promocji)
        {
            case 1: wsk_szachownica_wyjsciowa[u.naY][u.naX] = wsk_szachownica_robocza[u.naY][u.naX] = 's'; break;
            case 2: wsk_szachownica_wyjsciowa[u.naY][u.naX] = wsk_szachownica_robocza[u.naY][u.naX] = 'g'; break;
            case 3: wsk_szachownica_wyjsciowa[u.naY][u.naX] = wsk_szachownica_robocza[u.naY][u.naX] = 'w'; break;
            case 4: wsk_szachownica_wyjsciowa[u.naY][u.naX] = wsk_szachownica_robocza[u.naY][u.naX] = 'h'; break;
            default: break;
        }

    if(ruszana_bierka == 'k')//roszady byc moze
    {
        if(u.zX == 3 && u.naX == 1) // O-O bialymi
        {
            wsk_szachownica_wyjsciowa[0][0] = wsk_szachownica_robocza[0][0] = ' ';
            wsk_szachownica_wyjsciowa[0][2] = wsk_szachownica_robocza[0][2] = 'w';
        }
        if(u.zX == 3 && u.naX == 5) // O-O-O bialymi
        {
            wsk_szachownica_wyjsciowa[0][7] = wsk_szachownica_robocza[0][7] = ' ';
            wsk_szachownica_wyjsciowa[0][4] = wsk_szachownica_robocza[0][4] = 'w';
        }
        if(u.zX == 4 && u.naX == 2) // O-O-O czarnymi
        {
            wsk_szachownica_wyjsciowa[0][0] = wsk_szachownica_robocza[0][0] = ' ';
            wsk_szachownica_wyjsciowa[0][3] = wsk_szachownica_robocza[0][3] = 'w';
        }
        if(u.zX == 4 && u.naX == 6) // O-O czarnymi
        {
            wsk_szachownica_wyjsciowa[0][7] = wsk_szachownica_robocza[0][7] = ' ';
            wsk_szachownica_wyjsciowa[0][5] = wsk_szachownica_robocza[0][5] = 'w';
        }
    }
}
    void Silnik::wykonaj_ruch_debiutowy(){
//==============================================================================================================
    nr_ruchu_debiutowego++;
    switch(nr_ruchu_debiutowego)
    {
        case 1:
            if(kolor)
            {
                if(wsk_szachownica_robocza[3][2] == 'p' || wsk_szachownica_robocza[3][4] == 'p')
                {
                wsk_szachownica_robocza[6][4] = ' ';
                wsk_szachownica_robocza[4][4] = 'P';
                }
                else
                {
                wsk_szachownica_robocza[6][3] = ' ';
                wsk_szachownica_robocza[4][3] = 'P';
                }
            }
            else
            {

                wsk_szachownica_robocza[6][4] = ' ';
                wsk_szachownica_robocza[4][4] = 'P';
            }
            break;
        case 2:
            if(kolor)
            {
                if(aktualny.czy_moje_pole_jest_bite(3, 4, wsk_szachownica_robocza))
                {
                    wsk_szachownica_robocza[7][6] = ' ';
                    wsk_szachownica_robocza[5][5] = 'S';
                }
                else
                {
                    wsk_szachownica_robocza[7][1] = ' ';
                    wsk_szachownica_robocza[5][2] = 'S';
                }
            }
            else
            {
                if(aktualny.czy_moje_pole_jest_bite(4, 4, wsk_szachownica_robocza))
                {
                    wsk_szachownica_robocza[7][1] = ' ';
                    wsk_szachownica_robocza[5][2] = 'S';
                }
                else
                {
                    wsk_szachownica_robocza[7][6] = ' ';
                    wsk_szachownica_robocza[5][5] = 'S';
                }
            }
            break;
        case 3:
            if(wsk_szachownica_robocza[4][3] == 'p')
            {
                wsk_szachownica_robocza[5][5] = ' ';
                wsk_szachownica_robocza[4][3] = 'S';
            }
            else if(wsk_szachownica_robocza[4][4] == 'p')
            {
                wsk_szachownica_robocza[5][2] = ' ';
                wsk_szachownica_robocza[4][4] = 'S';
            }
            else
            {
                if(wsk_szachownica_robocza[7][1] == ' ')
                {
                    wsk_szachownica_robocza[7][6] = ' ';
                    wsk_szachownica_robocza[5][5] = 'S';
                }
                else
                {
                    wsk_szachownica_robocza[7][1] = ' ';
                    wsk_szachownica_robocza[5][2] = 'S';
                }
            }
            break;
        default:
            break;
    }
    if(nr_ruchu_debiutowego == 3)
        stan_gry = gra_wlasciwa;
}
    void Silnik::wykonaj_ruch_gry_srodkowej(){
//==============================================================================================================
    aktualny.przygotuj_Ruch();
    aktualny.oblicz_nastepny_ruch(wsk_szachownica_robocza);
    czy_czas_na_koncowke();
}
        void Silnik::czy_czas_na_koncowke(){
//==============================================================================================================
    bool czy_to_koncowka = true;
    if(wsk_szachownica_robocza != nullptr)
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<8; j++)
            {
                if('g' <= wsk_szachownica_robocza[i][j] && wsk_szachownica_robocza[i][j] <= 'w' && wsk_szachownica_robocza[i][j] != 'k')
                {
                    czy_to_koncowka = false;
                    break;
                }
            }
            if(czy_to_koncowka == false)
                break;
        }
    if(czy_to_koncowka == true)
        stan_gry = koncowka;
}
    void Silnik::wykonaj_koncowke(){
//==============================================================================================================

}
    int  Silnik::oblicz_posuniecie_maszyny(){
//==============================================================================================================
    znajdz_posuniecie_maszyny(); // wsk_szachownica_robocza wsk_szachownica_wyjsciowa >>>>> m.zY m.naY m.zX m.naX
    int posuniecie_maszyny = zaszyfruj_posuniecie_maszyny(); // m.zY m.naY m.zX m.naX >>>>> 1526
    nanies_posuniecie_maszyny_na_szachownice(); //          wsk_szachownica_wyjsciowa ===== wsk_szachownica_robocza
    return posuniecie_maszyny + czy_to_koniec_gry(); //zwraca ruch
}
        void Silnik::znajdz_posuniecie_maszyny(){
//==============================================================================================================
    if(wsk_szachownica_robocza == nullptr)
        return;
    bool czy_to_roszada = false;
    for(int i=0,x=0; i<8; i++) //czy doszlo do roszady
    {
        if(wsk_szachownica_robocza[7][i] != wsk_szachownica_wyjsciowa[7][i])
            x++;
        if(i==7 && x==4)
        czy_to_roszada = true;
    }
    if(czy_to_roszada == false)
    {
        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
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
    if(czy_to_roszada == true)
    {
        m.zY = m.naY = 7;
        if(kolor == 0)
        {
            m.zX = 4;
            if(wsk_szachownica_robocza[7][2] == 'K')
                m.naX = 2;
            else
                m.naX = 6;
        }
        if(kolor == 1)
        {
            m.zX = 3;
            if(wsk_szachownica_robocza[7][1] == 'K')
                m.naX = 1;
            else
                m.naX = 5;
        }
    }
}
        int  Silnik::zaszyfruj_posuniecie_maszyny(){
//==============================================================================================================
    if(wsk_szachownica_robocza == nullptr)
        return 0;
    int kod_wypromowanej_bierki = 0;
    if(wsk_szachownica_robocza[m.naY][m.naX] != wsk_szachownica_wyjsciowa[m.zY][m.zX])// doszlo do promocji pionka
        switch(wsk_szachownica_robocza[m.naY][m.naX])
        {
            case 'S': kod_wypromowanej_bierki = 10000; break;
            case 'G': kod_wypromowanej_bierki = 20000; break;
            case 'W': kod_wypromowanej_bierki = 30000; break;
            case 'H': kod_wypromowanej_bierki = 40000; break;
            default: break;
        }
    return m.zX*1000+m.zY*100+m.naX*10+m.naY*1 + kod_wypromowanej_bierki;
}
        void Silnik::nanies_posuniecie_maszyny_na_szachownice(){
//==============================================================================================================
    if(wsk_szachownica_robocza == nullptr)
        return;
    char ruszana_bierka;
    ruszana_bierka = wsk_szachownica_robocza[m.zY][m.zX];

    wsk_szachownica_wyjsciowa[m.naY][m.naX] = wsk_szachownica_robocza[m.naY][m.naX];
    wsk_szachownica_wyjsciowa[m.zY][m.zX]   = ' ';

    if(ruszana_bierka == 'K')//roszady byc moze
    {
        if(m.zX == 3 && m.naX == 1) // O-O czarnymi
        {
            wsk_szachownica_wyjsciowa[7][0] == ' ';
            wsk_szachownica_wyjsciowa[7][2] == 'W';
        }
        if(m.zX == 3 && m.naX == 5) // O-O-O czarnymi
        {
            wsk_szachownica_wyjsciowa[7][7] == ' ';
            wsk_szachownica_wyjsciowa[7][4] == 'W';
        }
        if(m.zX == 4 && m.naX == 2) // O-O-O bialymi
        {
            wsk_szachownica_wyjsciowa[7][0] == ' ';
            wsk_szachownica_wyjsciowa[7][3] == 'W';
        }
        if(m.zX == 4 && m.naX == 6) // O-O bialymi
        {
            wsk_szachownica_wyjsciowa[7][7] == ' ';
            wsk_szachownica_wyjsciowa[7][5] == 'W';
        }
    }
}
        int  Silnik::czy_to_koniec_gry(){
//==============================================================================================================
    if(aktualny.koniec_gry_wygrana_urzytkownika)
        return 60000; //wygrywa urzytkownik
    if(aktualny.koniec_gry_pat_urzytkownika)
        return 70000; //remis
    if(aktualny.koniec_gry_wygrana_maszyny)
        return 80000; //wygrywa maszyna
    if(aktualny.koniec_gry_pat_maszyny)
        return 90000; //remis

    return 0;
}

































