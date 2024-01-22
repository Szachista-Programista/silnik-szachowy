#ifndef SZACHOWNICA_H
#define SZACHOWNICA_H
#include "global_definitions.h"

class Szachownica{
private:char tablica_szachownica[globalType::wysokosc_szachownicy][globalType::szerokosc_szachownicy];
        std::string tablica_figor[globalType::liczba_figor];
        bool kolor;
//******************************************************************************************************
public: Szachownica(bool k = true);
private:    void wczytaj_tablica_figor();
            void wczytaj_tablica_szachownica();
                void wczytaj_deske_szachowa()noexcept;
                void wczytaj_wspolrzedne();
                    void wczytaj_znak_wspolrzedny(int wsp_x, int wsp_y, std::string znak);
                void wczytaj_ulozenie_poczatkowe();
                    void wczytaj_figure(int wsp_pola_poziom, int wsp_pola_pion, int figura, bool kolor_figury, bool czy_podswietlic=0);
public: void usun_obwieszczenie(int szerokosc_obwieszczenia);
        void usun_pole_wyboru(int szerokosc_pola, int wysokosc_pola, bool czy_zakryc_szachownica = true);
        void wypisz_tablica_szachownica();
        void zaktualizuj_pole(int wsp_pola_poziom, int wsp_pola_pion, int figura, bool kolor_figury, bool czy_podswietlic);
private:    void odswiez_tablica_szachownica_miejscowo(int od_X, int od_Y, int do_X, int do_Y);
                void wypisz_figure(int wsp_pola_poziom, int wsp_pola_pion, bool czy_podswietlic);
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
        enum Wymiary{
            gorny_margines = 12,
            boczny_margines = 24,
            wysokosc_pola = 19,
            szerokosc_pola = 46,
        };
        enum Wartosci{
            liczba_znakow_wspolzednych = 16
        };
};
#endif//SZACHOWNICA_H
