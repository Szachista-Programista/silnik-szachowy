#ifndef RUCH_H
#define RUCH_H
#include "global_definitions.h"

class Ruch{
    friend class Silnik;

    Ruch* wsk_do_rodzica_klasy_Ruch;
    std::vector<std::vector<globalType::T_wsk_szachownica>> wektor_na_ruchy;
    std::vector<double>statusy_materialne_rozpatrywanych_rochow;

    int polozenie_mojego_krola_x;
    int polozenie_mojego_krola_y;
    int polozenie_krola_urzytkownika_x;
    int polozenie_krola_urzytkownika_y;
    int pokolenie_klasy;
    int ostatnie_pokolenie       =  1;//XXXXXXXXXXXXXXXx
    int numer_mojego_ruchu       = -1;
    int index_najgorszego_ruchu  =  0;
    int ile_ruchow_rozpatrywac[10]{12,7,4,1,1,1,1,1,1,1};
    int ile_ruchow_urzytkownika_rozpatrywac[10]{6,3,2,1,1,1,1,1,1,1};

    double najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow = 100.0;

    bool kolor;
    bool czy_moj_krol_sie_ruszyl;
    bool czy_moja_lewa_wierza_sie_ruszyla;
    bool czy_moja_prawa_wierza_sie_ruszyla;
    bool czy_krol_urzytkownika_sie_ruszyl;
    bool czy_lewa_wierza_urzytkownika_sie_ruszyla;
    bool czy_prawa_wierza_urzytkownika_sie_ruszyla;
    bool czy_moj_krol_jest_szachowany;
    bool czy_krol_urzytkownika_jest_szachowany;
    bool koniec_gry_wygrana_maszyny      = false;
    bool koniec_gry_wygrana_urzytkownika = false;
    bool koniec_gry_pat_maszyny          = false;
    bool koniec_gry_pat_urzytkownika     = false;
//********************************************************************************
    Ruch(bool k)noexcept;
   ~Ruch()noexcept;
    void przygotuj_Ruch()noexcept;
    double oblicz_nastepny_ruch(globalType::T_wsk_szachownica &wsk_X);
        void zweryfikuj_polozenia_krolow(globalType::T_wsk_szachownica wsk_X);
        void sprawdz_czy_wieze_i_krole_byly_ruszane(globalType::T_wsk_szachownica wsk_X);
        void sprawdz_moj_krol_jest_szachowany(globalType::T_wsk_szachownica wsk_X);
        double znajdz_najleprze_z_posuniec(globalType::T_wsk_szachownica &wsk_X);
            double obsluga_poczatku_drzewa_poszukowan(globalType::T_wsk_szachownica &wsk_X);
                void obsluga_konca_poszukowan(globalType::T_wsk_szachownica &wsk_X, int &index);
                    void czy_urzytkownik_zakonczyl_gre(globalType::T_wsk_szachownica &wsk_X);
                    void czy_maszyna_zakonczyla_gre(globalType::T_wsk_szachownica &wsk_X, int &index);
            double obsluga_konca_drzewa_poszukowan()noexcept;
            void wyzeruj_wektor_na_ruchy()noexcept;
            Ruch(Ruch* wskaznik,double &status_materialny,globalType::T_wsk_szachownica wsk_X, int iteracja);
                void przepisz_dane_o_ruchach_krolow_i_wiez(Ruch* wsk);
    void wykonaj_moj_ruch(globalType::T_wsk_szachownica wsk_X);
        void obsluga_ruchu_maszyny(int zX, int zY, int naX, int naY, char bierka, globalType::T_wsk_szachownica wsk_X);
            void wykonaj_moj_ruch_jak_mozna(int &zY, int &zX, globalType::T_wsk_szachownica kopia);
                bool czy_moje_pole_jest_bite(const int &x,const int &y, globalType::T_wsk_szachownica kopia);
                void odrzuc_najgorszy_ruch();
    void wykonaj_ruch_urzytkownika(globalType::T_wsk_szachownica wsk_X);
        void obsluga_ruchu_urzytkownika(int zX, int zY, int naX, int naY, char bierka, globalType::T_wsk_szachownica wsk_X);
            void wykonaj_ruch_urzytkownika_jak_mozna(int &y, int &x, globalType::T_wsk_szachownica kopia);
                bool czy_pole_urzytkownika_jest_bite(const int &x, const int &y, globalType::T_wsk_szachownica kopia);
            void odrzuc_najgorsze_ruchy_urzytkownika();
    double oblicz_status_materialny(const globalType::T_wsk_szachownica wsk_X);
    globalType::T_wsk_szachownica skopiuj_szachownice(const globalType::T_wsk_szachownica oryginal);
};
#endif//RUCH_H








