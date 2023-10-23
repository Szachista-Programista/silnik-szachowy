#include <iostream>
#include <vector>
#include <algorithm>
#include "T_ruch.h"
#include "T_silnik.h"//?????????
using namespace std;
using T_wsk_szachownica = char(*)[8];





T_ruch::T_ruch()
{
    //cout<<"tworze obiekt klasy T_ruch"<<endl;////!!!!!!!!!!!!!!!!
    pokolenie_klasy = 0;

    polozenie_mojego_krola_x      = kolor?3:4;
    polozenie_mojego_krola_y      = 7;
    polozenie_krola_przeciwnika_x = kolor?3:4;
    polozenie_krola_przeciwnika_y = 0;

    czy_moj_krol_sie_ruszyl                  = false;
    czy_moja_lewa_wierza_sie_ruszyla         = false;
    czy_moja_prawa_wierza_sie_ruszyla        = false;
    czy_krol_przeciwnika_sie_ruszyl          = false;
    czy_lewa_wierza_przeciwnika_sie_ruszyla  = false;
    czy_prawa_wierza_przeciwnika_sie_ruszyla = false;
}
double T_ruch::oblicz_nastepny_ruch(T_wsk_szachownica wsk_X)
{
    zweryfikuj_polozenia_krolow(wsk_X);
    sprawdz_czy_wieze_i_krole_byly_ruszane(wsk_X);
    wykonaj_moj_ruch(wsk_X);
    return znajdz_najleprze_z_posuniec(wsk_X);
}
    void T_ruch::zweryfikuj_polozenia_krolow(T_wsk_szachownica wsk_X)
{
    for(int i=7; i>=0; i--)
        for(int j=0; j<8; j++)
            if(wsk_X[i][j]=='K')
            {
                polozenie_mojego_krola_x=j;
                polozenie_mojego_krola_y=i;
                break;
            }
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            if(wsk_X[i][j]=='k')
            {
                polozenie_krola_przeciwnika_x=j;
                polozenie_krola_przeciwnika_y=i;
                break;
            }
}
    void T_ruch::sprawdz_czy_wieze_i_krole_byly_ruszane(T_wsk_szachownica wsk_X)
{
    if(wsk_X[7][0]!='W') czy_moja_lewa_wierza_sie_ruszyla =true;
    if(wsk_X[7][7]!='W') czy_moja_prawa_wierza_sie_ruszyla =true;
    if(wsk_X[7][0]!='w') czy_lewa_wierza_przeciwnika_sie_ruszyla  =true;
    if(wsk_X[7][7]!='w') czy_prawa_wierza_przeciwnika_sie_ruszyla =true;
    if(kolor? wsk_X[7][3]!='K': wsk_X[7][4]!='K') czy_moj_krol_sie_ruszyl         =true;
    if(kolor? wsk_X[0][3]!='k': wsk_X[0][4]!='k') czy_krol_przeciwnika_sie_ruszyl =true;
}
    double T_ruch::znajdz_najleprze_z_posuniec(T_wsk_szachownica wsk_X)
{
    if(pokolenie_klasy < ostatnie_pokolenie)
    {
        double najleprzy_z_najgorszych_status_materialny = -100;
        int index_najleprzego_z_najgorszych_ruchow;
        for(int i=0; i<ile_ruchow_rozpatrywac[pokolenie_klasy] && i<wektor_na_ruchy.size(); i++)
        {
            double najgorszy_status_malerialny_po_ruchu_przeciwnika = 100,
                   bierzacy_status_malerialny_przeciwnika;
            for(int j=1; j<=ile_ruchow_przeciwnika_rozpatrywac[pokolenie_klasy] && j<wektor_na_ruchy[i].size(); j++)
            {
                T_ruch(this, bierzacy_status_malerialny_przeciwnika, wektor_na_ruchy[i][j], pokolenie_klasy + 1);
                if(bierzacy_status_malerialny_przeciwnika < najgorszy_status_malerialny_po_ruchu_przeciwnika)
                    najgorszy_status_malerialny_po_ruchu_przeciwnika = bierzacy_status_malerialny_przeciwnika;
            }
            if(najleprzy_z_najgorszych_status_materialny < najgorszy_status_malerialny_po_ruchu_przeciwnika)
            {
                najleprzy_z_najgorszych_status_materialny = najgorszy_status_malerialny_po_ruchu_przeciwnika;
                index_najleprzego_z_najgorszych_ruchow = i;
            }
        }
        if(pokolenie_klasy == 0)
        {
            delete[] wsk_X;
            wsk_X = skopiuj_szachownice(wektor_na_ruchy[index_najleprzego_z_najgorszych_ruchow][0]);
        }
        wyzeruj_wektor_na_ruchy();
        return najleprzy_z_najgorszych_status_materialny;
    }

    if(pokolenie_klasy == ostatnie_pokolenie)
    {
        double najleprzy_z_najgorszych_status_materialny = -100;
        for(auto element: statusy_materialne_rozpatrywanych_rochow)
            if(najleprzy_z_najgorszych_status_materialny < element)
                najleprzy_z_najgorszych_status_materialny = element;
        wyzeruj_wektor_na_ruchy();
        return najleprzy_z_najgorszych_status_materialny;
    }
}
        void T_ruch::wyzeruj_wektor_na_ruchy()
{
    for(auto wektorek: wektor_na_ruchy)
    {
        for(auto element: wektorek)
            delete[]element;
    }
    wektor_na_ruchy.clear();
}
    T_ruch::T_ruch(T_ruch* wskaznik, double &status_materialny, T_wsk_szachownica wsk_X, int iteracja): pokolenie_klasy {iteracja}
{
    przepisz_dane_o_ruchach_krolow_i_wiez(wskaznik);
    status_materialny = oblicz_nastepny_ruch(wsk_X);
}
        void T_ruch::przepisz_dane_o_ruchach_krolow_i_wiez(T_ruch* wsk)
{
    czy_moj_krol_sie_ruszyl                  = wsk->czy_moj_krol_sie_ruszyl;
    czy_moja_lewa_wierza_sie_ruszyla         = wsk->czy_moja_lewa_wierza_sie_ruszyla;
    czy_moja_prawa_wierza_sie_ruszyla        = wsk->czy_moja_prawa_wierza_sie_ruszyla;
    czy_krol_przeciwnika_sie_ruszyl          = wsk->czy_krol_przeciwnika_sie_ruszyl;
    czy_lewa_wierza_przeciwnika_sie_ruszyla  = wsk->czy_lewa_wierza_przeciwnika_sie_ruszyla;
    czy_prawa_wierza_przeciwnika_sie_ruszyla = wsk->czy_prawa_wierza_przeciwnika_sie_ruszyla;
}
void T_ruch::wykonaj_moj_ruch(T_wsk_szachownica wsk_X)
{
    bool czy_moj_krol_jest_szachowany = czy_moje_pole_jest_bite(polozenie_mojego_krola_x, polozenie_mojego_krola_y, wsk_X);
    int numer_mojego_ruchu=-1,
        index_najgorszego_ruchu=0;
    double najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow = 100.0;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            switch(wsk_X[i][j])
            {
                case ' ': break;
                case 'P':                                                       //KWESTIA PIONKA
                {
                    if(1<i && i<7)                                                  //PIONEK BEZ SZANSY NA PROMOCJE
                    {
                        if(wsk_X[i-1][j]==' ')                                              //POSUNIECIE O 1 W PRZOD
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i-1][j]='P';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                        if(i==6 && wsk_X[5][j]==' ' && wsk_X[4][j]==' ')                    //POSUNIECIE O 2 W PRZOD
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[4][j]='P';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                        if(j!=0 && 'g'<=wsk_X[i-1][j-1] && wsk_X[i-1][j-1]<='w')                //BICIE W LEWO
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i-1][j-1]='P';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                        if(j!=7 && 'g'<=wsk_X[i-1][j+1] && wsk_X[i-1][j+1]<='w')                //BICIE W PRAWO
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i-1][j+1]='P';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    if(i==1)                                                       //PROMOCJA
                    {
                        if(wsk_X[0][j]==' ')                                                //PROMOCJA PO PROSTU
                        {
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM SKOCZKA
                            kopia[1][j]=' ';
                            kopia[0][j]='S';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GONCA
                            kopia[1][j]=' ';
                            kopia[0][j]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[1][j]=' ';
                            kopia[0][j]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[1][j]=' ';
                            kopia[0][j]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                        }
                        if(j!=0 && 'g'<=wsk_X[0][j-1] && wsk_X[0][j-1]<='w')                //PROMOCJA W LEWO
                        {
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM SKOCZKA
                            kopia[1][j]=' ';
                            kopia[0][j-1]='S';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GONCA
                            kopia[1][j]=' ';
                            kopia[0][j-1]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[1][j]=' ';
                            kopia[0][j-1]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[1][j]=' ';
                            kopia[0][j-1]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                        }
                        if(j!=7 && 'g'<=wsk_X[0][j+1] && wsk_X[0][j+1]<='w')                //PROMOCJA W PRAWO
                        {
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM SKOCZKA
                            kopia[1][j]=' ';
                            kopia[0][j+1]='S';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GOÑCA
                            kopia[1][j]=' ';
                            kopia[0][j+1]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[1][j]=' ';
                            kopia[0][j+1]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[1][j]=' ';
                            kopia[0][j+1]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            }
                        }
                    }
                    break;
                }
                case 'S':                                                       //KWESTIA SKOCZKA
                {
                    if(0<=i-2 && j+1<=7 && (wsk_X[i-2][j+1]<'G' || 'W'<wsk_X[i-2][j+1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-2][j+1]='S';
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                    }
                    if(0<=i-1 && j+2<=7 && (wsk_X[i-1][j+2]<'G' || 'W'<wsk_X[i-1][j+2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j+2]='S';
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                    }
                    if(i+1<=7 && j+2<=7 && (wsk_X[i+1][j+2]<'G' || 'W'<wsk_X[i+1][j+2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j+2]='S';
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                    }
                    if(i+2<=7 && j+1<=7 && (wsk_X[i+2][j+1]<'G' || 'W'<wsk_X[i+2][j+1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+2][j+1]='S';
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                    }
                    if(i+2<=7 && 0<=j-1 && (wsk_X[i+2][j-1]<'G' || 'W'<wsk_X[i+2][j-1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+2][j-1]='S';
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                    }
                    if(i+1<=7 && 0<=j-2 && (wsk_X[i+1][j-2]<'G' || 'W'<wsk_X[i+1][j-2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j-2]='S';
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                    }
                    if(0<=i-1 && 0<=j-2 && (wsk_X[i-1][j-2]<'G' || 'W'<wsk_X[i-1][j-2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j-2]='S';
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                    }
                    if(0<=i-2 && 0<=j-1 && (wsk_X[i-2][j-1]<'G' || 'W'<wsk_X[i-2][j-1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-2][j-1]='S';
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                    }
                    break;
                }
                case 'G':                                                       //KWESTIA GONCA
                {
                    for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //RUCH NA 1:30
                    {
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                            break;
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //RUCH NA 4:30
                    {
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                            break;
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //RUCH NA 7:30
                    {
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                            break;
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //RUCH NA 10:30
                    {
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                            break;
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    break;
                }
                case 'W':                                                       //KWESTIA WIEZY
                {
                    for(int y=i-1; 0<=y; y--)                                       //RUCH NA 12:00
                    {
                        if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')
                            break;
                        if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int x=j+1; x<=7; x++)                                       //RUCH NA 3:00
                    {
                        if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')
                            break;
                        if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int y=i+1; y<=7; y++)                                       //RUCH NA 6:00
                    {
                        if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')
                            break;
                        if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int x=j-1; 0<=x; x--)                                       //RUCH NA 9:00
                    {
                        if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')
                            break;
                        if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='W';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    break;
                }
                case 'H':                                                       //KWESTIA HETMANA
                {
                    for(int y=i-1; 0<=y; y--)                                       //RUCH NA 12:00
                    {
                        if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')
                            break;
                        if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //RUCH NA 1:30
                    {
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                            break;
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int x=j+1; x<=7; x++)                                       //RUCH NA 3:00
                    {
                        if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')
                            break;
                        if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //RUCH NA 4:30
                    {
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                            break;
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int y=i+1; y<=7; y++)                                       //RUCH NA 6:00
                    {
                        if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')
                            break;
                        if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //RUCH NA 7:30
                    {
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                            break;
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int x=j-1; 0<=x; x--)                                       //RUCH NA 9:00
                    {
                        if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')
                            break;
                        if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //RUCH NA 10:30
                    {
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                            break;
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        }
                    }
                    break;
                }
                case 'K':                                                       //KWESTIA KROLA
                {
                    if(0<=i-1 && (wsk_X[i-1][j]<'G' || 'W'<wsk_X[i-1][j]))                    //RUCH NA 12:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j]='K';
                        polozenie_mojego_krola_y--;
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        polozenie_mojego_krola_y++;
                    }
                    if(0<=i-1 && j+1<=7 && (wsk_X[i-1][j+1]<'G' || 'W'<wsk_X[i-1][j+1]))      //RUCH NA 1:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j+1]='K';
                        polozenie_mojego_krola_x++;
                        polozenie_mojego_krola_y--;
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        polozenie_mojego_krola_x--;
                        polozenie_mojego_krola_y++;
                    }
                    if(j+1<=7 && (wsk_X[i][j+1]<'G' || 'W'<wsk_X[i][j+1]))                    //RUCH NA 3:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i][j+1]='K';
                        polozenie_mojego_krola_x++;
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        polozenie_mojego_krola_x--;
                    }
                    if(i+1<=7 && j+1<=7 && (wsk_X[i+1][j+1]<'G' || 'W'<wsk_X[i+1][j+1]))      //RUCH NA 4:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j+1]='K';
                        polozenie_mojego_krola_x++;
                        polozenie_mojego_krola_y++;
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        polozenie_mojego_krola_x--;
                        polozenie_mojego_krola_y--;
                    }
                    if(i+1<=7 && (wsk_X[i+1][j]<'G' || 'W'<wsk_X[i+1][j]))                    //RUCH NA 6:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j]='K';
                        polozenie_mojego_krola_y++;
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        polozenie_mojego_krola_y--;
                    }
                    if(i+1<=7 && 0<=j-1 && (wsk_X[i+1][j-1]<'G' || 'W'<wsk_X[i+1][j-1]))      //RUCH NA 7:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j-1]='K';
                        if(czy_moje_pole_jest_bite(polozenie_mojego_krola_x-1, polozenie_mojego_krola_y+1, kopia)==false)
                        polozenie_mojego_krola_x--;
                        polozenie_mojego_krola_y++;
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        polozenie_mojego_krola_x++;
                        polozenie_mojego_krola_y--;
                    }
                    if(0<=j-1 && (wsk_X[i][j-1]<'G' || 'W'<wsk_X[i][j-1]))                    //RUCH NA 9:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i][j-1]='K';
                        polozenie_mojego_krola_x--;
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        polozenie_mojego_krola_x++;
                    }
                    if(0<=i-1 && 0<=j-1 && (wsk_X[i-1][j-1]<'G' || 'W'<wsk_X[i-1][j-1]))      //RUCH NA 10:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j-1]='K';
                        polozenie_mojego_krola_x--;
                        polozenie_mojego_krola_y--;
                        wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                        polozenie_mojego_krola_x++;
                        polozenie_mojego_krola_y++;
                    }
                    if(!kolor)                                                                      //ROSZADA KIEDY UZYTKOWNIK GRA CZARNYMI
                    {
                        if(czy_moj_krol_sie_ruszyl==false && czy_moja_lewa_wierza_sie_ruszyla==false && wsk_X[7][1]==' ' && wsk_X[7][2]==' ' && wsk_X[7][3]==' ')       //ROSZADA DLUGA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_moje_pole_jest_bite(2, 7, kopia)==false && czy_moje_pole_jest_bite(3, 7, kopia)==false && czy_moje_pole_jest_bite(4, 7, kopia)==false)
                            {
                                kopia[7][0]=' ';
                                kopia[7][4]=' ';
                                kopia[7][2]='K';
                                kopia[7][3]='W';
                                polozenie_mojego_krola_x-=2;
                                wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                                polozenie_mojego_krola_x+=2;
                            }
                            else delete[]kopia;
                        }
                        if(czy_moj_krol_sie_ruszyl==false && czy_moja_prawa_wierza_sie_ruszyla==false && wsk_X[7][5]==' ' && wsk_X[7][6]==' ')                          //ROSZADA KROTKA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_moje_pole_jest_bite(4, 7, kopia)==false && czy_moje_pole_jest_bite(5, 7, kopia)==false && czy_moje_pole_jest_bite(6, 7, kopia)==false)
                            {
                                kopia[7][7]=' ';
                                kopia[7][4]=' ';
                                kopia[7][6]='K';
                                kopia[7][5]='W';
                                polozenie_mojego_krola_x+=2;
                                wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                                polozenie_mojego_krola_x-=2;
                            }
                            else delete[]kopia;
                        }
                    }
                    if(kolor)                                                                       //ROSZADA KIEDY UZYTKOWNIK GRA BIALYMI
                    {
                        if(czy_moj_krol_sie_ruszyl==false && czy_moja_prawa_wierza_sie_ruszyla==false && wsk_X[7][4]==' ' && wsk_X[7][5]==' ' && wsk_X[7][6]==' ')       //ROSZADA DLUGA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_moje_pole_jest_bite(3, 7, kopia)==false && czy_moje_pole_jest_bite(4, 7, kopia)==false && czy_moje_pole_jest_bite(5, 7, kopia)==false)
                            {
                                kopia[7][7]=' ';
                                kopia[7][3]=' ';
                                kopia[7][5]='K';
                                kopia[7][4]='W';
                                polozenie_mojego_krola_x+=2;
                                wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                                polozenie_mojego_krola_x-=2;
                            }
                            else delete[]kopia;
                        }
                        if(czy_moj_krol_sie_ruszyl==false && czy_moja_lewa_wierza_sie_ruszyla==false && wsk_X[7][1]==' ' && wsk_X[7][2]==' ')                          //ROSZADA KROTKA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_moje_pole_jest_bite(1, 7, kopia)==false && czy_moje_pole_jest_bite(2, 7, kopia)==false && czy_moje_pole_jest_bite(3, 7, kopia)==false)
                            {
                                kopia[7][0]=' ';
                                kopia[7][3]=' ';
                                kopia[7][1]='K';
                                kopia[7][2]='W';
                                polozenie_mojego_krola_x-=2;
                                wykonaj_moj_ruch_jak_mozna(czy_moj_krol_jest_szachowany, numer_mojego_ruchu, i, j, kopia, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
                                polozenie_mojego_krola_x+=2;
                            }
                            else delete[]kopia;
                        }
                    }
                    break;
                }
                default: break;
            }
        }
}
    void T_ruch::wykonaj_moj_ruch_jak_mozna(bool &czy_moj_krol_jest_szachowany, int &numer_mojego_ruchu, int &i, int &j, T_wsk_szachownica kopia, int &index_najgorszego_ruchu, double &najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow)
{
    if(czy_moj_krol_jest_szachowany==false && j!=polozenie_mojego_krola_x && i!=polozenie_mojego_krola_y && polozenie_mojego_krola_x+polozenie_mojego_krola_y!=j+i && polozenie_mojego_krola_x-polozenie_mojego_krola_y!=j-i)
    {
        numer_mojego_ruchu++;
        wektor_na_ruchy.push_back(vector<T_wsk_szachownica>());
        wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
        wykonaj_ruch_przeciwnika(kopia, numer_mojego_ruchu);
        odrzuc_najgorszy_ruch(numer_mojego_ruchu, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
    }
    else if(czy_moje_pole_jest_bite(polozenie_mojego_krola_x, polozenie_mojego_krola_y, kopia)==false)
    {
        numer_mojego_ruchu++;
        wektor_na_ruchy.push_back(vector<T_wsk_szachownica>());
        wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
        wykonaj_ruch_przeciwnika(kopia, numer_mojego_ruchu);
        odrzuc_najgorszy_ruch(numer_mojego_ruchu, index_najgorszego_ruchu, najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
    }
    else delete[]kopia;
}
        bool T_ruch::czy_moje_pole_jest_bite(const int &x,const int &y, T_wsk_szachownica kopia)
{

    for(int i=1; 0<=y-i; i++)                                       //12:00 //BITE PRZEZ HETMANA / WIEZE / GONCA
    {
        if(kopia[y-i][x]==' ') continue;
        if(kopia[y-i][x]=='h' || kopia[y-i][x]=='w') return true;
        else break;
    }
    for(int i=1; 0<=y-i && x+i<=7; i++)                             //1:30
    {
        if(kopia[y-i][x+i]==' ') continue;
        if(kopia[y-i][x+i]=='h' || kopia[y-i][x+i]=='g') return true;
        else break;
    }
    for(int i=1; x+i<=7; i++)                                       //3:00
    {
        if(kopia[y][x+i]==' ') continue;
        if(kopia[y][x+i]=='h' || kopia[y][x+i]=='w') return true;
        else break;
    }
    for(int i=1; y+i<=7 && x+i<=7; i++)                             //4:30
    {
        if(kopia[y+i][x+i]==' ') continue;
        if(kopia[y+i][x+i]=='h' || kopia[y+i][x+i]=='g') return true;
        else break;
    }
    for(int i=1; y+i<=7; i++)                                       //6:00
    {
        if(kopia[y+i][x]==' ') continue;
        if(kopia[y+i][x]=='h' || kopia[y+i][x]=='w') return true;
        else break;
    }
    for(int i=1; y+i<=7 && 0<=x-i; i++)                             //7:30
    {
        if(kopia[y+i][x-i]==' ') continue;
        if(kopia[y+i][x-i]=='h' || kopia[y+i][x-i]=='g') return true;
        else break;
    }
    for(int i=1; 0<=x-i; i++)                                       //9:00
    {
        if(kopia[y][x-i]==' ') continue;
        if(kopia[y][x-i]=='h' || kopia[y][x-i]=='w') return true;
        else break;
    }
    for(int i=1; 0<=y-i && 0<=x-i; i++)                             //10:30
    {
        if(kopia[y-i][x-i]==' ') continue;
        if(kopia[y-i][x-i]=='h' || kopia[y-i][x-i]=='g') return true;
        else break;
    }

    if(0<=y-2 && x+1<=7 && kopia[y-2][x+1]=='s') return true;                       //BITE PRZEZ SKOCZKA
    if(0<=y-1 && x+2<=7 && kopia[y-1][x+2]=='s') return true;
    if(y+1<=7 && x+2<=7 && kopia[y+1][x+2]=='s') return true;
    if(y+2<=7 && x+1<=7 && kopia[y+2][x+1]=='s') return true;
    if(y+2<=7 && 0<=x-1 && kopia[y+2][x-1]=='s') return true;
    if(y+1<=7 && 0<=x-2 && kopia[y+1][x-2]=='s') return true;
    if(0<=y-1 && 0<=x-2 && kopia[y-1][x-2]=='s') return true;
    if(0<=y-2 && 0<=x-1 && kopia[y-2][x-1]=='s') return true;

    if(2<=y)                                                                    //BITE PRZEZ PIONKA
    {
        if(1<=x && kopia[y-1][x-1]=='p') return true;
        if(x<=6 && kopia[y-1][x+1]=='p') return true;
    }
    return false;

}
        void T_ruch::odrzuc_najgorszy_ruch(int &numer_mojego_ruchu, int &index_najgorszego_ruchu, double &najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow)
{

    double najmniejszy_status_materialny_dla_danego_ruchu = 100.0;                                      //WYWALAMY NA BIERZACO NAJGORSZY RUCH
    for(int i=1; i < wektor_na_ruchy[numer_mojego_ruchu].size(); i++)
    {
        najmniejszy_status_materialny_dla_danego_ruchu = min(najmniejszy_status_materialny_dla_danego_ruchu, oblicz_status_materialny(wektor_na_ruchy[numer_mojego_ruchu][i]));
    }
    if(ile_ruchow_rozpatrywac[pokolenie_klasy] < wektor_na_ruchy.size())
        if(najmniejszy_status_materialny_dla_danego_ruchu <= najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow)
        {
            for(auto element: wektor_na_ruchy[numer_mojego_ruchu])
                delete[]element;
            wektor_na_ruchy.pop_back();
            numer_mojego_ruchu--;
        }
        else
        {
            statusy_materialne_rozpatrywanych_rochow[index_najgorszego_ruchu] = najmniejszy_status_materialny_dla_danego_ruchu;
            for(auto element: wektor_na_ruchy[index_najgorszego_ruchu])
                delete[]element;
            wektor_na_ruchy[index_najgorszego_ruchu] = wektor_na_ruchy[numer_mojego_ruchu];
            wektor_na_ruchy.pop_back();
            numer_mojego_ruchu--;
            index_najgorszego_ruchu = 0;
            for(int i=0; i<ile_ruchow_rozpatrywac[pokolenie_klasy]; i++)
                if(statusy_materialne_rozpatrywanych_rochow[i] < statusy_materialne_rozpatrywanych_rochow[index_najgorszego_ruchu])
                    index_najgorszego_ruchu = i;
        }
    else
    {
        if(najmniejszy_status_materialny_dla_danego_ruchu < najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow)
        {
            najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow = najmniejszy_status_materialny_dla_danego_ruchu;
            index_najgorszego_ruchu = numer_mojego_ruchu;
        }
        //statusy_materialne_rozpatrywanych_rochow[numer_mojego_ruchu] = najmniejszy_status_materialny_dla_danego_ruchu;
        statusy_materialne_rozpatrywanych_rochow.push_back(najmniejszy_status_materialny_dla_danego_ruchu);
    }
}
void T_ruch::wykonaj_ruch_przeciwnika(T_wsk_szachownica wsk_X, int &numer_mojego_ruchu)
{
    bool czy_krol_przeciwnika_jest_szachowany = czy_pole_przeciwnika_jest_bite(polozenie_krola_przeciwnika_x, polozenie_krola_przeciwnika_y, wsk_X);
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            switch(wsk_X[i][j])
            {
                case ' ': break;
                case 'p':                                                       //KWESTIA PIONKA
                {
                    if(0<i && i<6)                                                  //PIONEK BEZ SZANSY NA PROMOCJE
                    {
                        if(wsk_X[i+1][j]==' ')                                              //POSUNIECIE O 1 W PRZOD
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i+1][j]='p';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                        if(i==1 && wsk_X[2][j]==' ' && wsk_X[3][j]==' ')                    //POSUNIECIE O 2 W PRZOD
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[1][j]=' ';
                            kopia[3][j]='p';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                        if(j!=0 && 'G'<=wsk_X[i+1][j-1] && wsk_X[i+1][j-1]<='W')                //BICIE W LEWO
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i+1][j-1]='p';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                        if(j!=7 && 'G'<=wsk_X[i+1][j+1] && wsk_X[i+1][j+1]<='W')                //BICIE W PRAWO
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i+1][j+1]='p';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    if(i==6)                                                       //PROMOCJA
                    {
                        if(wsk_X[7][j]==' ')                                                //PROMOCJA PO PROSTU
                        {
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM SKOCZKA
                            kopia[6][j]=' ';
                            kopia[7][j]='s';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GONCA
                            kopia[6][j]=' ';
                            kopia[7][j]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[6][j]=' ';
                            kopia[7][j]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[6][j]=' ';
                            kopia[7][j]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                        }
                        if(j!=0 && 'G'<=wsk_X[7][j-1] && wsk_X[7][j-1]<='W')                //PROMOCJA W LEWO
                        {
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM SKOCZKA
                            kopia[6][j]=' ';
                            kopia[7][j-1]='s';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GOÑCA
                            kopia[6][j]=' ';
                            kopia[7][j-1]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[6][j]=' ';
                            kopia[7][j-1]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[6][j]=' ';
                            kopia[7][j-1]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                        }
                        if(j!=7 && 'G'<=wsk_X[7][j+1] && wsk_X[7][j+1]<='W')                //PROMOCJA W PRAWO
                        {
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM SKOCZKA
                            kopia[6][j]=' ';
                            kopia[7][j+1]='s';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GONCA
                            kopia[6][j]=' ';
                            kopia[7][j+1]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[6][j]=' ';
                            kopia[7][j+1]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[6][j]=' ';
                            kopia[7][j+1]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            }
                        }
                    }
                    break;
                }
                case 's':                                                       //KWESTIA SKOCZKA
                {
                    if(0<=i-2 && j+1<=7 && (wsk_X[i-2][j+1]<'g' || 'w'<wsk_X[i-2][j+1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-2][j+1]='s';
                        wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                    }
                    if(0<=i-1 && j+2<=7 && (wsk_X[i-1][j+2]<'g' || 'w'<wsk_X[i-1][j+2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j+2]='s';
                        wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                    }
                    if(i+1<=7 && j+2<=7 && (wsk_X[i+1][j+2]<'g' || 'w'<wsk_X[i+1][j+2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j+2]='s';
                        wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                    }
                    if(i+2<=7 && j+1<=7 && (wsk_X[i+2][j+1]<'g' || 'w'<wsk_X[i+2][j+1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+2][j+1]='s';
                        wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                    }
                    if(i+2<=7 && 0<=j-1 && (wsk_X[i+2][j-1]<'g' || 'w'<wsk_X[i+2][j-1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+2][j-1]='s';
                        wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                    }
                    if(i+1<=7 && 0<=j-2 && (wsk_X[i+1][j-2]<'g' || 'w'<wsk_X[i+1][j-2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j-2]='s';
                        wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                    }
                    if(0<=i-1 && 0<=j-2 && (wsk_X[i-1][j-2]<'g' || 'w'<wsk_X[i-1][j-2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j-2]='s';
                        wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                    }
                    if(0<=i-2 && 0<=j-1 && (wsk_X[i-2][j-1]<'g' || 'w'<wsk_X[i-2][j-1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-2][j-1]='s';
                        wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                    }
                    break;
                }
                case 'g':                                                       //KWESTIA GONCA
                {
                    for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //RUCH NA 1:30
                    {
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                            break;
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //RUCH NA 4:30
                    {
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                            break;
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //RUCH NA 7:30
                    {
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                            break;
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //RUCH NA 10:30
                    {
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                            break;
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    break;
                }
                case 'w':                                                       //KWESTIA WIEZY
                {
                    for(int y=i-1; 0<=y; y--)                                       //RUCH NA 12:00
                    {
                        if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')
                            break;
                        if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int x=j+1; x<=7; x++)                                       //RUCH NA 3:00
                    {
                        if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')
                            break;
                        if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int y=i+1; y<=7; y++)                                       //RUCH NA 6:00
                    {
                        if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')
                            break;
                        if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int x=j-1; 0<=x; x--)                                       //RUCH NA 9:00
                    {
                        if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')
                            break;
                        if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='w';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    break;
                }
                case 'h':                                                       //KWESTIA HETMANA
                {
                    for(int y=i-1; 0<=y; y--)                                       //RUCH NA 12:00
                    {
                        if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')
                            break;
                        if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int y=i-1, x=j+1; 0<=y && x<=7; y--, x++)                   //RUCH NA 1:30
                    {
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                            break;
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int x=j+1; x<=7; x++)                                       //RUCH NA 3:00
                    {
                        if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')
                            break;
                        if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int y=i+1, x=j+1; y<=7 && x<=7; y++, x++)                   //RUCH NA 4:30
                    {
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                            break;
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int y=i+1; y<=7; y++)                                       //RUCH NA 6:00
                    {
                        if('g'<=wsk_X[y][j] && wsk_X[y][j]<='w')
                            break;
                        if('G'<=wsk_X[y][j] && wsk_X[y][j]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int y=i+1, x=j-1; y<=7 && 0<=x; y++, x--)                   //RUCH NA 7:30
                    {
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                            break;
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int x=j-1; 0<=x; x--)                                       //RUCH NA 9:00
                    {
                        if('g'<=wsk_X[i][x] && wsk_X[i][x]<='w')
                            break;
                        if('G'<=wsk_X[i][x] && wsk_X[i][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    for(int y=i-1, x=j-1; 0<=y && 0<=x; y--, x--)                   //RUCH NA 10:30
                    {
                        if('g'<=wsk_X[y][x] && wsk_X[y][x]<='w')
                            break;
                        if('G'<=wsk_X[y][x] && wsk_X[y][x]<='W')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_przeciwnika_jak_mozna(czy_krol_przeciwnika_jest_szachowany, numer_mojego_ruchu, i, j, kopia);
                        }
                    }
                    break;
                }
                case 'k':                                                       //KWESTIA KROLA
                {
                    if(0<=i-1 && (wsk_X[i-1][j]<'g' || 'w'<wsk_X[i-1][j]))                    //RUCH NA 12:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j]='k';
                        if(czy_pole_przeciwnika_jest_bite(polozenie_krola_przeciwnika_x, polozenie_krola_przeciwnika_y-1, kopia)==false)
                        {
                            polozenie_krola_przeciwnika_y--;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_przeciwnika_y++;
                        }
                        else delete[]kopia;
                    }
                    if(0<=i-1 && j+1<=7 && (wsk_X[i-1][j+1]<'g' || 'w'<wsk_X[i-1][j+1]))      //RUCH NA 1:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j+1]='k';
                        if(czy_pole_przeciwnika_jest_bite(polozenie_krola_przeciwnika_x+1, polozenie_krola_przeciwnika_y-1, kopia)==false)
                        {
                            polozenie_krola_przeciwnika_x++;
                            polozenie_krola_przeciwnika_y--;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_przeciwnika_x--;
                            polozenie_krola_przeciwnika_y++;
                        }
                        else delete[]kopia;
                    }
                    if(j+1<=7 && (wsk_X[i][j+1]<'g' || 'w'<wsk_X[i][j+1]))                    //RUCH NA 3:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i][j+1]='k';
                        if(czy_pole_przeciwnika_jest_bite(polozenie_krola_przeciwnika_x+1, polozenie_krola_przeciwnika_y, kopia)==false)
                        {
                            polozenie_krola_przeciwnika_x++;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_przeciwnika_x--;
                        }
                        else delete[]kopia;
                    }
                    if(i+1<=7 && j+1<=7 && (wsk_X[i+1][j+1]<'g' || 'w'<wsk_X[i+1][j+1]))      //RUCH NA 4:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j+1]='k';
                        if(czy_pole_przeciwnika_jest_bite(polozenie_krola_przeciwnika_x+1, polozenie_krola_przeciwnika_y+1, kopia)==false)
                        {
                            polozenie_krola_przeciwnika_x++;
                            polozenie_krola_przeciwnika_y++;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_przeciwnika_x--;
                            polozenie_krola_przeciwnika_y--;
                        }
                        else delete[]kopia;
                    }
                    if(i+1<=7 && (wsk_X[i+1][j]<'g' || 'w'<wsk_X[i+1][j]))                    //RUCH NA 6:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j]='k';
                        if(czy_pole_przeciwnika_jest_bite(polozenie_krola_przeciwnika_x, polozenie_krola_przeciwnika_y+1, kopia)==false)
                        {
                            polozenie_krola_przeciwnika_y++;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_przeciwnika_y--;
                        }
                        else delete[]kopia;
                    }
                    if(i+1<=7 && 0<=j-1 && (wsk_X[i+1][j-1]<'g' || 'w'<wsk_X[i+1][j-1]))      //RUCH NA 7:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j-1]='k';
                        if(czy_pole_przeciwnika_jest_bite(polozenie_krola_przeciwnika_x-1, polozenie_krola_przeciwnika_y+1, kopia)==false)
                        {
                            polozenie_krola_przeciwnika_x--;
                            polozenie_krola_przeciwnika_y++;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_przeciwnika_x++;
                            polozenie_krola_przeciwnika_y--;
                        }
                        else delete[]kopia;
                    }
                    if(0<=j-1 && (wsk_X[i][j-1]<'g' || 'w'<wsk_X[i][j-1]))                    //RUCH NA 9:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i][j-1]='k';
                        if(czy_pole_przeciwnika_jest_bite(polozenie_krola_przeciwnika_x-1, polozenie_krola_przeciwnika_y, kopia)==false)
                        {
                            polozenie_krola_przeciwnika_x--;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_przeciwnika_x++;
                        }
                        else delete[]kopia;
                    }
                    if(0<=i-1 && 0<=j-1 && (wsk_X[i-1][j-1]<'g' || 'w'<wsk_X[i-1][j-1]))      //RUCH NA 10:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_przeciwnika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_przeciwnika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j-1]='k';
                        if(czy_pole_przeciwnika_jest_bite(polozenie_krola_przeciwnika_x-1, polozenie_krola_przeciwnika_y-1, kopia)==false)
                        {
                            polozenie_krola_przeciwnika_x--;
                            polozenie_krola_przeciwnika_y--;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_przeciwnika_x++;
                            polozenie_krola_przeciwnika_y++;
                        }
                        else delete[]kopia;
                    }
                    if(!kolor)                                                                      //ROSZADA KIEDY UZYTKOWNIK GRA CZARNYMI
                    {
                        if(czy_krol_przeciwnika_sie_ruszyl==false && czy_lewa_wierza_przeciwnika_sie_ruszyla==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ' && wsk_X[0][3]==' ')       //ROSZADA DLUGA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_pole_przeciwnika_jest_bite(2, 0, kopia)==false && czy_pole_przeciwnika_jest_bite(3, 0, kopia)==false && czy_pole_przeciwnika_jest_bite(4, 0, kopia)==false)
                            {
                                kopia[0][0]=' ';
                                kopia[0][4]=' ';
                                kopia[0][2]='k';
                                kopia[0][3]='w';
                                polozenie_krola_przeciwnika_x-=2;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_przeciwnika_x+=2;
                            }
                            else delete[]kopia;
                        }
                        if(czy_krol_przeciwnika_sie_ruszyl==false && czy_prawa_wierza_przeciwnika_sie_ruszyla==false && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')                          //ROSZADA KROTKA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_pole_przeciwnika_jest_bite(4, 0, kopia)==false && czy_pole_przeciwnika_jest_bite(5, 0, kopia)==false && czy_pole_przeciwnika_jest_bite(6, 0, kopia)==false)
                            {
                                kopia[0][7]=' ';
                                kopia[0][4]=' ';
                                kopia[0][6]='k';
                                kopia[0][5]='w';
                                polozenie_krola_przeciwnika_x+=2;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_przeciwnika_x-=2;
                            }
                            else delete[]kopia;
                        }
                    }
                    if(kolor)                                                                       //ROSZADA KIEDY UZYTKOWNIK GRA BIALYMI
                    {
                        if(czy_krol_przeciwnika_sie_ruszyl==false && czy_prawa_wierza_przeciwnika_sie_ruszyla==false && wsk_X[0][4]==' ' && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')       //ROSZADA DLUGA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_pole_przeciwnika_jest_bite(3, 0, kopia)==false && czy_pole_przeciwnika_jest_bite(4, 0, kopia)==false && czy_pole_przeciwnika_jest_bite(5, 0, kopia)==false)
                            {
                                kopia[0][7]=' ';
                                kopia[0][3]=' ';
                                kopia[0][5]='k';
                                kopia[0][4]='w';
                                polozenie_krola_przeciwnika_x+=2;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_przeciwnika_x-=2;
                            }
                            else delete[]kopia;
                        }
                        if(czy_krol_przeciwnika_sie_ruszyl==false && czy_lewa_wierza_przeciwnika_sie_ruszyla==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ')                          //ROSZADA KROTKA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_pole_przeciwnika_jest_bite(1, 0, kopia)==false && czy_pole_przeciwnika_jest_bite(2, 0, kopia)==false && czy_pole_przeciwnika_jest_bite(3, 0, kopia)==false)
                            {
                                kopia[0][0]=' ';
                                kopia[0][3]=' ';
                                kopia[0][1]='k';
                                kopia[0][2]='w';
                                polozenie_krola_przeciwnika_x-=2;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_przeciwnika_x+=2;
                            }
                            else delete[]kopia;
                        }
                    }
                    break;
                }
                default: break;
            }
        }
    odrzuc_najgorsze_ruchy_przeciwnika(numer_mojego_ruchu);
}
    void T_ruch::wykonaj_ruch_przeciwnika_jak_mozna(bool &czy_krol_przeciwnika_jest_szachowany, int &numer_mojego_ruchu, int &i, int &j, T_wsk_szachownica kopia)
{
    if(czy_krol_przeciwnika_jest_szachowany==false && j!=polozenie_krola_przeciwnika_x && i!=polozenie_krola_przeciwnika_y && polozenie_krola_przeciwnika_x+polozenie_krola_przeciwnika_y!=j+i && polozenie_krola_przeciwnika_x-polozenie_krola_przeciwnika_y!=j-i)
        wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
    else if(czy_pole_przeciwnika_jest_bite(polozenie_krola_przeciwnika_x, polozenie_krola_przeciwnika_y, kopia)==false)
        wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
    else delete[]kopia;
}
        bool T_ruch::czy_pole_przeciwnika_jest_bite(const int &x, const int &y, T_wsk_szachownica kopia)
{
    for(int i=1; 0<=y-i; i++)                                       //12:00 //BITE PRZEZ HETMANA / WIEZE / GONCA
    {
        if(kopia[y-i][x]==' ') continue;
        if(kopia[y-i][x]=='H' || kopia[y-i][x]=='W') return true;
        else break;
    }
    for(int i=1; 0<=y-i && x+i<=7; i++)                             //1:30
    {
        if(kopia[y-i][x+i]==' ') continue;
        if(kopia[y-i][x+i]=='H' || kopia[y-i][x+i]=='G') return true;
        else break;
    }
    for(int i=1; x+i<=7; i++)                                       //3:00
    {
        if(kopia[y][x+i]==' ') continue;
        if(kopia[y][x+i]=='H' || kopia[y][x+i]=='W') return true;
        else break;
    }
    for(int i=1; y+i<=7 && x+i<=7; i++)                             //4:30
    {
        if(kopia[y+i][x+i]==' ') continue;
        if(kopia[y+i][x+i]=='H' || kopia[y+i][x+i]=='G') return true;
        else break;
    }
    for(int i=1; y+i<=7; i++)                                       //6:00
    {
        if(kopia[y+i][x]==' ') continue;
        if(kopia[y+i][x]=='H' || kopia[y+i][x]=='W') return true;
        else break;
    }
    for(int i=1; y+i<=7 && 0<=x-i; i++)                             //7:30
    {
        if(kopia[y+i][x-i]==' ') continue;
        if(kopia[y+i][x-i]=='H' || kopia[y+i][x-i]=='G') return true;
        else break;
    }
    for(int i=1; 0<=x-i; i++)                                       //9:00
    {
        if(kopia[y][x-i]==' ') continue;
        if(kopia[y][x-i]=='H' || kopia[y][x-i]=='W') return true;
        else break;
    }
    for(int i=1; 0<=y-i && 0<=x-i; i++)                             //10:30
    {
        if(kopia[y-i][x-i]==' ') continue;
        if(kopia[y-i][x-i]=='H' || kopia[y-i][x-i]=='G') return true;
        else break;
    }

    if(0<=y-2 && x+1<=7 && kopia[y-2][x+1]=='S') return true;                       //BITE PRZEZ SKOCZKA
    if(0<=y-1 && x+2<=7 && kopia[y-1][x+2]=='S') return true;
    if(y+1<=7 && x+2<=7 && kopia[y+1][x+2]=='S') return true;
    if(y+2<=7 && x+1<=7 && kopia[y+2][x+1]=='S') return true;
    if(y+2<=7 && 0<=x-1 && kopia[y+2][x-1]=='S') return true;
    if(y+1<=7 && 0<=x-2 && kopia[y+1][x-2]=='S') return true;
    if(0<=y-1 && 0<=x-2 && kopia[y-1][x-2]=='S') return true;
    if(0<=y-2 && 0<=x-1 && kopia[y-2][x-1]=='S') return true;

    if(y<=5)                                                                    //BITE PRZEZ PIONKA
    {
        if(1<=x && kopia[y+1][x-1]=='P') return true;
        if(x<=6 && kopia[y+1][x+1]=='P') return true;
    }
    return false;
}
        void T_ruch::odrzuc_najgorsze_ruchy_przeciwnika(int &numer_mojego_ruchu)
{
    int liczba_ruchow_przeciwnika = wektor_na_ruchy[numer_mojego_ruchu].size()-1;
    vector<double>status_materialny;
    for(int i=0; i<liczba_ruchow_przeciwnika; i++)                                                  //obliczamy statusy materialne przeciwnika
        status_materialny.push_back(oblicz_status_materialny(wektor_na_ruchy[numer_mojego_ruchu][i+1]));
    int index_komorki_z_najwieksza_wartoscia;


    while(liczba_ruchow_przeciwnika > ile_ruchow_przeciwnika_rozpatrywac[pokolenie_klasy])
    {
        index_komorki_z_najwieksza_wartoscia = 0;
        for(int i=1; i<liczba_ruchow_przeciwnika; i++)
        {
            if(status_materialny[index_komorki_z_najwieksza_wartoscia] < status_materialny[i])
                index_komorki_z_najwieksza_wartoscia = i;
        }
        status_materialny.erase(status_materialny.begin() + index_komorki_z_najwieksza_wartoscia);
        delete[]wektor_na_ruchy[numer_mojego_ruchu][index_komorki_z_najwieksza_wartoscia+1];
        wektor_na_ruchy[numer_mojego_ruchu].erase(wektor_na_ruchy[numer_mojego_ruchu].begin() + index_komorki_z_najwieksza_wartoscia + 1);
        liczba_ruchow_przeciwnika--;
    }
}
double T_ruch::oblicz_status_materialny(const T_wsk_szachownica wsk_X)
{
    double status_materialny{0};
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            switch(wsk_X[i][j])
            {
                case 'p': status_materialny += (i==6)? -2.0 : -1.0; break;
                case 's': status_materialny += (i==0 || i==7 || j==0 || j==7)? -2.9 : -3.0; break;
                case 'g': status_materialny += (i==0 || i==7 || j==0 || j==7)? -2.9 : -3.0; break;
                case 'w': status_materialny += (wsk_X[1][j]=='P' || wsk_X[2][j]=='P' || wsk_X[3][j]=='P' || wsk_X[4][j]=='P' || wsk_X[5][j]=='P' || wsk_X[6][j]=='P')? -4.9 : -5.0; break;
                case 'h': status_materialny += (wsk_X[1][j]=='P' || wsk_X[2][j]=='P' || wsk_X[3][j]=='P' || wsk_X[4][j]=='P' || wsk_X[5][j]=='P' || wsk_X[6][j]=='P')? -6.9 : -7.0; break;

                case 'P': status_materialny += (i==1)? 2.0 : 1.0; break;
                case 'S': status_materialny += (i==0 || i==7 || j==0 || j==7)? 2.9 : 3.0; break;
                case 'G': status_materialny += (i==0 || i==7 || j==0 || j==7)? 2.9 : 3.0; break;
                case 'W': status_materialny += (wsk_X[1][j]=='p' || wsk_X[2][j]=='p' || wsk_X[3][j]=='p' || wsk_X[4][j]=='p' || wsk_X[5][j]=='p' || wsk_X[6][j]=='p')? 4.9 : 5.0; break;
                case 'H': status_materialny += (wsk_X[1][j]=='p' || wsk_X[2][j]=='p' || wsk_X[3][j]=='p' || wsk_X[4][j]=='p' || wsk_X[5][j]=='p' || wsk_X[6][j]=='p')? 6.9 : 7.0; break;
                default: break;
            }
        }
    return status_materialny;
}
T_wsk_szachownica T_ruch::skopiuj_szachownice(T_wsk_szachownica oryginal)
{
    T_wsk_szachownica kopia = new char[8][8];
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            kopia[i][j] = oryginal[i][j];
    return kopia;
}
void T_ruch::wypisz_szachownice(const T_wsk_szachownica wsk_X)
{
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
            cout<<wsk_X[i][j];
        cout<<endl;
    }
}













