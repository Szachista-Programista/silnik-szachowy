#ifndef NOTATNIK_H
#define NOTATNIK_H
#include <iomanip>
#include <sstream>
#include "global_definitions.h"

class Notatnik{
public: globalType::T_wsk_szachownica aktualna_szachownica;
private:globalType::T_wsk_szachownica ubiegla_szachownica;

        std::vector<std::string>zapis_calej_gry;

        std::string notacja;
        std::string znaki[globalType::liczba_znakow][globalType::wysokosc_litery];
        std::string kolumna_z_notacjami[globalType::liczba_kolumn_notacji][globalType::wysokosc_kolumny];
        std::string tablica_notacja[globalType::wysokosc_litery];
        std::string kod_aktualizacji_szachownicy;
        std::string wynik_gry;

        int krol_maszyny_x;
        int krol_maszyny_y;
        int krol_urzytkownika_x;
        int krol_urzytkownika_y;

        int kod_ruchu;
        int ruch_zX;
        int ruch_zY;
        int ruch_naX;
        int ruch_naY;
        int parametr_dodatkowy;

        int nr_ruchu = 0;
        int nr_pary_ruchow;

        int aktualny_wiersz_notacji  = 0;
        int aktualna_kolumna_notacji = 0;
        int ubiegly_wiersz_notacji   = 0;
        int ubiegla_kolumna_notacji  = 0;

        bool kolor;
        bool ruch_maszyny;
        bool naniesiono_ruch;
        bool koniec_gry = false;
//********************************************************************************
public: Notatnik(bool k);
private:    globalType::T_wsk_szachownica zainicjalizuj_szachownice();
            void wczytaj_znaki();
public:~Notatnik()noexcept;
        void wygeneruj_i_wypisz_notacje(int ruch);
private:    void uaktualnij_wartosci_parametrow(int ruch);
                void rozszyfruj_ruch();
                void zaktualizuj_polozenia_krolow()noexcept;
            void nanies_ruch_na_szachownice_i_zapisz_notacje();
                void nanies_roszade()noexcept;
                void nanies_promocje_pionka()noexcept;
                void nanies_bicie_w_przelocie()noexcept;
                void nanies_zwykly_ruch();
                std::string podaj_wspolrzedne(int x, int y);
                bool czy_jest_szach()noexcept;
            void uzupelnij_zapis_gry();
                void obsluga_konca_gry();
                    void zamien_plus_na_hasztag();
            void wypisz_notacje();
                void wyczysc_tablica_notacja()noexcept;
                void wypisz_ubiegla_notacje(int wiersz, int kolumna);
                void cofnij_kolumny();
                    void skopiuj_kolumne(int index_kopi, int index_wzorca);
                    void oproznij_kolumne(int numer_kolumny);
                    void wypisz_kolumne(int nr);
public:             void wyczysc_kolumne(int nr);
private:        void dodaj_tresc_do_tablica_notacja(std::string tresc);
                    void dodaj_znak_do_tablica_notacja(char znak);
                        int podaj_index_znaku(char znak);
                void wypisz_tablica_notacja(int wiersz, int kolumna, bool podswietlenie);
                void przepisz_tablice_do_kolumny()noexcept;
public: std::string podaj_kod_aktualizacji_szachownicy()noexcept;
private:    void oblicz_kod_aktualizacji_szachownicy()noexcept;
public: void zapisz_gre_w_notatniku();
};
#endif//NOTATNIK_H
