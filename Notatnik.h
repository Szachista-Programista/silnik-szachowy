#ifndef Notatnik_H
#define Notatnik_H
using T_wsk_szachownica = char(*)[8];




class Notatnik
{

    string notacja;

    int krol_maszyny_x,
        krol_maszyny_y,
        krol_urzytkownika_x,
        krol_urzytkownika_y,
        ruch_zX,
        ruch_zY,
        ruch_naX,
        ruch_naY,
        parametr_dodatkowy;










    T_wsk_szachownica szachownica;

    Notatnik();
    ~Notatnik();
    T_wsk_szachownica zainicjalizuj_szachownice();



    string podaj_notacje_tego_ruchu(int ruch);



    void rozszyfruj_ruch(int ruch);



    void zaktualizuj_polozenia_krolow();



    bool czy_jest_szach_dla_urzytkownika();
    bool czy_jest_szach_dla_maszyny();























};
#endif
