#ifndef T_SILNIK_H
#define T_SILNIK_H
#include <vector>
#include <string>
#include "Ruch.h"
using T_wsk_szachownica = char(*)[8];

class Silnik
{
    public:////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    int kod_promocji,
        nr_ruchu_debiutowego = 0;

    enum gra{
    debiut       = 1,
    gra_wlasciwa = 2,
    koncowka     = 3
    };
    gra stan_gry = debiut;
    //gra stan_gry = gra_wlasciwa;

    bool kolor;
    Ruch aktualny;
    T_wsk_szachownica wsk_szachownica_wyjsciowa,
                      wsk_szachownica_robocza;



    Silnik(bool k);
        T_wsk_szachownica zainicjalizuj_szachownice();
    ~Silnik();
    bool czy_urzytkownik_moze_wykonac_takie_posuniecie(int posuniecie_urzytkownika);
        void rozszyfruj_posuniecie_urzytkownika(int posuniecie_urzytkownika);
        bool czy_na_polu_wyjsciowym_jest_bierka_urzytkownika();
        bool czy_na_polu_docelowym_jest_bierka_urzytkownika();
        bool czy_ten_ruch_powoduje_wystawienie_swojego_krola_na_bicie();
        bool czy_ruch_jest_zgodny_z_zasadami();
    int wykonaj_posuniecie(int posuniecie_urzytkownika);
        void przygotuj_maszyne_na_ruch(int posuniecie_urzytkownika);
            void nanies_posuniecie_urzytkownika_na_szachownice(int posuniecie_urzytkownika);
        void wykonaj_ruch_debiutowy();
        void wykonaj_ruch_gry_srodkowej();
            void czy_czas_na_koncowke();
        void wykonaj_koncowke();
        int oblicz_posuniecie_maszyny();
            void znajdz_posuniecie_maszyny();
            int  zaszyfruj_posuniecie_maszyny();
            void nanies_posuniecie_maszyny_na_szachownice();
            int  czy_to_koniec_gry();
//********************************************************************************************
    struct posuniecie
    {
        int zX,
            zY,
            naX,
            naY;
    }u,m;//urzytkownika oraz maszyny
};

#endif
