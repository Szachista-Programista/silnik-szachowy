#ifndef T_SILNIK_H
#define T_SILNIK_H

#include <vector>
#include <string>

#include "T_ruch.h"
using T_wsk_szachownica = char(*)[8];
class T_ruch;
//extern bool kolor;

class T_silnik
{
    T_ruch aktualny;
    T_wsk_szachownica wsk_szachownica_wyjsciowa;

    /*int polozenie_mojego_krola_x      = kolor?3:4 ,
        polozenie_mojego_krola_y      = 7,
        polozenie_krola_przeciwnika_x = kolor?3:4,
        polozenie_krola_przeciwnika_y = 0;


    bool czy_moj_krol_sie_ruszyl                  = false,
         czy_moja_lewa_wierza_sie_ruszyla         = false,
         czy_moja_prawa_wierza_sie_ruszyla        = false,
         czy_krol_przeciwnika_sie_ruszyl          = false,
         czy_lewa_wierza_przeciwnika_sie_ruszyla  = false,
         czy_prawa_wierza_przeciwnika_sie_ruszyla = false;*/

    int zX, zY, naX, naY;      //posuniecie_przeciwnika

public:

    T_silnik(): wsk_szachownica_wyjsciowa{zainicjalizuj_szachownica_wyjsciowa()}{}
    //~T_silnik(){delete[]wsk_szachownica_wyjsciowa;}
    T_wsk_szachownica zainicjalizuj_szachownica_wyjsciowa();
    bool czy_przeciwnik_moze_wykonac_takie_posuniecie(int posuniecie);
        void rozszyfruj_posuniecie_przeciwnika(int posuniecie);
        bool czy_na_polu_wyjsciowym_jest_bierka_przeciwnika();
        bool czy_na_polu_docelowym_jest_bierka_przeciwnika();
        bool czy_ten_ruch_powoduje_wystawienie_swojego_krola_na_bicie();
        bool czy_ruch_jest_zgodny_z_zasadami();



    //int odpowiedz_przeciwnikowi_na_takie_posuniecie(int posuniecie);


};

#endif
