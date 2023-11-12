#ifndef T_RUCH_H
#define T_RUCH_H
#include <vector>
#include <algorithm>

using T_wsk_szachownica = char(*)[8];


class Ruch
{
    public:////!!!!!!!!!!!!!!!!!!!!!!!!!!
    friend class Silnik;

    Ruch* wsk_do_rodzica_klasy_Ruch;
    std::vector<std::vector<T_wsk_szachownica>> wektor_na_ruchy;
    std::vector<double>statusy_materialne_rozpatrywanych_rochow;

    int
        polozenie_mojego_krola_x      ,
        polozenie_mojego_krola_y      ,
        polozenie_krola_urzytkownika_x ,//////
        polozenie_krola_urzytkownika_y ,//////////
        pokolenie_klasy               ,
        ostatnie_pokolenie       =  3 ,
        numer_mojego_ruchu       = -1 ,
        index_najgorszego_ruchu  =  0 ,
        ile_ruchow_rozpatrywac[10]{6,4,3,2,2,2,2,2,1,1},
        ile_ruchow_urzytkownika_rozpatrywac[10]{6,4,3,2,2,2,2,2,1,1};//////////

    double najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow = 100.0;

    bool kolor                                   ,
         czy_moj_krol_sie_ruszyl                 ,
         czy_moja_lewa_wierza_sie_ruszyla        ,
         czy_moja_prawa_wierza_sie_ruszyla       ,
         czy_krol_urzytkownika_sie_ruszyl         ,/////////////
         czy_lewa_wierza_urzytkownika_sie_ruszyla ,////////////
         czy_prawa_wierza_urzytkownika_sie_ruszyla,///////////
         czy_moj_krol_jest_szachowany            ,
         czy_krol_urzytkownika_jest_szachowany    ,//////////////
         koniec_gry_wygrana_maszyny      = false ,
         koniec_gry_wygrana_urzytkownika = false ,
         koniec_gry_pat_maszyny          = false ,
         koniec_gry_pat_urzytkownika     = false ;


    Ruch(bool k);
   ~Ruch();
    void przygotuj_Ruch();
    double oblicz_nastepny_ruch(T_wsk_szachownica &wsk_X);
        void zweryfikuj_polozenia_krolow(T_wsk_szachownica wsk_X);
        void sprawdz_czy_wieze_i_krole_byly_ruszane(T_wsk_szachownica wsk_X);
        void sprawdz_moj_krol_jest_szachowany(T_wsk_szachownica wsk_X);
        double znajdz_najleprze_z_posuniec(T_wsk_szachownica &wsk_X);
            void wyzeruj_wektor_na_ruchy();
            Ruch(Ruch* wskaznik,double &status_materialny,T_wsk_szachownica wsk_X, int iteracja);
                void przepisz_dane_o_ruchach_krolow_i_wiez(Ruch* wsk);
    void wykonaj_moj_ruch(T_wsk_szachownica wsk_X);
        void wykonaj_moj_ruch_jak_mozna(int &i, int &j, T_wsk_szachownica kopia);
            bool czy_moje_pole_jest_bite(const int &x,const int &y, T_wsk_szachownica kopia);
            void odrzuc_najgorszy_ruch();
    void wykonaj_ruch_urzytkownika(T_wsk_szachownica wsk_X);
        void wykonaj_ruch_urzytkownika_jak_mozna(int &i, int &j, T_wsk_szachownica kopia);
            bool czy_pole_urzytkownika_jest_bite(const int &x, const int &y, T_wsk_szachownica kopia);
            void odrzuc_najgorsze_ruchy_urzytkownika();
    double oblicz_status_materialny(const T_wsk_szachownica wsk_X);////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    T_wsk_szachownica skopiuj_szachownice(const T_wsk_szachownica oryginal);
    void wypisz_szachownice(const T_wsk_szachownica wsk_X);//??????????????????????????




};
#endif








