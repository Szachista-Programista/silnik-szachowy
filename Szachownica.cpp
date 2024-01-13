#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include "Szachownica.h"

using namespace std;
Szachownica::Szachownica(bool k): kolor{k}{
//==============================================================================================================
    wczytaj_tablica_figor();
    wczytaj_tablica_szachownica();
}
    void Szachownica::wczytaj_tablica_figor(){
//==============================================================================================================
    ifstream odczyt;
    odczyt.open("figury.txt");
    for(int i=0; i<7; i++)
        getline(odczyt, tablica_figor[i]);
    odczyt.close();
}
    void Szachownica::wczytaj_tablica_szachownica(){
//==============================================================================================================
    wczytaj_deske_szachowa();
    wczytaj_wspolrzedne();
    wczytaj_ulozenie_poczatkowe();
}
        void Szachownica::wczytaj_deske_szachowa(){
//==============================================================================================================
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
        void Szachownica::wczytaj_wspolrzedne(){
//==============================================================================================================
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
            void Szachownica::wczytaj_znak_wspolrzedny(int wsp_x, int wsp_y, string znak){
//==============================================================================================================
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
        void Szachownica::wczytaj_ulozenie_poczatkowe(){//@@@
//==============================================================================================================
    /*char ulozenie_poczatkowe[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                                  {'p','p','p','p','p','p','p','p'},
                                  {' ',' ',' ',' ',' ',' ',' ',' '},
                                  {' ',' ',' ',' ',' ',' ',' ',' '},
                                  {' ',' ',' ',' ',' ',' ',' ',' '},
                                  {' ',' ',' ',' ',' ',' ',' ',' '},
                                  {'P','P','P','P','P','P','P','P'},
                                  {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};*/
    char ulozenie_poczatkowe[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                          {'p','p','p','p','p','p','p','p'},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {'P','P','P','P','P','P','P','P'},
                          {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};
    /*char ulozenie_poczatkowe[8][8]{{'k',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ','w',' ',' ',' ',' ',' '},
                          {' ','w',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {'K',' ',' ',' ',' ',' ',' ',' '}};*/


    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            switch (ulozenie_poczatkowe[i][j])
            {
                case 'p': wczytaj_figure(7-j,7-i , pion,     kolor); break;
                case 'P': wczytaj_figure(7-j,7-i , pion,    !kolor); break;
                case 's': wczytaj_figure(7-j,7-i , skoczek,  kolor); break;
                case 'S': wczytaj_figure(7-j,7-i , skoczek, !kolor); break;
                case 'g': wczytaj_figure(7-j,7-i , goniec,   kolor); break;
                case 'G': wczytaj_figure(7-j,7-i , goniec,  !kolor); break;
                case 'w': wczytaj_figure(7-j,7-i , wierza,   kolor); break;
                case 'W': wczytaj_figure(7-j,7-i , wierza,  !kolor); break;
                case 'h': wczytaj_figure(7-j,7-i , hetman,   kolor); break;
                case 'H': wczytaj_figure(7-j,7-i , hetman,  !kolor); break;
                case 'k': wczytaj_figure(7-j,7-i , krol,     kolor); break;
                case 'K': wczytaj_figure(7-j,7-i , krol,    !kolor); break;
                case ' ': wczytaj_figure(7-j,7-i , brak,     kolor); break;
                default: break;
            }
        }
}
            void Szachownica::wczytaj_figure(int wsp_pola_poziom, int wsp_pola_pion, int figura, bool kolor_figury, bool czy_podswietlic){
//==============================================================================================================
    int wsp_x = (wsp_pola_poziom)*46+24;
    int wsp_y = (wsp_pola_pion)*19+12;
    bool kolor_pola;
    if(!czy_podswietlic)
        kolor_pola = !((wsp_pola_poziom+wsp_pola_pion)%2);
    else
        kolor_pola = true;
    for(int i=0, y=wsp_y; i<874; y++ )
        for(int x=wsp_x; x<wsp_x+46; i++, x++)
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
void Szachownica::usun_obwieszczenie(int szerokosc_obwieszczenia){
//==============================================================================================================
    int szerokosc_marginesu = (416-szerokosc_obwieszczenia)/2;
    odswiez_tablica_szachownica_miejscowo(szerokosc_marginesu, 81, szerokosc_marginesu+szerokosc_obwieszczenia, 94);
}
void Szachownica::usun_pole_wyboru(int szerokosc_pola, int wysokosc_pola){
//==============================================================================================================
    int margines_bok =  (416-szerokosc_pola)/2;
    int margines_gora = (176-wysokosc_pola) /2;

    for(int i=0; i<wysokosc_pola; i++)
    {
        ustaw_kursor_na(margines_bok, margines_gora + i);
        for(int j=0; j<szerokosc_pola; j++)
            cout<<tablica_szachownica[margines_gora + i][margines_bok + j];
    }
}
void Szachownica::wypisz_tablica_szachownica(){
//==============================================================================================================
    ustaw_kursor_na(0, 0);
    for(int i=0; i<176; i++)
    {
        for(int j=0; j<416; j++)
            cout<<tablica_szachownica[i][j];
        cout<<endl;
    }
}
void Szachownica::zaktualizuj_pole(int wsp_pola_poziom, int wsp_pola_pion, int figura, bool kolor_figury, bool czy_podswietlic){
//==============================================================================================================
    wczytaj_figure(wsp_pola_poziom, wsp_pola_pion, figura, kolor_figury, czy_podswietlic);
    wypisz_figure(wsp_pola_poziom, wsp_pola_pion, czy_podswietlic);
}
    void Szachownica::wypisz_figure(int wsp_pola_poziom, int wsp_pola_pion, bool czy_podswietlic){//mmmmmmmmmmmmmm
//==============================================================================================================
    if(czy_podswietlic)
        cout<<"\033[31m";
    int wsp_x = (wsp_pola_poziom)*46+24;
    int wsp_y = (wsp_pola_pion)*19+12;
    odswiez_tablica_szachownica_miejscowo(wsp_x, wsp_y, wsp_x+45, wsp_y+18);
    cout<<"\033[0m";
}
        void Szachownica::odswiez_tablica_szachownica_miejscowo(int od_X, int od_Y, int do_X, int do_Y){
//==============================================================================================================
    for(int i=od_Y; i<=do_Y; i++)
    {
        ustaw_kursor_na(od_X, i);
        for(int j=od_X; j<=do_X; j++)
            cout<<tablica_szachownica[i][j];
    }
    ustaw_kursor_na(0, 180);
}
            void Szachownica::ustaw_kursor_na(int x, int y){
//==============================================================================================================
    cout<<"\033["<<y+1<<";"<<x+1<<"H";
}







