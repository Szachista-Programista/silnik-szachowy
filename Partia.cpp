#include <conio.h>
#include <cctype>
#include "Partia.h"
#include <iostream>//????????????????
using namespace std;


Partia::Partia(bool k): kolor{k}, szachownica{k}, silnik{k}, notatnik{k}{
//==============================================================================================================
}
void Partia::zagraj_z_urzytkownikiem(){
//==============================================================================================================
    szachownica.wypisz_tablica_szachownica();
    if(kolor == false) //maszyna zaczyna
        obsluga_ruchu_maszyny(10000);
    while(obsluga_ruchu_urzytkownika() && obsluga_ruchu_maszyny(ruch_urzytkownika)){}
    menu_zapisu_gry();
}
    bool Partia::obsluga_ruchu_urzytkownika(){
//==============================================================================================================
    while(true)
    {

        if((ruch_urzytkownika = wspolzedne_urzytkownika = pobierz_wspolzedne()) == -1)
            return false;
        kod_aktualnej_aktualizacji_szachownicy = kod_czastkowy = wygeneruj_kod_czastkowy();
        wylacz_podswietlenie_ubieglego_ruchu_maszyny();
        wybrano_prawidlowe_pole = czy_wybrano_prawidlowe_pole();
        zaktualizuj_szachownice(kod_czastkowy, true);
        if(!wybrano_prawidlowe_pole)
        {
            komunikat.obwieszczenie("to pole nie nalezy do ciebie", 1000);
            szachownica.usun_obwieszczenie(240);
            zaktualizuj_szachownice(kod_czastkowy, false);
            continue;
        }
        if(wybrano_prawidlowe_pole)
        {
            if((wspolzedne_urzytkownika = pobierz_wspolzedne()) == -1)
                return false;
            ruch_urzytkownika = ruch_urzytkownika*100 + wspolzedne_urzytkownika;
            kod_aktualnej_aktualizacji_szachownicy += kod_czastkowy = wygeneruj_kod_czastkowy();
            wykonano_prawidlowy_ruch = silnik.czy_urzytkownik_moze_wykonac_takie_posuniecie(ruch_urzytkownika);
            zaktualizuj_szachownice(kod_czastkowy, true);
            if(!wykonano_prawidlowy_ruch)
            {
                komunikat.obwieszczenie("nieprawidlowy ruch", 1000);
                szachownica.usun_obwieszczenie(170);
                zaktualizuj_szachownice(kod_aktualnej_aktualizacji_szachownicy, false);
                continue;
            }
            if(wykonano_prawidlowy_ruch)
            {
                if(czy_urzytkownik_dokonuje_promocji_pionka())
                {
                    vector<string> opcje_promocji{"1. Skoczek", "2. Goniec", "3. Wieza", "4. Hetman"};
                    kod_promocji = komunikat.pole_wyboru(opcje_promocji)*10000;
                    szachownica.usun_pole_wyboru(220, 60);
                    ruch_urzytkownika += kod_promocji;
                }
                notatnik.wygeneruj_i_wypisz_notacje(ruch_urzytkownika);
                kod_aktualnej_aktualizacji_szachownicy = notatnik.podaj_kod_aktualizacji_szachownicy();
                zaktualizuj_szachownice(kod_aktualnej_aktualizacji_szachownicy, true);
                kod_ubieglej_aktualizacji_szachownicy = kod_aktualnej_aktualizacji_szachownicy;
                return true;
            }
        }
    }
}
        int Partia::pobierz_wspolzedne(){//###
//==============================================================================================================
    string wspolzedne;
    char znak;
    bool wczytano_litere = false;

    while(true)
    {
        znak = getch();
        if(menu_zakonczenia_parti(znak))
            return -1;
        if('A'<=znak && znak<='H')
        {
            wspolzedne = znak;
            szachownica.usun_obwieszczenie(40);
            komunikat.obwieszczenie(wspolzedne, 0);
            wczytano_litere = true;
            continue;
        }
        if('a'<=znak && znak<='h')
        {
            znak = toupper(znak);
            wspolzedne = znak;
            szachownica.usun_obwieszczenie(40);
            komunikat.obwieszczenie(wspolzedne, 0);
            wczytano_litere = true;
            continue;
        }
        if('1'<=znak && znak<='8' && wczytano_litere == true)
        {
            wspolzedne += znak;
            komunikat.obwieszczenie(wspolzedne, 400);
            szachownica.usun_obwieszczenie(40);
            break;
        }
    }
    if(kolor == true)
    {
        int wsp_x = 7-(wspolzedne[0]-'A');
        int wsp_y =    wspolzedne[1]-'1';
        return wsp_x*10 + wsp_y;
    }
    if(kolor == false)
    {
        int wsp_x =    wspolzedne[0]-'A';
        int wsp_y = 7-(wspolzedne[1]-'1');
        return  wsp_x*10 + wsp_y;
    }
}
            bool Partia::menu_zakonczenia_parti(char znak){
//==============================================================================================================
    if(znak != 27)
        return false;
    vector<string> menu_zakonczenia_parti{"1. kontynuuj", "2. zakoncz partie"};
    switch(komunikat.pole_wyboru(menu_zakonczenia_parti))
   {
       case 1: szachownica.usun_pole_wyboru(210, 34); return false;
       case 2: szachownica.usun_pole_wyboru(210, 34); return true;
   }
}
        string Partia::wygeneruj_kod_czastkowy(){//xxx
//==============================================================================================================
    int x     = wspolzedne_urzytkownika / 10;
    int y     = wspolzedne_urzytkownika % 10;
    char znak = notatnik.aktualna_szachownica[y][x];
    return to_string(x) + to_string(y) + znak;
}
        void Partia::zaktualizuj_szachownice(string kod_aktualizacji, bool podswietlenie){//xxx
//==============================================================================================================
    string pojedyncze_polecenie;
    int x;
    int y;
    char znak;
    int bierka;
    bool kolor_figury;
    while(kod_aktualizacji.size())
    {
        pojedyncze_polecenie = kod_aktualizacji.substr(0, 3);
        kod_aktualizacji.erase(0, 3);

        x = 7-(pojedyncze_polecenie[0] - '0');
        y = 7-(pojedyncze_polecenie[1] - '0');
        znak = pojedyncze_polecenie[2];
        bierka = podaj_kod_bierki(znak);
        kolor_figury = podaj_kolor_bierki(znak);
        szachownica.zaktualizuj_pole(x, y, bierka, kolor_figury, podswietlenie);
    }
}
            int Partia::podaj_kod_bierki(char znak){//xxx
//==============================================================================================================
    switch(znak)
    {
        case ' ':           return 0;
        case 'P': case 'p': return 1;
        case 'S': case 's': return 2;
        case 'G': case 'g': return 3;
        case 'W': case 'w': return 4;
        case 'H': case 'h': return 5;
        case 'K': case 'k': return 6;
    }
}
            bool Partia::podaj_kolor_bierki(char znak){//xxx
//==============================================================================================================
    if(isalpha(znak))
    {
        if(islower(znak))
            return kolor;
        if(isupper(znak))
            return !kolor;
    }
    if(znak == ' ')
        return !kolor;
}
        void Partia::wylacz_podswietlenie_ubieglego_ruchu_maszyny(){//xxx
//==============================================================================================================
    if(ruch_maszyny_jest_podswietlony == true)
    {
        zaktualizuj_szachownice(kod_ubieglej_aktualizacji_szachownicy, false);
        ruch_maszyny_jest_podswietlony == false;
    }
}
        bool Partia::czy_wybrano_prawidlowe_pole(){//xxx
//==============================================================================================================
    int x     = wspolzedne_urzytkownika / 10;
    int y     = wspolzedne_urzytkownika % 10;
    char znak = notatnik.aktualna_szachownica[y][x];
    return (podaj_kolor_bierki(znak) == kolor);
}
        bool Partia::czy_urzytkownik_dokonuje_promocji_pionka(){//###
//==============================================================================================================
    int zX, zY, naY;
    zX  =  ruch_urzytkownika/1000;
    zY  = (ruch_urzytkownika/ 100)%10;
    naY =  ruch_urzytkownika      %10;
    if(notatnik.aktualna_szachownica[zY][zX] == 'p' && naY == 7)
        return true;
    else
        return false;
}
    bool Partia::obsluga_ruchu_maszyny(int komenda){//xxx
//==============================================================================================================
    ruch_maszyny = silnik.wykonaj_posuniecie(komenda);
    notatnik.wygeneruj_i_wypisz_notacje(ruch_maszyny);
    kod_aktualnej_aktualizacji_szachownicy = notatnik.podaj_kod_aktualizacji_szachownicy();
    zaktualizuj_szachownice(kod_ubieglej_aktualizacji_szachownicy, false);
    zaktualizuj_szachownice(kod_aktualnej_aktualizacji_szachownicy, true);
    ruch_maszyny_jest_podswietlony = true;
    kod_ubieglej_aktualizacji_szachownicy = kod_aktualnej_aktualizacji_szachownicy;
    return !czy_to_koniec_gry();
}
        bool Partia::czy_to_koniec_gry(){
//==============================================================================================================
    int parametr_dodatkowy = ruch_maszyny / 10000;
    if(!parametr_dodatkowy)
        return false;
    string informacja;
    switch(parametr_dodatkowy)
    {
        case 6:
            informacja = "WYGRALES";
            break;
        case 7:
            informacja = "spowodowales pata";
            break;
        case 8:
            informacja = "PRZEGRALES";
            break;
        case 9:
            informacja = "maszyna spowodowala pata";
            break;
    }
    komunikat.obwieszczenie(informacja, 2000);
    szachownica.usun_obwieszczenie(240);
    return true;
}
    void Partia::menu_zapisu_gry(){
//==============================================================================================================

    komunikat.obwieszczenie("czy zapisac notacje w notatniku", 2000);
    szachownica.usun_obwieszczenie(280);

    vector<string> menu_zapisu{"1. tak", "2. nie"};
    switch(komunikat.pole_wyboru(menu_zapisu))
    {
       case 1:  notatnik.zapisz_gre_w_notatniku(); break;
       case 2:  break;
       default: break;
    }
    szachownica.usun_pole_wyboru(210, 40);



}

















