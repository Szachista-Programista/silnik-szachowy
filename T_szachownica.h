#ifndef T_SZACHOWNICA_H
#define T_SZACHOWNICA_H


#include <string>

extern bool kolor;


class T_szachownica
{
    enum T_wymiary_szachownicy
    {
        ramka_gora     =  2,
        ramka_bok      =  4,
        margines_gora  = 10,
        margines_bok   = 20,
        pole_wysokosc  = 19,
        pole_szerokosc = 46,
        wysokosc       = 2*margines_gora + 2*ramka_gora + 8*pole_wysokosc,
        szerokosc      = 2*margines_bok  + 2*ramka_bok  + 8*pole_szerokosc
    };
    enum T_bierki
    {
        brak    = 0,
        pion    = 1,
        skoczek = 2,
        goniec  = 3,
        wierza  = 4,
        hetman  = 5,
        krol    = 6
    };
    char tablica_szachownica[wysokosc][szerokosc];
    std::string tablica_figor[7];


    void wczytaj_tablica_figor();
    void wczytaj_tablica_szachownica(bool kolor);
        void wczytaj_deske_szachowa();
        void wczytaj_wspolrzedne(bool kolor);
            void wczytaj_znak_wspolrzedny(int wsp_x, int wsp_y, std::string znak);
        void wczytaj_ulozenie_poczatkowe(bool kolor);
public:
            void wczytaj_figure(int figura, int wsp_pola_poziom, int wsp_pola_pion, bool kolor_figury);
    T_szachownica();
    void wypisz_tablica_szachownica();
    void ustawienie_wielkosci_obrazu();

};

#endif
