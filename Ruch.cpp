#include <iostream>
#include <vector>
#include <algorithm>
#include "Ruch.h"
using namespace std;
using T_wsk_szachownica = char(*)[8];
extern bool kolor;

Ruch::Ruch(bool k): kolor{k}{
//==============================================================================================================
    pokolenie_klasy = 0;

    polozenie_mojego_krola_x      = kolor?3:4;
    polozenie_mojego_krola_y      =         7;
    polozenie_krola_urzytkownika_x = kolor?3:4;
    polozenie_krola_urzytkownika_y =         0;

    czy_moj_krol_sie_ruszyl                  = false;
    czy_moja_lewa_wierza_sie_ruszyla         = false;
    czy_moja_prawa_wierza_sie_ruszyla        = false;
    czy_krol_urzytkownika_sie_ruszyl          = false;
    czy_lewa_wierza_urzytkownika_sie_ruszyla  = false;
    czy_prawa_wierza_urzytkownika_sie_ruszyla = false;
}
Ruch::~Ruch(){
//==============================================================================================================
    wyzeruj_wektor_na_ruchy();
}
void Ruch::przygotuj_Ruch(){
//==============================================================================================================
    statusy_materialne_rozpatrywanych_rochow.clear();
    numer_mojego_ruchu       = -1 ;
    index_najgorszego_ruchu  =  0 ;
    najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow = 100.0;
}
double Ruch::oblicz_nastepny_ruch(T_wsk_szachownica &wsk_X){
//==============================================================================================================
    zweryfikuj_polozenia_krolow(wsk_X);
    sprawdz_czy_wieze_i_krole_byly_ruszane(wsk_X);
    sprawdz_moj_krol_jest_szachowany(wsk_X);
    wykonaj_moj_ruch(wsk_X);
    return znajdz_najleprze_z_posuniec(wsk_X);
}
    void Ruch::zweryfikuj_polozenia_krolow(T_wsk_szachownica wsk_X){
//==============================================================================================================
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
                polozenie_krola_urzytkownika_x=j;
                polozenie_krola_urzytkownika_y=i;
                break;
            }
}
    void Ruch::sprawdz_czy_wieze_i_krole_byly_ruszane(T_wsk_szachownica wsk_X){
//==============================================================================================================
    if(wsk_X[7][0]!='W') czy_moja_lewa_wierza_sie_ruszyla =true;
    if(wsk_X[7][7]!='W') czy_moja_prawa_wierza_sie_ruszyla =true;
    if(wsk_X[7][0]!='w') czy_lewa_wierza_urzytkownika_sie_ruszyla  =true;
    if(wsk_X[7][7]!='w') czy_prawa_wierza_urzytkownika_sie_ruszyla =true;
    if(kolor? wsk_X[7][3]!='K': wsk_X[7][4]!='K') czy_moj_krol_sie_ruszyl         =true;
    if(kolor? wsk_X[0][3]!='k': wsk_X[0][4]!='k') czy_krol_urzytkownika_sie_ruszyl =true;
}
    void Ruch::sprawdz_moj_krol_jest_szachowany(T_wsk_szachownica wsk_X){
//==============================================================================================================
    czy_moj_krol_jest_szachowany = czy_moje_pole_jest_bite(polozenie_mojego_krola_x, polozenie_mojego_krola_y, wsk_X);
}
    double Ruch::znajdz_najleprze_z_posuniec(T_wsk_szachownica &wsk_X){
//==============================================================================================================
    double najleprzy_z_najgorszych_status_materialny = -100;
    if(pokolenie_klasy < ostatnie_pokolenie)
    {
        int index_najleprzego_z_najgorszych_ruchow;
        for(int i=0; i<wektor_na_ruchy.size() && i<ile_ruchow_rozpatrywac[pokolenie_klasy]; i++)
        {
            double najgorszy_status_malerialny_po_ruchu_urzytkownika = 100,
                   bierzacy_status_malerialny_urzytkownika;
            for(int j=1; j<wektor_na_ruchy[i].size() && j<=ile_ruchow_urzytkownika_rozpatrywac[pokolenie_klasy]; j++)
            {
                Ruch(this, bierzacy_status_malerialny_urzytkownika, wektor_na_ruchy[i][j], pokolenie_klasy + 1);
                if(bierzacy_status_malerialny_urzytkownika < najgorszy_status_malerialny_po_ruchu_urzytkownika)
                    najgorszy_status_malerialny_po_ruchu_urzytkownika = bierzacy_status_malerialny_urzytkownika;
            }
            if(najleprzy_z_najgorszych_status_materialny < najgorszy_status_malerialny_po_ruchu_urzytkownika)
            {
                najleprzy_z_najgorszych_status_materialny = najgorszy_status_malerialny_po_ruchu_urzytkownika;
                index_najleprzego_z_najgorszych_ruchow = i;
            }
        }
        if(pokolenie_klasy == 0)
        {
            if(wektor_na_ruchy.size()==0) //czy urzytkownik zamatowal maszyne albo wywolal pata?
            {
                if(czy_moje_pole_jest_bite(polozenie_mojego_krola_x, polozenie_mojego_krola_y, wsk_X))
                    koniec_gry_wygrana_urzytkownika = true;
                else
                    koniec_gry_pat_urzytkownika     = true;
            }
            delete[]wsk_X;
            wsk_X=nullptr;
            for(int i=0; i<wektor_na_ruchy.size(); i++)
            {
                if(wektor_na_ruchy[i].size()==1) // czy maszyna zamatowala urzytkowanika albo wywolala pata?
                {
                    wsk_X = skopiuj_szachownice(wektor_na_ruchy[i][0]);
                    if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x, polozenie_krola_urzytkownika_y, wsk_X))
                        koniec_gry_wygrana_maszyny = true;
                    else
                        koniec_gry_pat_maszyny     = true;
                    break;
                }
            }
            if(wsk_X == nullptr)
                wsk_X = skopiuj_szachownice(wektor_na_ruchy[index_najleprzego_z_najgorszych_ruchow][0]);
        }
        wyzeruj_wektor_na_ruchy();
        return najleprzy_z_najgorszych_status_materialny;
    }
    if(pokolenie_klasy == ostatnie_pokolenie)// koniec drzewa poszukiwan
    {
        for(auto element: statusy_materialne_rozpatrywanych_rochow)
            if(najleprzy_z_najgorszych_status_materialny < element)
                najleprzy_z_najgorszych_status_materialny = element;
        wyzeruj_wektor_na_ruchy();
        return najleprzy_z_najgorszych_status_materialny;
    }
}
        void Ruch::wyzeruj_wektor_na_ruchy(){
//==============================================================================================================
    for(auto wektorek: wektor_na_ruchy)
    {
        for(auto element: wektorek)
            delete[]element;
    }
    wektor_na_ruchy.clear();
}
        Ruch::Ruch(Ruch* wskaznik, double &status_materialny, T_wsk_szachownica wsk_X, int iteracja): pokolenie_klasy {iteracja}{
//==============================================================================================================
    przepisz_dane_o_ruchach_krolow_i_wiez(wskaznik);
    status_materialny = oblicz_nastepny_ruch(wsk_X);
}
            void Ruch::przepisz_dane_o_ruchach_krolow_i_wiez(Ruch* wsk){
//==============================================================================================================
    czy_moj_krol_sie_ruszyl                  = wsk->czy_moj_krol_sie_ruszyl;
    czy_moja_lewa_wierza_sie_ruszyla         = wsk->czy_moja_lewa_wierza_sie_ruszyla;
    czy_moja_prawa_wierza_sie_ruszyla        = wsk->czy_moja_prawa_wierza_sie_ruszyla;
    czy_krol_urzytkownika_sie_ruszyl          = wsk->czy_krol_urzytkownika_sie_ruszyl;
    czy_lewa_wierza_urzytkownika_sie_ruszyla  = wsk->czy_lewa_wierza_urzytkownika_sie_ruszyla;
    czy_prawa_wierza_urzytkownika_sie_ruszyla = wsk->czy_prawa_wierza_urzytkownika_sie_ruszyla;
}
void Ruch::wykonaj_moj_ruch(T_wsk_szachownica wsk_X){
//==============================================================================================================
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        }
                        if(i==6 && wsk_X[5][j]==' ' && wsk_X[4][j]==' ')                    //POSUNIECIE O 2 W PRZOD
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[4][j]='P';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        }
                        if(j!=0 && 'g'<=wsk_X[i-1][j-1] && wsk_X[i-1][j-1]<='w')                //BICIE W LEWO
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i-1][j-1]='P';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        }
                        if(j!=7 && 'g'<=wsk_X[i-1][j+1] && wsk_X[i-1][j+1]<='w')                //BICIE W PRAWO
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i-1][j+1]='P';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GONCA
                            kopia[1][j]=' ';
                            kopia[0][j]='G';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[1][j]=' ';
                            kopia[0][j]='W';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[1][j]=' ';
                            kopia[0][j]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                        }
                        if(j!=0 && 'g'<=wsk_X[0][j-1] && wsk_X[0][j-1]<='w')                //PROMOCJA W LEWO
                        {
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM SKOCZKA
                            kopia[1][j]=' ';
                            kopia[0][j-1]='S';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GONCA
                            kopia[1][j]=' ';
                            kopia[0][j-1]='G';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[1][j]=' ';
                            kopia[0][j-1]='W';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[1][j]=' ';
                            kopia[0][j-1]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                        }
                        if(j!=7 && 'g'<=wsk_X[0][j+1] && wsk_X[0][j+1]<='w')                //PROMOCJA W PRAWO
                        {
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM SKOCZKA
                            kopia[1][j]=' ';
                            kopia[0][j+1]='S';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GOÑCA
                            kopia[1][j]=' ';
                            kopia[0][j+1]='G';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[1][j]=' ';
                            kopia[0][j+1]='W';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[1][j]=' ';
                            kopia[0][j+1]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                    }
                    if(0<=i-1 && j+2<=7 && (wsk_X[i-1][j+2]<'G' || 'W'<wsk_X[i-1][j+2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j+2]='S';
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                    }
                    if(i+1<=7 && j+2<=7 && (wsk_X[i+1][j+2]<'G' || 'W'<wsk_X[i+1][j+2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j+2]='S';
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                    }
                    if(i+2<=7 && j+1<=7 && (wsk_X[i+2][j+1]<'G' || 'W'<wsk_X[i+2][j+1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+2][j+1]='S';
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                    }
                    if(i+2<=7 && 0<=j-1 && (wsk_X[i+2][j-1]<'G' || 'W'<wsk_X[i+2][j-1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+2][j-1]='S';
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                    }
                    if(i+1<=7 && 0<=j-2 && (wsk_X[i+1][j-2]<'G' || 'W'<wsk_X[i+1][j-2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j-2]='S';
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                    }
                    if(0<=i-1 && 0<=j-2 && (wsk_X[i-1][j-2]<'G' || 'W'<wsk_X[i-1][j-2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j-2]='S';
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                    }
                    if(0<=i-2 && 0<=j-1 && (wsk_X[i-2][j-1]<'G' || 'W'<wsk_X[i-2][j-1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-2][j-1]='S';
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='G';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='W';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='W';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='W';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='W';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='H';
                            wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        }
                    }
                    break;
                }
                case 'K':                                                       //KWESTIA KROLA
                {
                    if(0<=i-1 && (wsk_X[i-1][j]<'G' || 'W'<wsk_X[i-1][j]))                    //RUCH NA 12:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j]='K';
                        polozenie_mojego_krola_y--;
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        polozenie_mojego_krola_y++;
                    }
                    if(0<=i-1 && j+1<=7 && (wsk_X[i-1][j+1]<'G' || 'W'<wsk_X[i-1][j+1]))      //RUCH NA 1:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j+1]='K';
                        polozenie_mojego_krola_x++;
                        polozenie_mojego_krola_y--;
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        polozenie_mojego_krola_x--;
                        polozenie_mojego_krola_y++;
                    }
                    if(j+1<=7 && (wsk_X[i][j+1]<'G' || 'W'<wsk_X[i][j+1]))                    //RUCH NA 3:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i][j+1]='K';
                        polozenie_mojego_krola_x++;
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        polozenie_mojego_krola_x--;
                    }
                    if(i+1<=7 && j+1<=7 && (wsk_X[i+1][j+1]<'G' || 'W'<wsk_X[i+1][j+1]))      //RUCH NA 4:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j+1]='K';
                        polozenie_mojego_krola_x++;
                        polozenie_mojego_krola_y++;
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        polozenie_mojego_krola_x--;
                        polozenie_mojego_krola_y--;
                    }
                    if(i+1<=7 && (wsk_X[i+1][j]<'G' || 'W'<wsk_X[i+1][j]))                    //RUCH NA 6:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j]='K';
                        polozenie_mojego_krola_y++;
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        polozenie_mojego_krola_y--;
                    }
                    if(i+1<=7 && 0<=j-1 && (wsk_X[i+1][j-1]<'G' || 'W'<wsk_X[i+1][j-1]))      //RUCH NA 7:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j-1]='K';
                        if(czy_moje_pole_jest_bite(polozenie_mojego_krola_x-1, polozenie_mojego_krola_y+1, kopia)==false)
                        polozenie_mojego_krola_x--;
                        polozenie_mojego_krola_y++;
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        polozenie_mojego_krola_x++;
                        polozenie_mojego_krola_y--;
                    }
                    if(0<=j-1 && (wsk_X[i][j-1]<'G' || 'W'<wsk_X[i][j-1]))                    //RUCH NA 9:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i][j-1]='K';
                        polozenie_mojego_krola_x--;
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
                        polozenie_mojego_krola_x++;
                    }
                    if(0<=i-1 && 0<=j-1 && (wsk_X[i-1][j-1]<'G' || 'W'<wsk_X[i-1][j-1]))      //RUCH NA 10:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j-1]='K';
                        polozenie_mojego_krola_x--;
                        polozenie_mojego_krola_y--;
                        wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                                wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                                wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                                wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
                                wykonaj_moj_ruch_jak_mozna(i, j, kopia);
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
    void Ruch::wykonaj_moj_ruch_jak_mozna(int &i, int &j, T_wsk_szachownica kopia){
//==============================================================================================================
    if(czy_moj_krol_jest_szachowany==false && j!=polozenie_mojego_krola_x && i!=polozenie_mojego_krola_y && polozenie_mojego_krola_x+polozenie_mojego_krola_y!=j+i && polozenie_mojego_krola_x-polozenie_mojego_krola_y!=j-i)
    {
        numer_mojego_ruchu++;
        wektor_na_ruchy.push_back(vector<T_wsk_szachownica>());
        wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
        wykonaj_ruch_urzytkownika(kopia);
        odrzuc_najgorszy_ruch();
    }
    else if(czy_moje_pole_jest_bite(polozenie_mojego_krola_x, polozenie_mojego_krola_y, kopia)==false)
    {
        numer_mojego_ruchu++;
        wektor_na_ruchy.push_back(vector<T_wsk_szachownica>());
        wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
        wykonaj_ruch_urzytkownika(kopia);
        odrzuc_najgorszy_ruch();
    }
    else delete[]kopia; //jesli nie mozna to uwalniamy pamiec
}
        bool Ruch::czy_moje_pole_jest_bite(const int &x,const int &y, T_wsk_szachownica kopia){
//==============================================================================================================

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
        void Ruch::odrzuc_najgorszy_ruch(){
//==============================================================================================================
    double najmniejszy_status_materialny_dla_danego_ruchu = 100.0;                                      //WYWALAMY NA BIEZACO NAJGORSZY RUCH
    for(int i=1; i < wektor_na_ruchy[numer_mojego_ruchu].size(); i++) //znajdujemy najkorzyskniejszy dla urzytkownika po jego odpowiedzi
    {
        najmniejszy_status_materialny_dla_danego_ruchu = min(najmniejszy_status_materialny_dla_danego_ruchu, oblicz_status_materialny(wektor_na_ruchy[numer_mojego_ruchu][i]));
    }
    if(ile_ruchow_rozpatrywac[pokolenie_klasy] >= wektor_na_ruchy.size()) //sytuacja, w ktorej ilosc obliczonych ruchow nieprzekracza limitu
    {
        if(najmniejszy_status_materialny_dla_danego_ruchu < najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow)
        {
            najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow = najmniejszy_status_materialny_dla_danego_ruchu;
            index_najgorszego_ruchu = numer_mojego_ruchu;
        }
        statusy_materialne_rozpatrywanych_rochow.push_back(najmniejszy_status_materialny_dla_danego_ruchu);
    }
    else  //sytuacja, w ktorej ilosc obliczonych ruchow przekracza limit i trzeba odrzucic najgorszy ruch
    {
        if(najmniejszy_status_materialny_dla_danego_ruchu <= najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow)//jesli odrzucamy bieżacy ruch
        {
            for(auto element: wektor_na_ruchy[numer_mojego_ruchu])
                delete[]element;
            wektor_na_ruchy.pop_back();
            numer_mojego_ruchu--;
        }
        else//jesli odrzucamy ruch ktorys z poprzednich
        {
            statusy_materialne_rozpatrywanych_rochow[index_najgorszego_ruchu] = najmniejszy_status_materialny_dla_danego_ruchu;
            for(auto element: wektor_na_ruchy[index_najgorszego_ruchu])
                delete[]element;
            wektor_na_ruchy[index_najgorszego_ruchu] = wektor_na_ruchy[numer_mojego_ruchu];
            wektor_na_ruchy.pop_back();
            numer_mojego_ruchu--;
            index_najgorszego_ruchu = 0;// od poczatku szukamu ktory ruch jest potencjalnie najgorszym
            for(int i=0; i<ile_ruchow_rozpatrywac[pokolenie_klasy]; i++)
                if(statusy_materialne_rozpatrywanych_rochow[i] < statusy_materialne_rozpatrywanych_rochow[index_najgorszego_ruchu])
                    index_najgorszego_ruchu = i;
        }
    }
}
void Ruch::wykonaj_ruch_urzytkownika(T_wsk_szachownica wsk_X){
//==============================================================================================================
    czy_krol_urzytkownika_jest_szachowany = czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x, polozenie_krola_urzytkownika_y, wsk_X);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                        }
                        if(i==1 && wsk_X[2][j]==' ' && wsk_X[3][j]==' ')                    //POSUNIECIE O 2 W PRZOD
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[1][j]=' ';
                            kopia[3][j]='p';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                        }
                        if(j!=0 && 'G'<=wsk_X[i+1][j-1] && wsk_X[i+1][j-1]<='W')                //BICIE W LEWO
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i+1][j-1]='p';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                        }
                        if(j!=7 && 'G'<=wsk_X[i+1][j+1] && wsk_X[i+1][j+1]<='W')                //BICIE W PRAWO
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i+1][j+1]='p';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GONCA
                            kopia[6][j]=' ';
                            kopia[7][j]='g';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[6][j]=' ';
                            kopia[7][j]='w';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[6][j]=' ';
                            kopia[7][j]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                        }
                        if(j!=0 && 'G'<=wsk_X[7][j-1] && wsk_X[7][j-1]<='W')                //PROMOCJA W LEWO
                        {
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM SKOCZKA
                            kopia[6][j]=' ';
                            kopia[7][j-1]='s';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GOÑCA
                            kopia[6][j]=' ';
                            kopia[7][j-1]='g';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[6][j]=' ';
                            kopia[7][j-1]='w';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[6][j]=' ';
                            kopia[7][j-1]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                        }
                        if(j!=7 && 'G'<=wsk_X[7][j+1] && wsk_X[7][j+1]<='W')                //PROMOCJA W PRAWO
                        {
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM SKOCZKA
                            kopia[6][j]=' ';
                            kopia[7][j+1]='s';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM GONCA
                            kopia[6][j]=' ';
                            kopia[7][j+1]='g';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM WIERZY
                            kopia[6][j]=' ';
                            kopia[7][j+1]='w';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            }
                            {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);               //Z UTWORZENIEM HETMANA
                            kopia[6][j]=' ';
                            kopia[7][j+1]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                        wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                    }
                    if(0<=i-1 && j+2<=7 && (wsk_X[i-1][j+2]<'g' || 'w'<wsk_X[i-1][j+2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j+2]='s';
                        wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                    }
                    if(i+1<=7 && j+2<=7 && (wsk_X[i+1][j+2]<'g' || 'w'<wsk_X[i+1][j+2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j+2]='s';
                        wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                    }
                    if(i+2<=7 && j+1<=7 && (wsk_X[i+2][j+1]<'g' || 'w'<wsk_X[i+2][j+1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+2][j+1]='s';
                        wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                    }
                    if(i+2<=7 && 0<=j-1 && (wsk_X[i+2][j-1]<'g' || 'w'<wsk_X[i+2][j-1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+2][j-1]='s';
                        wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                    }
                    if(i+1<=7 && 0<=j-2 && (wsk_X[i+1][j-2]<'g' || 'w'<wsk_X[i+1][j-2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j-2]='s';
                        wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                    }
                    if(0<=i-1 && 0<=j-2 && (wsk_X[i-1][j-2]<'g' || 'w'<wsk_X[i-1][j-2]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j-2]='s';
                        wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                    }
                    if(0<=i-2 && 0<=j-1 && (wsk_X[i-2][j-1]<'g' || 'w'<wsk_X[i-2][j-1]))
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-2][j-1]='s';
                        wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='g';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='w';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='w';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='w';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='w';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][j]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][j]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[i][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[i][x]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
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
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                            break;
                        }
                        if(wsk_X[y][x]==' ')
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            kopia[i][j]=' ';
                            kopia[y][x]='h';
                            wykonaj_ruch_urzytkownika_jak_mozna(i, j, kopia);
                        }
                    }
                    break;
                }
                case 'k':                                                       //KWESTIA KROLA
                {
                    if(0<=i-1 && (wsk_X[i-1][j]<'g' || 'w'<wsk_X[i-1][j]))                    //RUCH NA 12:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j]='k';
                        if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x, polozenie_krola_urzytkownika_y-1, kopia)==false)
                        {
                            polozenie_krola_urzytkownika_y--;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_urzytkownika_y++;
                        }
                        else delete[]kopia;
                    }
                    if(0<=i-1 && j+1<=7 && (wsk_X[i-1][j+1]<'g' || 'w'<wsk_X[i-1][j+1]))      //RUCH NA 1:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j+1]='k';
                        if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x+1, polozenie_krola_urzytkownika_y-1, kopia)==false)
                        {
                            polozenie_krola_urzytkownika_x++;
                            polozenie_krola_urzytkownika_y--;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_urzytkownika_x--;
                            polozenie_krola_urzytkownika_y++;
                        }
                        else delete[]kopia;
                    }
                    if(j+1<=7 && (wsk_X[i][j+1]<'g' || 'w'<wsk_X[i][j+1]))                    //RUCH NA 3:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i][j+1]='k';
                        if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x+1, polozenie_krola_urzytkownika_y, kopia)==false)
                        {
                            polozenie_krola_urzytkownika_x++;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_urzytkownika_x--;
                        }
                        else delete[]kopia;
                    }
                    if(i+1<=7 && j+1<=7 && (wsk_X[i+1][j+1]<'g' || 'w'<wsk_X[i+1][j+1]))      //RUCH NA 4:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j+1]='k';
                        if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x+1, polozenie_krola_urzytkownika_y+1, kopia)==false)
                        {
                            polozenie_krola_urzytkownika_x++;
                            polozenie_krola_urzytkownika_y++;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_urzytkownika_x--;
                            polozenie_krola_urzytkownika_y--;
                        }
                        else delete[]kopia;
                    }
                    if(i+1<=7 && (wsk_X[i+1][j]<'g' || 'w'<wsk_X[i+1][j]))                    //RUCH NA 6:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j]='k';
                        if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x, polozenie_krola_urzytkownika_y+1, kopia)==false)
                        {
                            polozenie_krola_urzytkownika_y++;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_urzytkownika_y--;
                        }
                        else delete[]kopia;
                    }
                    if(i+1<=7 && 0<=j-1 && (wsk_X[i+1][j-1]<'g' || 'w'<wsk_X[i+1][j-1]))      //RUCH NA 7:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i+1][j-1]='k';
                        if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x-1, polozenie_krola_urzytkownika_y+1, kopia)==false)
                        {
                            polozenie_krola_urzytkownika_x--;
                            polozenie_krola_urzytkownika_y++;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_urzytkownika_x++;
                            polozenie_krola_urzytkownika_y--;
                        }
                        else delete[]kopia;
                    }
                    if(0<=j-1 && (wsk_X[i][j-1]<'g' || 'w'<wsk_X[i][j-1]))                    //RUCH NA 9:00
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i][j-1]='k';
                        if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x-1, polozenie_krola_urzytkownika_y, kopia)==false)
                        {
                            polozenie_krola_urzytkownika_x--;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_urzytkownika_x++;
                        }
                        else delete[]kopia;
                    }
                    if(0<=i-1 && 0<=j-1 && (wsk_X[i-1][j-1]<'g' || 'w'<wsk_X[i-1][j-1]))      //RUCH NA 10:30
                    if(2<=abs(polozenie_mojego_krola_x-polozenie_krola_urzytkownika_x) || 2<=abs(polozenie_mojego_krola_y-polozenie_krola_urzytkownika_y)) //czy krol do krola sie nie przytula
                    {
                        T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                        kopia[i][j]=' ';
                        kopia[i-1][j-1]='k';
                        if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x-1, polozenie_krola_urzytkownika_y-1, kopia)==false)
                        {
                            polozenie_krola_urzytkownika_x--;
                            polozenie_krola_urzytkownika_y--;
                            wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                            polozenie_krola_urzytkownika_x++;
                            polozenie_krola_urzytkownika_y++;
                        }
                        else delete[]kopia;
                    }
                    if(!kolor)                                                                      //ROSZADA KIEDY UZYTKOWNIK GRA CZARNYMI
                    {
                        if(czy_krol_urzytkownika_sie_ruszyl==false && czy_lewa_wierza_urzytkownika_sie_ruszyla==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ' && wsk_X[0][3]==' ')       //ROSZADA DLUGA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_pole_urzytkownika_jest_bite(2, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(3, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(4, 0, kopia)==false)
                            {
                                kopia[0][0]=' ';
                                kopia[0][4]=' ';
                                kopia[0][2]='k';
                                kopia[0][3]='w';
                                polozenie_krola_urzytkownika_x-=2;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_x+=2;
                            }
                            else delete[]kopia;
                        }
                        if(czy_krol_urzytkownika_sie_ruszyl==false && czy_prawa_wierza_urzytkownika_sie_ruszyla==false && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')                          //ROSZADA KROTKA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_pole_urzytkownika_jest_bite(4, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(5, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(6, 0, kopia)==false)
                            {
                                kopia[0][7]=' ';
                                kopia[0][4]=' ';
                                kopia[0][6]='k';
                                kopia[0][5]='w';
                                polozenie_krola_urzytkownika_x+=2;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_x-=2;
                            }
                            else delete[]kopia;
                        }
                    }
                    if(kolor)                                                                       //ROSZADA KIEDY UZYTKOWNIK GRA BIALYMI
                    {
                        if(czy_krol_urzytkownika_sie_ruszyl==false && czy_prawa_wierza_urzytkownika_sie_ruszyla==false && wsk_X[0][4]==' ' && wsk_X[0][5]==' ' && wsk_X[0][6]==' ')       //ROSZADA DLUGA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_pole_urzytkownika_jest_bite(3, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(4, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(5, 0, kopia)==false)
                            {
                                kopia[0][7]=' ';
                                kopia[0][3]=' ';
                                kopia[0][5]='k';
                                kopia[0][4]='w';
                                polozenie_krola_urzytkownika_x+=2;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_x-=2;
                            }
                            else delete[]kopia;
                        }
                        if(czy_krol_urzytkownika_sie_ruszyl==false && czy_lewa_wierza_urzytkownika_sie_ruszyla==false && wsk_X[0][1]==' ' && wsk_X[0][2]==' ')                          //ROSZADA KROTKA
                        {
                            T_wsk_szachownica kopia = skopiuj_szachownice(wsk_X);
                            if(czy_pole_urzytkownika_jest_bite(1, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(2, 0, kopia)==false && czy_pole_urzytkownika_jest_bite(3, 0, kopia)==false)
                            {
                                kopia[0][0]=' ';
                                kopia[0][3]=' ';
                                kopia[0][1]='k';
                                kopia[0][2]='w';
                                polozenie_krola_urzytkownika_x-=2;
                                wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
                                polozenie_krola_urzytkownika_x+=2;
                            }
                            else delete[]kopia;
                        }
                    }
                    break;
                }
                default: break;
            }
        }
    odrzuc_najgorsze_ruchy_urzytkownika();
}
    void Ruch::wykonaj_ruch_urzytkownika_jak_mozna(int &i, int &j, T_wsk_szachownica kopia){
//==============================================================================================================
    if(czy_krol_urzytkownika_jest_szachowany==false && j!=polozenie_krola_urzytkownika_x && i!=polozenie_krola_urzytkownika_y && polozenie_krola_urzytkownika_x+polozenie_krola_urzytkownika_y!=j+i && polozenie_krola_urzytkownika_x-polozenie_krola_urzytkownika_y!=j-i)
        wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
    else if(czy_pole_urzytkownika_jest_bite(polozenie_krola_urzytkownika_x, polozenie_krola_urzytkownika_y, kopia)==false)
        wektor_na_ruchy[numer_mojego_ruchu].push_back(kopia);
    else delete[]kopia;
}
        bool Ruch::czy_pole_urzytkownika_jest_bite(const int &x, const int &y, T_wsk_szachownica kopia){
//==============================================================================================================
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
        void Ruch::odrzuc_najgorsze_ruchy_urzytkownika(){
//==============================================================================================================
    int liczba_ruchow_urzytkownika = wektor_na_ruchy[numer_mojego_ruchu].size()-1;
    vector<double>status_materialny;
    for(int i=0; i<liczba_ruchow_urzytkownika; i++)                                                  //obliczamy statusy materialne urzytkownika
        status_materialny.push_back(oblicz_status_materialny(wektor_na_ruchy[numer_mojego_ruchu][i+1]));
    int index_komorki_z_najwieksza_wartoscia;


    while(liczba_ruchow_urzytkownika > ile_ruchow_urzytkownika_rozpatrywac[pokolenie_klasy])
    {
        index_komorki_z_najwieksza_wartoscia = 0;
        for(int i=1; i<liczba_ruchow_urzytkownika; i++)        //szukamu komorki z najwiekszym statusem materialnym
        {
            if(status_materialny[index_komorki_z_najwieksza_wartoscia] < status_materialny[i])
                index_komorki_z_najwieksza_wartoscia = i;
        }
        status_materialny.erase(status_materialny.begin() + index_komorki_z_najwieksza_wartoscia);  //usuwamy ta komorke
        delete[]wektor_na_ruchy[numer_mojego_ruchu][index_komorki_z_najwieksza_wartoscia+1];        //niezapominajac o zwolnieniu pamieci
        wektor_na_ruchy[numer_mojego_ruchu].erase(wektor_na_ruchy[numer_mojego_ruchu].begin() + index_komorki_z_najwieksza_wartoscia + 1);//wektor na ruchy tez nam sie zmienjsza
        liczba_ruchow_urzytkownika--;
    }
}
double Ruch::oblicz_status_materialny(const T_wsk_szachownica wsk_X){////!!!!!!!!!!!!!!!!!!!!!
//==============================================================================================================
    double status_materialny=0.0,
           x                =0.0;

    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            switch(wsk_X[i][j])
            {
                case 'p': status_materialny += (i==6)? -2.0 : -1.0; break;
                case 's': status_materialny += (i==0 || i==7 || j==0 || j==7)? -3.0-x : -3.0; break;
                case 'g': status_materialny += (i==0 || i==7 || j==0 || j==7)? -3.0-x : -3.0; break;
                case 'w': status_materialny += (wsk_X[1][j]=='P' || wsk_X[2][j]=='P' || wsk_X[3][j]=='P' || wsk_X[4][j]=='P' || wsk_X[5][j]=='P' || wsk_X[6][j]=='P')? -5.0-x : -5.0; break;
                case 'h': status_materialny += (wsk_X[1][j]=='P' || wsk_X[2][j]=='P' || wsk_X[3][j]=='P' || wsk_X[4][j]=='P' || wsk_X[5][j]=='P' || wsk_X[6][j]=='P')? -7.0-x : -7.0; break;

                case 'P': status_materialny += (i==1)? 2.0 : 1.0; break;
                case 'S': status_materialny += (i==0 || i==7 || j==0 || j==7)? 3.0+x : 3.0; break;
                case 'G': status_materialny += (i==0 || i==7 || j==0 || j==7)? 3.0+x : 3.0; break;
                case 'W': status_materialny += (wsk_X[1][j]=='p' || wsk_X[2][j]=='p' || wsk_X[3][j]=='p' || wsk_X[4][j]=='p' || wsk_X[5][j]=='p' || wsk_X[6][j]=='p')? 5.0+x : 5.0; break;
                case 'H': status_materialny += (wsk_X[1][j]=='p' || wsk_X[2][j]=='p' || wsk_X[3][j]=='p' || wsk_X[4][j]=='p' || wsk_X[5][j]=='p' || wsk_X[6][j]=='p')? 7.0+x : 7.0; break;
                default: break;
            }
        }
    return status_materialny;
}
T_wsk_szachownica Ruch::skopiuj_szachownice(const T_wsk_szachownica oryginal){
//==============================================================================================================
    T_wsk_szachownica kopia = new char[8][8];
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            kopia[i][j] = oryginal[i][j];
    return kopia;
}
void Ruch::wypisz_szachownice(const T_wsk_szachownica wsk_X){////!!!!!!!!!!!!!!!!!!!!!!!!!!
//==============================================================================================================
    cout<<string(23,'\333')<<endl;
    cout<<"\333\333\333 0 1 2 3 4 5 6 7 \333\333\333"<<endl;
    cout<<string(23,'\333')<<endl;
    for(int i=0; i<8; i++)
    //for(int i=7; i>=0; i--)
    {
        cout<<"\333"<<i<<"\333";
        for(int j=0; j<8; j++)
            cout<<' '<<wsk_X[i][j];
        cout<<' ';
        cout<<"\333"<<i<<"\333"<<endl;
    }
    cout<<string(23,'\333')<<endl;
    cout<<"\333\333\333 0 1 2 3 4 5 6 7 \333\333\333"<<endl;
    cout<<string(23,'\333')<<endl;
}











