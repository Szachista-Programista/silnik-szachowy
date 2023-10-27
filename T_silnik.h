#ifndef T_SILNIK_H
#define T_SILNIK_H
#include <vector>
#include <string>
#include "T_ruch.h"
using T_wsk_szachownica = char(*)[8];

class T_silnik
{
    public:////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    T_ruch aktualny;
    T_wsk_szachownica wsk_szachownica_wyjsciowa,
                      wsk_szachownica_robocza;

    struct posuniecie
    {
        int zX,
            zY,
            naX,
            naY;
    }u,m;//urzytkownika oraz maszyny


    T_silnik();
        T_wsk_szachownica zainicjalizuj_szachownice();
    ~T_silnik();
//********************************************************************************************
    bool czy_urzytkownik_moze_wykonac_takie_posuniecie(int posuniecie_urzytkownika);
        void rozszyfruj_posuniecie_urzytkownika(int posuniecie_urzytkownika);
        bool czy_na_polu_wyjsciowym_jest_bierka_urzytkownika();
        bool czy_na_polu_docelowym_jest_bierka_urzytkownika();
        bool czy_ten_ruch_powoduje_wystawienie_swojego_krola_na_bicie();
        bool czy_ruch_jest_zgodny_z_zasadami();
//********************************************************************************************
    int wykonaj_posunieice(int posuniecie_urzytkownika);
        void nanies_posuniecie_urzytkownika_na_szachownice(int posuniecie_urzytkownika);
        void znajdz_posuniecie_maszyny();
        int  zaszyfruj_posuniecie_maszyny();
        void nanies_posuniecie_maszyny_na_szachownice();
        int  czy_to_koniec_gry();
};

#endif
