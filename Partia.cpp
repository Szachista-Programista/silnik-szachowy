#include "Partia.h"
using namespace std;
using namespace globalType;

Partia::Partia(bool k)noexcept: kolor{k}, szachownica{k}, silnik{k}, notatnik{k}{
//==============================================================================================================
}
void Partia::zagraj_z_urzytkownikiem(){//*4
//==============================================================================================================
    try{
        notatnik.wyczysc_kolumne(0);
        notatnik.wyczysc_kolumne(1);
        notatnik.wyczysc_kolumne(2);
        szachownica.wypisz_tablica_szachownica();

        if(kolor == false) //maszyna zaczyna
            obsluga_ruchu_maszyny(10000);
        while(obsluga_ruchu_urzytkownika() && obsluga_ruchu_maszyny(ruch_urzytkownika)){}
        menu_zapisu_gry();
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
    bool Partia::obsluga_ruchu_urzytkownika(){//*3
//==============================================================================================================
    try{
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
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        int Partia::pobierz_wspolzedne(){//*2
//==============================================================================================================
    try{
        string wspolzedne;
        char znak;
        bool wczytano_litere = false;

        while(true)
        {
            znak = SystemInfo::getChar();
            if(menu_zakonczenia_parti(znak))
                return -1;
            if('A'<=znak && znak<='H')
            {
                wspolzedne = znak;
                if(znak == 'A')
                    szachownica.usun_obwieszczenie(30);
                else
                    szachownica.usun_obwieszczenie(28);
                komunikat.obwieszczenie(wspolzedne, 1);
                wczytano_litere = true;
                continue;
            }
            if('a'<=znak && znak<='h')
            {
                znak = toupper(znak);
                wspolzedne = znak;
                if(znak == 'A')
                    szachownica.usun_obwieszczenie(30);
                else
                    szachownica.usun_obwieszczenie(28);
                komunikat.obwieszczenie(wspolzedne, 1);
                wczytano_litere = true;
                continue;
            }
            if('1'<=znak && znak<='8' && wczytano_litere == true)
            {
                wspolzedne += znak;
                komunikat.obwieszczenie(wspolzedne, 400);
                if(wspolzedne[0] == 'A')
                    szachownica.usun_obwieszczenie(40);
                else
                    szachownica.usun_obwieszczenie(38);
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
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
            bool Partia::menu_zakonczenia_parti(char znak){//*1
//==============================================================================================================
    try{
        if(znak != 27)
            return false;
        vector<string> menu_zakonczenia_parti{"1. kontynuuj", "2. ustawienia kolorow", "3. zakoncz partie"};
        bool x = true;
        while(x)
            switch(komunikat.pole_wyboru(menu_zakonczenia_parti))
           {
               case 1: szachownica.usun_pole_wyboru(205, 43); x = false; return false;
               case 2: menu_ustawien(); break;
               case 3: szachownica.usun_pole_wyboru(205, 43); x = false; return true;
               default: throw invalid_argument("Blad wyboru opcji.");
           }
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
                void Partia::menu_ustawien(){//*0
//==============================================================================================================
    try{
        vector<string> menu_kolorowego_elementu{"1. kolor zaznaczonego pola", "2. kolor menu/komunikatow", "3. kolor wybieranej opcji w menu", "4. kolor notacji"};
        vector<string> menu_kolorow{"1. czerwony", "2. zielony", "3. zolty", "4. niebieski"};

        int wybrany_element = komunikat.pole_wyboru(menu_kolorowego_elementu);
        szachownica.usun_pole_wyboru(293, 53);

        SystemInfo::Kolor wybrany_kolor = static_cast<SystemInfo::Kolor>(komunikat.pole_wyboru(menu_kolorow));
        switch(wybrany_element)
       {
           case 1:  SystemInfo::zaznaczone_pole = wybrany_kolor; break;
           case 2:  SystemInfo::menu            = wybrany_kolor; break;
           case 3:  SystemInfo::wybierana_opcja = wybrany_kolor; break;
           case 4:  SystemInfo::notacja         = wybrany_kolor; break;
           default: throw invalid_argument("Blad wyboru opcji.");
       }
       szachownica.usun_pole_wyboru(205, 53);
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        string Partia::wygeneruj_kod_czastkowy()noexcept{
//==============================================================================================================
    int x     = wspolzedne_urzytkownika / 10;
    int y     = wspolzedne_urzytkownika % 10;
    char znak = notatnik.aktualna_szachownica[y][x];
    return to_string(x) + to_string(y) + znak;
}
        void Partia::zaktualizuj_szachownice(string kod_aktualizacji, bool podswietlenie){//1
//==============================================================================================================
    try{
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
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
            int Partia::podaj_kod_bierki(char znak){//0+
//==============================================================================================================
    try{
        switch(znak)
        {
            case ' ':           return 0;
            case 'P': case 'p': return 1;
            case 'S': case 's': return 2;
            case 'G': case 'g': return 3;
            case 'W': case 'w': return 4;
            case 'H': case 'h': return 5;
            case 'K': case 'k': return 6;
            default: throw invalid_argument("Nieprawidlowy znak.");
        }
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
            bool Partia::podaj_kolor_bierki(char znak){//0+
//==============================================================================================================
    try{
        if(isalpha(znak))
        {
            if(islower(znak))
                return kolor;
            if(isupper(znak))
                return !kolor;
        }
        else if(znak == ' ')
            return !kolor;
        else
            throw invalid_argument("Nieprawid³owy znak.");
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
        void Partia::wylacz_podswietlenie_ubieglego_ruchu_maszyny(){//2
//==============================================================================================================
    try{
        if(ruch_maszyny_jest_podswietlony == true)
        {
            zaktualizuj_szachownice(kod_ubieglej_aktualizacji_szachownicy, false);
            ruch_maszyny_jest_podswietlony = false;
        }
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        bool Partia::czy_wybrano_prawidlowe_pole(){//1
//==============================================================================================================
    try{
        int x     = wspolzedne_urzytkownika / 10;
        int y     = wspolzedne_urzytkownika % 10;
        char znak = notatnik.aktualna_szachownica[y][x];
        return (podaj_kolor_bierki(znak) == kolor);
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        bool Partia::czy_urzytkownik_dokonuje_promocji_pionka()noexcept{
//==============================================================================================================
    int zX, zY, naY;
    zX  =  ruch_urzytkownika/1000;
    zY  = (ruch_urzytkownika/ 100)%10;
    naY =  ruch_urzytkownika      %10;
    return (notatnik.aktualna_szachownica[zY][zX] == 'p' && naY == 7);
}
    bool Partia::obsluga_ruchu_maszyny(int komenda){//*1
//==============================================================================================================
    try{
        ruch_maszyny = silnik.wykonaj_posuniecie(komenda);
        notatnik.wygeneruj_i_wypisz_notacje(ruch_maszyny);
        kod_aktualnej_aktualizacji_szachownicy = notatnik.podaj_kod_aktualizacji_szachownicy();
        zaktualizuj_szachownice(kod_ubieglej_aktualizacji_szachownicy, false);
        zaktualizuj_szachownice(kod_aktualnej_aktualizacji_szachownicy, true);
        ruch_maszyny_jest_podswietlony = true;
        kod_ubieglej_aktualizacji_szachownicy = kod_aktualnej_aktualizacji_szachownicy;
        return !czy_to_koniec_gry();
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        bool Partia::czy_to_koniec_gry(){//*0
//==============================================================================================================
    try{
        int parametr_dodatkowy = ruch_maszyny / 10000;
        if(!parametr_dodatkowy)
            return false;
        string informacja;
        switch(parametr_dodatkowy)
        {
            case 6:
                informacja = "WYGRALES...";
                break;
            case 7:
                informacja = "spowodowales pata...";
                break;
            case 8:
                informacja = "PRZEGRALES...";
                break;
            case 9:
                informacja = "maszyna spowodowala pata...";
                break;
            default: throw invalid_argument("Nieprawidlowy parametr dodatkowy.");
        }
        komunikat.obwieszczenie(informacja);
        szachownica.usun_obwieszczenie(260);
        return true;
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
    void Partia::menu_zapisu_gry(){//*0
//==============================================================================================================
    try{
        komunikat.obwieszczenie("czy zapisac notacje w notatniku...");
        szachownica.usun_obwieszczenie(300);
        vector<string> menu_zapisu{"1. tak", "2. nie"};
        switch(komunikat.pole_wyboru(menu_zapisu))
        {
           case 1:  notatnik.zapisz_gre_w_notatniku(); break;
           case 2:  break;
           default: throw invalid_argument("Blad wyboru opcji.");
        }
        szachownica.usun_pole_wyboru(210, 40);
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}

















