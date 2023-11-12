#include <conio.h>
#include <cctype>
#include "Gra.h"
using namespace std;






Gra::Gra(bool k): kolor{k}, szachownica{k}, silnik{k}, notatnik{k}{
//==============================================================================================================
}
void Gra::zagraj_z_urzytkownikiem(){
//==============================================================================================================
    if(kolor == false) //maszyna zaczyna
        obsluga_ruchu_maszyny(10000);
    for( ; ; )
    {
        obsluga_ruchu_urzytkownika();
        obsluga_ruchu_maszyny(ruch_urzytkownika);
    }
}
    void Gra::zaktualizuj_szachownice(string kod_aktualizacji, bool podswietlenie){
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
        x = odwroc_wspolzedne( isdigit( pojedyncze_polecenie[0] ));
        y = odwroc_wspolzedne( isdigit( pojedyncze_polecenie[1] ));
        znak = pojedyncze_polecenie[2];
        bierka = podaj_kod_bierki(znak);
        kolor_figury = podaj_kolor_bierki(znak);
        szachownica.zaktualizuj_pole(x, y, bierka, kolor_figury, podswietlenie);
    }
}
        int Gra::podaj_kod_bierki(char znak){
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
        default: return 7;
    }
}
        bool Gra::podaj_kolor_bierki(char znak){
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
    void Gra::obsluga_ruchu_urzytkownika(){
//==============================================================================================================
    ruch_jest_podswietlony = true;
    while(true)
    {
        ruch_urzytkownika = wspolzedne_urzytkownika = pobierz_wspolzedne();
        kod_aktualnej_aktualizacji_szachownicy = kod_czastkowy = wygeneruj_kod_czastkowy();
        wylacz_podswietlenie_ubieglego_ruchu();
        wybrano_prawidlowe_pole = czy_wybrano_prawidlowe_pole();
        zaktualizuj_szachownice(kod_czastkowy, true);
        if(!wybrano_prawidlowe_pole)
        {
            komunikat.obwieszczenie("to pole nie nalezy do ciebie", 500);
            szachownica.usun_obwieszczenie(170);
            zaktualizuj_szachownice(kod_czastkowy, false);
            continue;
        }
        if(wybrano_prawidlowe_pole)
        {
            wspolzedne_urzytkownika = pobierz_wspolzedne();
            ruch_urzytkownika = ruch_urzytkownika*100 + wspolzedne_urzytkownika;
            kod_aktualnej_aktualizacji_szachownicy += kod_czastkowy = wygeneruj_kod_czastkowy();
            wykonano_prawidlowy_ruch = silnik.czy_urzytkownik_moze_wykonac_takie_posuniecie(ruch_urzytkownika);
            zaktualizuj_szachownice(kod_czastkowy, true);
            if(!wykonano_prawidlowy_ruch)
            {
                komunikat.obwieszczenie("nieprawidlowy ruch", 500);
                szachownica.usun_obwieszczenie(170);
                zaktualizuj_szachownice(kod_aktualnej_aktualizacji_szachownicy, false);
                continue;
            }
            if(wykonano_prawidlowy_ruch)
            {
                if(czy_urzytkownik_dokonuje_promocji_pionka())
                {
                    vector<string> opcje_promocji{"Skoczek", "Goniec", "Wieza", "Krol"};
                    kod_promocji = komunikat.pole_wyboru(opcje_promocji)*10000;
                    ruch_urzytkownika += kod_promocji;
                }
                notatnik.wygeneruj_i_wypisz_notacje(ruch_urzytkownika);
                kod_aktualnej_aktualizacji_szachownicy = notatnik.podaj_kod_aktualizacji_szachownicy();
                zaktualizuj_szachownice(kod_aktualnej_aktualizacji_szachownicy, true);
                kod_ubieglej_aktualizacji_szachownicy = kod_aktualnej_aktualizacji_szachownicy;
                break;
            }
        }
    }
}
        int Gra::pobierz_wspolzedne(){
//==============================================================================================================
    string wspolzedne;
    char znak;
    bool wczytano_litere = false;

    while(true)
    {
        znak = getch();
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
            znak -= 32;
            wspolzedne = znak;
            szachownica.usun_obwieszczenie(40);
            komunikat.obwieszczenie(wspolzedne, 0);
            wczytano_litere = true;
            continue;
        }
        if(wczytano_litere == true && '1'<=znak && znak<='8')
        {
            wspolzedne += znak;
            komunikat.obwieszczenie(wspolzedne, 400);
            szachownica.usun_obwieszczenie(40);
            break;
        }
    }
    if(kolor == true)
    {
        int wsp_x = wspolzedne[0]-65;
        int wsp_y = 8-isdigit(wspolzedne[1]);
        return  odwroc_wspolzedne(wsp_x*10 + wsp_y);
    }
    if(kolor == false)
    {
        int wsp_x = 8-(wspolzedne[0]-65);
        int wsp_y = isdigit(wspolzedne[1]);
        return  odwroc_wspolzedne(wsp_x*10 + wsp_y);
    }
}
            int Gra::odwroc_wspolzedne(int wspolzedne){
//==============================================================================================================
    return 77-wspolzedne;
}
        string Gra::wygeneruj_kod_czastkowy(){
//==============================================================================================================
    int x     = wspolzedne_urzytkownika / 10;
    int y     = wspolzedne_urzytkownika % 10;
    char znak = notatnik.aktualna_szachownica[y][x];
    return to_string(wspolzedne_urzytkownika) + znak;
}
        void Gra::wylacz_podswietlenie_ubieglego_ruchu(){
//==============================================================================================================
    if(ruch_jest_podswietlony == true)
    {
        zaktualizuj_szachownice(kod_ubieglej_aktualizacji_szachownicy, false);
        ruch_jest_podswietlony == false;
    }
}
        bool Gra::czy_wybrano_prawidlowe_pole(){
//==============================================================================================================
    int x     = wspolzedne_urzytkownika / 10;
    int y     = wspolzedne_urzytkownika % 10;
    char znak = notatnik.aktualna_szachownica[y][x];
    return (podaj_kolor_bierki(znak) == kolor);
}
        bool Gra::czy_urzytkownik_dokonuje_promocji_pionka(){
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
    void Gra::obsluga_ruchu_maszyny(int komenda){
//==============================================================================================================
    ruch_maszyny = silnik.wykonaj_posuniecie(komenda);
    notatnik.wygeneruj_i_wypisz_notacje(ruch_maszyny);
    kod_aktualnej_aktualizacji_szachownicy = notatnik.podaj_kod_aktualizacji_szachownicy();
    zaktualizuj_szachownice(kod_ubieglej_aktualizacji_szachownicy, false);
    zaktualizuj_szachownice(kod_aktualnej_aktualizacji_szachownicy, true);
    kod_ubieglej_aktualizacji_szachownicy = kod_aktualnej_aktualizacji_szachownicy;
}
int Gra::odwroc_ruch(int ruch){//?????????
//==============================================================================================================
    return 7777-ruch;
}




