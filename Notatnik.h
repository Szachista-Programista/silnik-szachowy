#ifndef Notatnik_H
#define Notatnik_H
#include <string>
#include <vector>
using T_wsk_szachownica = char(*)[8];




class Notatnik
{
public://???????????????????????????????????????????
    T_wsk_szachownica aktualna_szachownica,
                      ubiegla_szachownica;
    std::string notacja;
    std::vector<std::string>zapis_calej_gry;
    std::string znaki[69][9];
    std::string kolumna_z_notacjami[3][176];
    std::string tablica_notacja[9];
    std::string kod_aktualizacji_szachownicy;
    std::string wynik_gry;

    int krol_maszyny_x,
        krol_maszyny_y,
        krol_urzytkownika_x,
        krol_urzytkownika_y,

        kod_ruchu,
        ruch_zX,
        ruch_zY,
        ruch_naX,
        ruch_naY,
        parametr_dodatkowy,

        nr_ruchu=0,
        nr_pary_ruchow,

        aktualny_wiersz_notacji  = 0,
        aktualna_kolumna_notacji = 0,
        ubiegly_wiersz_notacji   = 0,
        ubiegla_kolumna_notacji  = 0;

    bool kolor,
         ruch_maszyny,
         naniesiono_ruch,
         koniec_gry = false;



//********************************************************************************
    Notatnik(bool k);
        T_wsk_szachownica zainicjalizuj_szachownice();
        void wczytaj_znaki();
    ~Notatnik();
    void wygeneruj_i_wypisz_notacje(int ruch);
        void uaktualnij_wartosci_parametrow(int ruch);
            void rozszyfruj_ruch();
            void zaktualizuj_polozenia_krolow();
        void nanies_ruch_na_szachownice_i_zapisz_notacje();
            void nanies_roszade();
            void nanies_promocje_pionka();
            void nanies_bicie_w_przelocie();
            void nanies_zwykly_ruch();
            std::string podaj_wspolrzedne(int x, int y);
            bool czy_jest_szach();
        void uzupelnij_zapis_gry();
            void obsluga_konca_gry();
                void zamien_plus_na_hasztag();
        void wypisz_notacje();
            void wyczysc_tablica_notacja();
            void wypisz_ubiegla_notacje(int wiersz, int kolumna);
            void cofnij_kolumny();
                void skopiuj_kolumne(int index_kopi, int index_wzorca);
                void oproznij_kolumne(int numer_kolumny);
                void wypisz_kolumne(int nr);
                void wyczysc_kolumne(int nr);
            void dodaj_tresc_do_tablica_notacja(std::string tresc);
                void dodaj_znak_do_tablica_notacja(char znak);
                    int podaj_index_znaku(char znak);
            void wypisz_tablica_notacja(int wiersz, int kolumna, bool podswietlenie);
                void ustaw_kursor_na(int x, int y);
            void przepisz_tablice_do_kolumny();
    std::string podaj_kod_aktualizacji_szachownicy();
        void oblicz_kod_aktualizacji_szachownicy();
    void zapisz_gre_w_notatniku();




};
#endif
