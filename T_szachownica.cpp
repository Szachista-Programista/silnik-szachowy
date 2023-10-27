#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include "T_szachownica.h"

using namespace std;

T_szachownica::T_szachownica()
{
    wczytaj_tablica_figor();
    wczytaj_tablica_szachownica(kolor);
}
    void T_szachownica::wczytaj_tablica_figor()
{
    ifstream odczyt;
    odczyt.open("figury.txt");
    for(int i=0; i<7; i++)
        getline(odczyt, tablica_figor[i]);
    odczyt.close();
}
    void T_szachownica::wczytaj_tablica_szachownica(bool kolor)
{
    wczytaj_deske_szachowa();
    wczytaj_wspolrzedne(kolor);
    wczytaj_ulozenie_poczatkowe(kolor);
    xxx();
}
        void T_szachownica::wczytaj_deske_szachowa()
{
    string pole_biale       (46, '\333');
    string pole_czarne      (46,    ' ');
    string margines_boczny  (20,    ' ');
    string ramka_boczna     (4,  '\333');
    string margines_podstawa(416,   ' ');

    string podstawa_ramki = margines_boczny + string(376,'\333') + margines_boczny;
    string pola_bcbcbcbc  = margines_boczny + ramka_boczna + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + ramka_boczna + margines_boczny;
    string pola_cbcbcbcb  = margines_boczny + ramka_boczna + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + ramka_boczna + margines_boczny;

    for(int i=0; i<=166; i+=166)
        for(int j=0; j<10; j++)
            margines_podstawa.copy(tablica_szachownica[j+i],416);
    for(int i=0; i<=154; i+=154)
        for(int j=10; j<10+2; j++)
            podstawa_ramki.copy(tablica_szachownica[j+i],416);
    for(int i=0; i<152; i+=38)
    {
        for(int j=12; j<31; j++)
            pola_bcbcbcbc.copy(tablica_szachownica[j+i],416);
        for(int j=31; j<50; j++)
            pola_cbcbcbcb.copy(tablica_szachownica[j+i],416);
    }
}
        void T_szachownica::wczytaj_wspolrzedne(bool kolor)
{
    ifstream odczyt;
    string wspolrzedne[16];
    odczyt.open("wspolzedne.txt");
    for(int i=0; i<16; i++)
        getline(odczyt, wspolrzedne[i]);
    odczyt.close();

    for(int x=43, i=kolor?8:15 ; x<=365; x+=46, kolor?i++:i--)
    {
        wczytaj_znak_wspolrzedny(x, 3,   wspolrzedne[i]);
        wczytaj_znak_wspolrzedny(x, 168, wspolrzedne[i]);
    }
    for(int y=19, i=kolor?7:0 ; y<=152; y+=19, kolor?i--:i++)
    {
        wczytaj_znak_wspolrzedny(6,   y, wspolrzedne[i]);
        wczytaj_znak_wspolrzedny(402, y, wspolrzedne[i]);
    }


}
            void T_szachownica::wczytaj_znak_wspolrzedny(int wsp_x, int wsp_y, string znak)
{
    for(int i=0, y=wsp_y; i<40; y++ )
        for(int x=wsp_x; x<wsp_x+8; i++, x++)
        {
            switch(znak[i])
            {
                case 'X': tablica_szachownica[y][x] = '\333'; break;
                case 'D': tablica_szachownica[y][x] = '\334'; break;
                case 'G': tablica_szachownica[y][x] = '\337'; break;
                default : break;
            }
        }
}
        void T_szachownica::wczytaj_ulozenie_poczatkowe(bool kolor)
{
    for(int i=0; i<=7; i++) //wczytanie pionkow
    {
        wczytaj_figure(pion, i, 1, !kolor);
        wczytaj_figure(pion, i, 6,  kolor);
    }
    wczytaj_figure(wierza,  0, 0, !kolor);  //wczytanie reszty bierek komputera
    wczytaj_figure(skoczek, 1, 0, !kolor);
    wczytaj_figure(goniec,  2, 0, !kolor);
    wczytaj_figure(kolor?hetman:krol, 3, 0, !kolor);
    wczytaj_figure(kolor?krol:hetman, 4, 0, !kolor);
    wczytaj_figure(goniec,  5, 0, !kolor);
    wczytaj_figure(skoczek, 6, 0, !kolor);
    wczytaj_figure(wierza,  7, 0, !kolor);

    wczytaj_figure(wierza,  0, 7, kolor);  //wczytanie reszty bierek urzytkownika
    wczytaj_figure(skoczek, 1, 7, kolor);
    wczytaj_figure(goniec,  2, 7, kolor);
    wczytaj_figure(kolor?hetman:krol, 3, 7, kolor);
    wczytaj_figure(kolor?krol:hetman, 4, 7, kolor);
    wczytaj_figure(goniec,  5, 7, kolor);
    wczytaj_figure(skoczek, 6, 7, kolor);
    wczytaj_figure(wierza,  7, 7, kolor);
}
            void T_szachownica::wczytaj_figure(int figura, int wsp_pola_poziom, int wsp_pola_pion, bool kolor_figury)////////
{
    int wsp_x = (wsp_pola_poziom)*46+37;
    int wsp_y = (wsp_pola_pion)*19+12;
    bool kolor_pola = !((wsp_pola_poziom+wsp_pola_pion)%2);

    for(int i=0, y=wsp_y; i<418; y++ )
        for(int x=wsp_x; x<wsp_x+22; i++, x++)
        {
            switch(tablica_figor[figura][i])
            {
                case 'T': tablica_szachownica[y][x] = kolor_pola  ?'\333'                   :' '   ;                       break;
                case 'G': tablica_szachownica[y][x] = kolor_pola  ?' '                      :'\333';                       break;
                case 'S': tablica_szachownica[y][x] = kolor_figury?'\333'                   :' '   ;                       break;
                case '1': tablica_szachownica[y][x] = kolor_pola  ?(kolor_figury?'\334':' '):(kolor_figury?'\333':'\337'); break;
                case '2': tablica_szachownica[y][x] = kolor_pola  ?(kolor_figury?'\337':' '):(kolor_figury?'\333':'\334'); break;
                case '3': tablica_szachownica[y][x] = kolor_pola  ?'\334'                   :'\337' ;                      break;
                case '4': tablica_szachownica[y][x] = kolor_pola  ?'\337'                   :'\334' ;                      break;
                default : break;
            }
        }
}
void T_szachownica::wypisz_tablica_szachownica()//??????????????????
{
    for(int i=0; i<176; i++)
    {
        for(int j=0; j<800; j++)
            cout<<tablica_szachownica[i][j];
        cout<<endl;
    }
}
void T_szachownica::ustawienie_wielkosci_obrazu()
{
    for(int i=0; i<176; i++)
        cout<<string(15,'\333')<<endl;
    cout<<"1. Prosze, wyswietl konsole na pelnym ekranie."<<endl
        <<"2. Ustaw wieklosc wyswietlanego obrazu tak aby powyzszy bialy pasek byl widoczny w calosi."<<endl
        <<"3. Wcisnij dowolny klawisz, aby rozpoczac."
        <<"\n\n\n\n\n\n\n\n\n";
    system("pause");
    system("cls");
}


void T_szachownica::xxx()
{
    for(int i=0; i<176; i++)
    {
        for(int j=416; j<800; j++)
            tablica_szachownica[i][j]='#';
    }
}













