#ifndef SILNIK_H
#define SILNIK_H
#include <cmath>
#include "Ruch.h"

class Silnik{
private:Ruch aktualny;

        int kod_promocji;
        int nr_ruchu_debiutowego = 0;

        bool kolor;

        globalType::T_wsk_szachownica wsk_szachownica_wyjsciowa;
        globalType::T_wsk_szachownica wsk_szachownica_robocza;
    //********************************************************************************
public: Silnik(bool k);
private:    globalType::T_wsk_szachownica zainicjalizuj_szachownice();
public:~Silnik()noexcept;
        bool czy_urzytkownik_moze_wykonac_takie_posuniecie(int posuniecie_urzytkownika);
private:    void rozszyfruj_posuniecie_urzytkownika(int posuniecie_urzytkownika);
            bool czy_na_polu_wyjsciowym_jest_bierka_urzytkownika();
            bool czy_na_polu_docelowym_jest_bierka_urzytkownika();
            bool czy_ten_ruch_powoduje_wystawienie_swojego_krola_na_bicie();
            bool czy_ruch_jest_zgodny_z_zasadami();
public: int wykonaj_posuniecie(int posuniecie_urzytkownika);
private:    void przygotuj_maszyne_na_ruch(int posuniecie_urzytkownika);
                void nanies_posuniecie_urzytkownika_na_szachownice(int posuniecie_urzytkownika);
            void wykonaj_ruch_debiutowy();
            void wykonaj_ruch_gry_srodkowej();
                void czy_czas_na_koncowke();
            void wykonaj_koncowke();
            int oblicz_posuniecie_maszyny()noexcept;
                void znajdz_posuniecie_maszyny()noexcept;
                int  zaszyfruj_posuniecie_maszyny()noexcept;
                void nanies_posuniecie_maszyny_na_szachownice()noexcept;
                int  czy_to_koniec_gry()noexcept;
    //********************************************************************************************
        struct Posuniecie{
            int zX,
                zY,
                naX,
                naY;
        }u,m;//urzytkownika oraz maszyny

        enum Etap_gry{
            debiut       = 1,
            gra_wlasciwa = 2,
            koncowka     = 3
        }etap_gry = gra_wlasciwa;//debiut;//XXXXXXXXXXXXXXXXX

};
#endif//SILNIK_H
