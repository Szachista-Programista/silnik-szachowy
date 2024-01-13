#ifndef Komunikat_H
#define Komunikat_H
#include <string>
#include <vector>

class Komunikat
{
public://????????????????????//

    std::string znaki[69][9];
    std::string tablica_obwieszczenie[13];
    std::string tablica_pole_wyboru[53];
    int szerokosc_pola;
    int wysokosc_pola;
    int szerokosc_najdluzszej_opcji;
    int liczba_opcji;






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
    int pole_wyboru(std::vector<std::string> opcje);
        void nadaj_wartosci_parametrom_pola(std::vector<std::string> opcje);
            int oblicz_szerokosc_tekstu(std::string tresc);
                int podaj_szerokosc_znaku(char znak);
        void wyczysc_tablica_pole_wyboru();
        void wczytaj_obramowanie_pola();
        void wczytaj_tresc_pola_wyboru(std::vector<std::string> opcje);
            void wczytaj_linijke_tekstu(std::string tresc, int nr_linijki);
                void dodaj_znak_do_linijki(char znak, std::string linijka[]);
                void przepisz_linijke_do_tablica_pola_wyboru(std::string linijka[], int nr_linijki);
        void wypisz_pole_wyboru();
        int wybor_opcji();
            void zaznacz_opcje(int nr_opcji);
                void anuluj_podswietlenie();
                void podswietl_opcje(int nr_opcji);





};
#endif





