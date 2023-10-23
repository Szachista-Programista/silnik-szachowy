#ifndef T_RUCH_H
#define T_RUCH_H

#include <vector>
#include <algorithm>
//#include "T_silnik.h"

using T_wsk_szachownica = char(*)[8];//????????????
extern bool kolor;
class T_silnik;//????????????

class T_ruch
{
public:////!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    friend class T_silnik;

    T_ruch* wsk_do_rodzica_klasy_T_ruch;
    int pokolenie_klasy = 0,//?????????
        ostatnie_pokolenie = 3;

    int polozenie_mojego_krola_x     ,
        polozenie_mojego_krola_y     ,
        polozenie_krola_przeciwnika_x,
        polozenie_krola_przeciwnika_y;

    bool czy_moj_krol_sie_ruszyl                 ,
         czy_moja_lewa_wierza_sie_ruszyla        ,
         czy_moja_prawa_wierza_sie_ruszyla       ,
         czy_krol_przeciwnika_sie_ruszyl         ,
         czy_lewa_wierza_przeciwnika_sie_ruszyla ,
         czy_prawa_wierza_przeciwnika_sie_ruszyla;

    int ile_ruchow_rozpatrywac[10]{2,2,2,2,2,2,2,2,2,2};
    int ile_ruchow_przeciwnika_rozpatrywac[10]{2,2,2,2,2,2,2,2,2,2};

    //T_wsk_szachownica wsk_szachownica_wyjsciowa;// !!!!!!!!!
//***************************************************
    std::vector<std::vector<T_wsk_szachownica>> wektor_na_ruchy;
    std::vector<double>statusy_materialne_rozpatrywanych_rochow;
//***************************************************




    T_ruch();
    double oblicz_nastepny_ruch(T_wsk_szachownica wsk_X);
        void zweryfikuj_polozenia_krolow(T_wsk_szachownica wsk_X);
        void sprawdz_czy_wieze_i_krole_byly_ruszane(T_wsk_szachownica wsk_X);
        double znajdz_najleprze_z_posuniec(T_wsk_szachownica wsk_X);
            void wyzeruj_wektor_na_ruchy();
        T_ruch(T_ruch* wskaznik,double &status_materialny,T_wsk_szachownica wsk_X, int iteracja);
            void przepisz_dane_o_ruchach_krolow_i_wiez(T_ruch* wsk);
    void wykonaj_moj_ruch(T_wsk_szachownica wsk_X);
        void wykonaj_moj_ruch_jak_mozna(bool &czy_moj_krol_jest_szachowany, int &numer_mojego_ruchu, int &i, int &j, T_wsk_szachownica kopia, int &index_najgorszego_ruchu, double &najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
            bool czy_moje_pole_jest_bite(const int &x,const int &y, T_wsk_szachownica kopia);
            void odrzuc_najgorszy_ruch(int &numer_mojego_ruchu, int &index_najgorszego_ruchu, double &najmniejszy_status_materialny_sposrod_rozpatrywanych_ruchow);
    void wykonaj_ruch_przeciwnika(T_wsk_szachownica wsk_X, int &numer_mojego_ruchu);
        void wykonaj_ruch_przeciwnika_jak_mozna(bool &czy_krol_przeciwnika_jest_szachowany, int &numer_mojego_ruchu, int &i, int &j, T_wsk_szachownica kopia);
            bool czy_pole_przeciwnika_jest_bite(const int &x, const int &y, T_wsk_szachownica kopia);
            void odrzuc_najgorsze_ruchy_przeciwnika(int &numer_mojego_ruchu);
    double oblicz_status_materialny(const T_wsk_szachownica wsk_X);
    T_wsk_szachownica skopiuj_szachownice(T_wsk_szachownica oryginal);
    void wypisz_szachownice(const T_wsk_szachownica wsk_X);//??????????????????????????
};
#endif








