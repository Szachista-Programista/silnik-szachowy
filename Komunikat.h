#ifndef Komunikat_H
#define Komunikat_H

#include <string>

class Komunikat
{
public://????????????????????//

    std::string znaki[68][9];
    std::string tablica_obwieszczenie[13];
    std::string tablica_pole_wyboru[53];
    int szerokosc_pola;
    int wysokosc_pola;
    int szerokosc_najdluzszej_opcji;
    int liczba_opcji;






//***************************************
    Komunikat();
        void wczytaj_znaki();
    void ustawienie_wielkosci_obrazu();
    void obwieszczenie(std::string tresc, int czas_ms);
        void wyczysc_tablica_obwieszczenie();
        void dodaj_tresc_do_tablica_obwieszczenie(std::string tresc);
            void dodaj_znak_do_tablica_obwieszczenie(char znak);
                int podaj_index_znaku(char znak);
        void wypisz_obwieszczenie();
            void ustaw_kursor_na(int x, int y);

    int pole_wyboru(string opcje[]);
        void nadaj_wartosci_parametrom_pola(string opcje[]);
        void wyczysc_tablica_pole_wyboru();
            int oblicz_szerokosc_tekstu(string tresc);
                int podaj_szerokosc_znaku(char znak);
        void wczytaj_obramowanie_pola();
        void wszytaj_tresc_pola_wyboru(string opcje[]);
            void wczytaj_linijke_tekstu(string tresc, int nr_linijki);
                void dodaj_znak_do_linijki(char znak, string linijka[]);
                void przepisz_linijke_do_tablica_pola_wyboru(string linijka[], int nr_linijki);
        void wypisz_pole_wyboru();
        int wybor_opcji();
            void zaznacz_opcje(int nr_opcji);
                void anuluj_podswietlenie();
                void podswietl_opcje(int nr_opcji);





};
#endif





