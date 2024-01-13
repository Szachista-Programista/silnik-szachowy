#ifndef PARTIA_H
#define PARTIA_H
#include "Szachownica.h"
#include "Silnik.h"
#include "Notatnik.h"
#include "Komunikat.h"

class Partia
{
public:

    bool kolor;
    bool ruch_maszyny_jest_podswietlony;
    bool wybrano_prawidlowe_pole;
    bool wykonano_prawidlowy_ruch;

    int ruch_maszyny;
    int ruch_urzytkownika;
    int wspolzedne_urzytkownika;
    int kod_promocji;

    Komunikat    komunikat;
    Szachownica  szachownica;
    Silnik       silnik;
    Notatnik     notatnik;

    std::string kod_aktualnej_aktualizacji_szachownicy;
    std::string kod_ubieglej_aktualizacji_szachownicy;
    std::string kod_czastkowy;

//********************************************************************************

    Partia(bool k);
    void zagraj_z_urzytkownikiem();
        bool obsluga_ruchu_urzytkownika();
            int pobierz_wspolzedne();
                bool menu_zakonczenia_parti(char znak);
            std::string wygeneruj_kod_czastkowy();
            void zaktualizuj_szachownice(std::string kod_aktualizacji, bool podswietlenie);
                int podaj_kod_bierki(char znak);
                bool podaj_kolor_bierki(char znak);
            void wylacz_podswietlenie_ubieglego_ruchu_maszyny();
            bool czy_wybrano_prawidlowe_pole();
            bool czy_urzytkownik_dokonuje_promocji_pionka();
        bool obsluga_ruchu_maszyny(int komenda);
            bool czy_to_koniec_gry();
        void menu_zapisu_gry();
};
#endif






