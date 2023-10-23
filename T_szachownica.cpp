#include <iostream>//?????????????????
#include <windows.h>
#include <string>

#include "T_szachownica.h"

using namespace std;

void T_szachownica::wczytaj_tablica_figor()
{
    tablica_figor[brak]  =  string(418,'t'),
    tablica_figor[pion]  = {string{"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"}+
                            string{"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"}+
                            string{"TTTTTTTTTTTTTTTTTTTTTTTTTTTTT4444TTTTTTTTTTTTTTTT41SSSS14TTTTTTTTT"}+
                            string{"TTTTTGSSSSSSGTTTTTTTTTTTTTT32SSSS23TTTTTTTTTTTTTTT41SS14TTTTTTTTTT"}+
                            string{"TTTTT332SS233TTTTTTTTTTTTTTTTGSSGTTTTTTTTTTTTTTTTTGSSSSGTTTTTTTTTT"}+
                            string{"TTTTT41SSSS14TTTTTTTTTTTTT41SSSSSS14TTTTTTTTTT41SSSSSSSSSS14TTTTTT"}+
                            string{"TT33333333333333TTTTTT"}},
    tablica_figor[skoczek]={string{"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"}+
                            string{"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT41GTTTTTTTTTTTTTTTT411SSS144TTTTTT"}+
                            string{"TTTTT41SSSSSSSSS1144TTTTTT41SSSS2SSSSS22SSGTTTTTGSSSSGT32233TT33TT"}+
                            string{"TTTGSSSSSSGTTTTTTTTTTTTTGSSSSSSSSGTTTTTTTTTTTTGSSSSSSSSS14TTTTTTTT"}+
                            string{"TTGSSSSSSSSSSS14TTTTTTTT32SSSSSSSSSSSGTTTTTTTTT32SSSSSSSSSGTTTTTTT"}+
                            string{"TTTTGSSSSSSSSGTTTTTTTTTTT441SSSSSS144TTTTTTTT41SSSSSSSSSSSS14TTTTT"}+
                            string{"T3333333333333333TTTTT"}},
    tablica_figor[goniec] ={string{"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTGGTTTTTTTTTTTT"}+
                            string{"TTTTTT41SSGTTTTTTTTTTTTTTTT4T32SS14TTTTTTTTTTTTTTG14TGSSGTTTTTTTTT"}+
                            string{"TTTTTTGS1SSGTTTTTTTTTTTTTTTTT3223TTTTTTTTTTTTTTTTT3GSSG3TTTTTTTTTT"}+
                            string{"TTTTT41SSSS14TTTTTTTTTTTTTT332SS233TTTTTTTTTTTTTTT41SS14TTTTTTTTTT"}+
                            string{"TTTTT41SSSS14TTTTTTTTTTTTT41SSSSSS14TTTTTTTTTTT41SSSSSSSS14TTTTTTT"}+
                            string{"TTT32SSSSSSSS23TTTTTTTTTT441SSSSSS144TTTTTTTT41SSSSSSSSSSSS14TTTTT"}+
                            string{"T3333333333333333TTTTT"}},
    tablica_figor[wierza] ={string{"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"}+
                            string{"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"}+
                            string{"TT444TT4444TT444TTTTTTTTGS14TGSSGT41SGTTTTTTTT32SS1SSSS1SS23TTTTTT"}+
                            string{"TTT32SSSSSSSS23TTTTTTTTTTTTGSSSSSSGTTTTTTTTTTTTTTGSSSSSSGTTTTTTTTT"}+
                            string{"TTTTTGSSSSSSGTTTTTTTTTTTTTGSSSSSSSSGTTTTTTTTTTTTGSSSSSSSSGTTTTTTTT"}+
                            string{"TTTTGSSSSSSSSGTTTTTTTTTTT41SSSSSSSS14TTTTTTTT41SSSSSSSSSSSS14TTTTT"}+
                            string{"T3333333333333333TTTTT"}},
    tablica_figor[hetman] ={string{"TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"}+
                            string{"TTTT4TTTTTTTT4TTTTTTTTTTTTGGTTGGTTGGTTTTTTTTTTTTGS141141SGTTTTTTTT"}+
                            string{"TTTTTGSSSSSSGTTTTTTTTTTTTTT32SSSS23TTTTTTTTTTTTTT441SS144TTTTTTTTT"}+
                            string{"TTTTTT32SS23TTTTTTTTTTTTTTTTGSSSSGTTTTTTTTTTTTTTTTGSSSSGTTTTTTTTTT"}+
                            string{"TTTTTTGSSSSGTTTTTTTTTTTTTTTGSSSSSSGTTTTTTTTTTTT41SSSSSSSS14TTTTTTT"}+
                            string{"TTT32SSSSSSSS23TTTTTTTTTT441SSSSSS144TTTTTTTT41SSSSSSSSSSSS14TTTTT"}+
                            string{"T3333333333333333TTTTT"}},
    tablica_figor[krol]  = {string{"TTTTTTTT44TTTTTTTTTTTTTTTTTT441144TTTTTTTTTTTTTTTT332233TTTTTTTTTT"}+
                            string{"TTTTTT441144TTTTTTTTTTTTTT41SSSSSS14TTTTTTTTTTTT32SSSSSS23TTTTTTTT"}+
                            string{"TTTTTTGSSSSGTTTTTTTTTTTTTTTTTGSSGTTTTTTTTTTTTTTTT4GSSSSG4TTTTTTTTT"}+
                            string{"TTTTTTTGSSGTTTTTTTTTTTTTTTTTGSSSSGTTTTTTTTTTTTTTTTGSSSSGTTTTTTTTTT"}+
                            string{"TTTTTTGSSSSGTTTTTTTTTTTTTTTGSSSSSSGTTTTTTTTTTTT41SSSSSSSS14TTTTTTT"}+
                            string{"TTT32SSSSSSSS23TTTTTTTTTT441SSSSSS144TTTTTTTT41SSSSSSSSSSSS14TTTTT"}+
                            string{"T3333333333333333TTTTT"}};
}
void T_szachownica::wczytaj_tablica_szachownica(bool kolor)
{
    wczytaj_deske_szachowa();
    wczytaj_wspolrzedne(kolor);
    wczytaj_ulozenie_poczatkowe(kolor);
}
    void T_szachownica::wczytaj_deske_szachowa()
{
    string pole_biale      (pole_szerokosc, '\333');
    string pole_czarne     (pole_szerokosc,    ' ');
    string margines_boczny (margines_bok,      ' ');
    string ramka_boczna    (ramka_bok,      '\333');

    string margines_podstawa  (szerokosc,         ' ');
    string podstawa_ramki = margines_boczny + string(2*ramka_bok + 8*pole_szerokosc,'\333') + margines_boczny;
    string pola_bcbcbcbc  = margines_boczny + ramka_boczna + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + ramka_boczna + margines_boczny;
    string pola_cbcbcbcb  = margines_boczny + ramka_boczna + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + ramka_boczna + margines_boczny;

    for(int i=0; i<=2*ramka_gora+margines_gora+8*pole_wysokosc; i+=2*ramka_gora+margines_gora+8*pole_wysokosc)
        for(int j=0; j<margines_gora; j++)
            margines_podstawa.copy(tablica_szachownica[j+i],szerokosc);

    for(int i=0; i<=ramka_gora+8*pole_wysokosc; i+=ramka_gora+8*pole_wysokosc)
        for(int j=margines_gora; j<margines_gora+ramka_gora; j++)
            podstawa_ramki.copy(tablica_szachownica[j+i],szerokosc);

    for(int i=0; i<8*pole_wysokosc ; i+=2*pole_wysokosc)
    {
        for(int j = margines_gora+ramka_gora; j < margines_gora+ramka_gora+pole_wysokosc; j++)
            pola_bcbcbcbc.copy(tablica_szachownica[j+i],szerokosc);

        for(int j = margines_gora+ramka_gora+pole_wysokosc; j < margines_gora+ramka_gora+2*pole_wysokosc; j++)
            pola_cbcbcbcb.copy(tablica_szachownica[j+i],szerokosc);
    }
}
    void T_szachownica::wczytaj_wspolrzedne(bool kolor)
{
    string wspolrzedne[16]{{"  DXXX      XX      XX      XX      XX  "},
                           {" GXXXXD      XX  DXXXXG  XX      GXXXXD "},
                           {" GXXXXD      XX  XXXXXX      XX  DXXXXG "},
                           {" XD  XX  XX  XX  GXXXXX      XX      XX "},
                           {" DXXXXG  XX      GXXXXD      XX  DXXXXG "},
                           {" DXXXXG  XX      XXXXXD  XX  XX  GXXXXG "},
                           {" GXXXXX     DXX    DXX     XX      XX   "},
                           {" DXXXXD  XX  XX  XXXXXX  XX  XX  GXXXXG "},
                           {" DXXXXD XX    XXXXXXXXXXXX    XXXX    XX"},
                           {"XXXXXXD XX    XXXXXXXXX XX    XXXXXXXXG "},
                           {" DXXXXD XX    GGXX      XX    DD GXXXXG "},
                           {"XXXXXXD XX    XXXX    XXXX    XXXXXXXXG "},
                           {"XXXXXXX XX      XXXXXX  XX      XXXXXXX "},
                           {"XXXXXXX XX      XXXXXX  XX      XX      "},
                           {" DXXXXXDXX      XX   XXXXX    XX GXXXXXG"},
                           {"XX    XXXX    XXXXXXXXXXXX    XXXX    XX"}};

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
    for(int i=1; i<=8; i++)
    {
        wczytaj_figure(pion, i, 2, !kolor);
        wczytaj_figure(pion, i, 7,  kolor);
    }
    wczytaj_figure(wierza,  1, 1, !kolor);
    wczytaj_figure(skoczek, 2, 1, !kolor);
    wczytaj_figure(goniec,  3, 1, !kolor);
    wczytaj_figure(kolor?hetman:krol, 4, 1, !kolor);
    wczytaj_figure(kolor?krol:hetman, 5, 1, !kolor);
    wczytaj_figure(goniec,  6, 1, !kolor);
    wczytaj_figure(skoczek, 7, 1, !kolor);
    wczytaj_figure(wierza,  8, 1, !kolor);

    wczytaj_figure(wierza,  1, 8, kolor);
    wczytaj_figure(skoczek, 2, 8, kolor);
    wczytaj_figure(goniec,  3, 8, kolor);
    wczytaj_figure(kolor?hetman:krol, 4, 8, kolor);
    wczytaj_figure(kolor?krol:hetman, 5, 8, kolor);
    wczytaj_figure(goniec,  6, 8, kolor);
    wczytaj_figure(skoczek, 7, 8, kolor);
    wczytaj_figure(wierza,  8, 8, kolor);
}
        void T_szachownica::wczytaj_figure(int figura, int wsp_pola_poziom, int wsp_pola_pion, bool kolor_figury)
{
    int wsp_x = (wsp_pola_poziom-1)*pole_szerokosc+35;
    int wsp_y = (wsp_pola_pion-1)*pole_wysokosc+12;
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
T_szachownica::T_szachownica()
{
    wczytaj_tablica_figor();
    wczytaj_tablica_szachownica(kolor);
}
void T_szachownica::wypisz_tablica_szachownica()
{
    for(int i=0; i<wysokosc; i++)
    {
        for(int j=0; j<szerokosc; j++)
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
