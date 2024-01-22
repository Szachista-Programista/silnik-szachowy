#ifndef KOMUNIKAT_H
#define KOMUNIKAT_H
#include "global_definitions.h"

class Komunikat
{
private:std::string znaki_ASCII[globalType::liczba_znakow][globalType::wysokosc_litery];
        std::string tablica_obwieszczenie[globalType::wysokosc_obwieszczenia];
        std::string tablica_pole_wyboru[globalType::max_wysokosc_pola_wyboru];
        int szerokosc_pola;
        int wysokosc_pola;
        int szerokosc_najdluzszej_opcji;
        int liczba_opcji;
    //******************************************************************************************************
public: Komunikat();
private:    void wczytaj_znaki();
public: void ustawienie_wielkosci_obrazu()noexcept;
        void obwieszczenie(std::string tresc, int czas_ms = 0);
private:    void wyczysc_tablica_obwieszczenie()noexcept;
            void dodaj_tresc_do_tablica_obwieszczenie(std::string tresc);
                void dodaj_znak_do_tablica_obwieszczenie(char znak);
                    int podaj_index_znaku(char znak);
            void wypisz_obwieszczenie();
public: int pole_wyboru(std::vector<std::string> opcje);
private:    void nadaj_wartosci_parametrom_pola(std::vector<std::string> opcje);
                int oblicz_szerokosc_tekstu(std::string tresc);
                    int podaj_szerokosc_znaku(char znak);
            void wyczysc_tablica_pole_wyboru()noexcept;
            void wczytaj_obramowanie_pola()noexcept;
            void wczytaj_tresc_pola_wyboru(std::vector<std::string> opcje);
                void wczytaj_linijke_tekstu(std::string tresc, int nr_linijki);
                    void dodaj_znak_do_linijki(char znak, std::string linijka[]);
                    void przepisz_linijke_do_tablica_pola_wyboru(std::string linijka[], int nr_linijki)noexcept;
            void wypisz_pole_wyboru();
            int wybor_opcji();
                void zaznacz_opcje(int nr_opcji);
                    void anuluj_podswietlenie();
                    void podswietl_opcje(int nr_opcji);
};
#endif//KOMUNIKAT_H




















