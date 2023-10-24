#ifndef T_SZACHOWNICA_H
#define T_SZACHOWNICA_H


#include <string>

extern bool kolor;


class T_szachownica
{
public:////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    char tablica_szachownica[176][416];
    std::string tablica_figor[7];

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

    void wczytaj_tablica_figor();
    void wczytaj_tablica_szachownica(bool kolor);
        void wczytaj_deske_szachowa();
        void wczytaj_wspolrzedne(bool kolor);
            void wczytaj_znak_wspolrzedny(int wsp_x, int wsp_y, std::string znak);
        void wczytaj_ulozenie_poczatkowe(bool kolor);
            void wczytaj_figure(int figura, int wsp_pola_poziom, int wsp_pola_pion, bool kolor_figury);
    T_szachownica();
    void wypisz_tablica_szachownica();
    void ustawienie_wielkosci_obrazu();

};

#endif
