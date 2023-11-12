#ifndef GRA_H
#define GRA_H
#include "Szachownica.h"
#include "Silnik.h"
#include "Notatnik.h"
#include "Komunikat.h"

class Menu
{
public:


    bool kolor;
    bool ruch_jest_podswietlony;
    bool wybrano_prawidlowe_pole;
    bool wykonano_prawidlowy_ruch;

    int ruch_maszyny;
    int ruch_urzytkownika;
    int wspolzedne_urzytkownika;
    int kod_promocji;

    Komunikat komunikat;
    Szachownica szachownica;
    Silnik silnik;
    Notatnik notatnik;

    std::string kod_aktualnej_aktualizacji_szachownicy;
    std::string kod_ubieglej_aktualizacji_szachownicy;
    std::string kod_czastkowy;


//********************************************************************************


    Gra(bool k);
    void zagraj_z_urzytkownikiem();
        void zaktualizuj_szachownice(std::string kod_aktualizacji, bool podswietlenie);
            int podaj_kod_bierki(char znak);
            bool podaj_kolor_bierki(char znak);
        void obsluga_ruchu_urzytkownika();
            int pobierz_wspolzedne();
                int odwroc_wspolzedne(int wspolzedne);
            std::string wygeneruj_kod_czastkowy();
            void wylacz_podswietlenie_ubieglego_ruchu();
            bool czy_wybrano_prawidlowe_pole();
            bool czy_urzytkownik_dokonuje_promocji_pionka();
        void obsluga_ruchu_maszyny(int komenda = ruch_urzytkownika);
    int odwroc_ruch(int wspolzedne);//????????????




};

#endif






