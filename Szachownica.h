#ifndef Szachownica_H
#define Szachownica_H


#include <string>

extern bool kolor;


class Szachownica
{
public:////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    char tablica_szachownica[176][416];
    std::string tablica_figor[7];
    bool kolor;


//******************************************************************************************************
    Szachownica(bool k);
        void wczytaj_tablica_figor();
        void wczytaj_tablica_szachownica();
            void wczytaj_deske_szachowa();
            void wczytaj_wspolrzedne();
                void wczytaj_znak_wspolrzedny(int wsp_x, int wsp_y, std::string znak);
            void wczytaj_ulozenie_poczatkowe();
                void wczytaj_figure(int wsp_pola_poziom, int wsp_pola_pion, int figura, bool kolor_figury, bool czy_podswietlic=0);
    void usun_obwieszczenie(int szerokosc_obwieszczenia);
    void usun_pole_wyboru(int szerokosc_pola, int wysokosc_pola);
    void wypisz_tablica_szachownica();
    void zaktualizuj_pole(int wsp_pola_poziom, int wsp_pola_pion, int figura, bool kolor_figury, bool czy_podswietlic);
        void odswiez_tablica_szachownica_miejscowo(int od_X, int od_Y, int do_X, int do_Y);
            void wypisz_figure(int wsp_pola_poziom, int wsp_pola_pion, bool czy_podswietlic);
                void ustaw_kursor_na(int x, int y);
//******************************************************************************************************
    enum Bierki
    {
        brak    = 0,
        pion    = 1,
        skoczek = 2,
        goniec  = 3,
        wierza  = 4,
        hetman  = 5,
        krol    = 6
    };

};

#endif
