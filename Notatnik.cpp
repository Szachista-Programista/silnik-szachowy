#include <algorithm>
#include <string>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <sstream>
#include <iostream>
#include <windows.h>
#include "Notatnik.h"
using namespace std;
using T_wsk_szachownica = char(*)[8];




Notatnik::Notatnik(bool k): kolor{k}{
//==============================================================================================================
    aktualna_szachownica = zainicjalizuj_szachownice();
    ubiegla_szachownica  = zainicjalizuj_szachownice();
    wczytaj_znaki();
}
    T_wsk_szachownica Notatnik::zainicjalizuj_szachownice(){//@@@@
//==============================================================================================================
    /*return new char[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                          {'p','p','p','p','p','p','p','p'},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {'P','P','P','P','P','P','P','P'},
                          {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};*/
    return new char[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                          {'p','p','p','p','p','p','p','p'},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {'P','P','P','P','P','P','P','P'},
                          {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};
   /*return new char[8][8]{{'k',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ','w',' ',' ',' ',' ',' '},
                          {' ','w',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {' ',' ',' ',' ',' ',' ',' ',' '},
                          {'K',' ',' ',' ',' ',' ',' ',' '}};*/
}
    void Notatnik::wczytaj_znaki(){
//==============================================================================================================
    ifstream odczyt;
    odczyt.open("znaki.txt");
    string znak;
    for(int i=0; i<69; i++)
    {
        getline(odczyt, znak);
        znak.erase(0, 2);
        for(int j=0, k=0; j<9; j++, k++)
        {
            for(; znak[k] != '$'; k++)
                znaki[i][j] += znak[k];
        }
    }
    odczyt.close();
}
Notatnik::~Notatnik(){
//==============================================================================================================
    delete[]aktualna_szachownica;
    delete[]ubiegla_szachownica;
}
void Notatnik::wygeneruj_i_wypisz_notacje(int ruch){
//==============================================================================================================
    uaktualnij_wartosci_parametrow(ruch);
    nanies_ruch_na_szachownice_i_zapisz_notacje();
    uzupelnij_zapis_gry();
    wypisz_notacje();
}
    void Notatnik::uaktualnij_wartosci_parametrow(int ruch){
//==============================================================================================================
    notacja.clear();
    kod_ruchu = ruch;
    nr_ruchu++;
    nr_pary_ruchow = (nr_ruchu+1)/2;
    ruch_maszyny = kolor ^ nr_ruchu%2;
    naniesiono_ruch = false;
    rozszyfruj_ruch();
    zaktualizuj_polozenia_krolow();
}
        void Notatnik::rozszyfruj_ruch(){
//==============================================================================================================
    ruch_naY = kod_ruchu % 10;
    kod_ruchu /=10;
    ruch_naX = kod_ruchu % 10;
    kod_ruchu /=10;
    ruch_zY  = kod_ruchu % 10;
    kod_ruchu /=10;
    ruch_zX  = kod_ruchu % 10;
    kod_ruchu /=10;
    parametr_dodatkowy = kod_ruchu;
    if(6<=parametr_dodatkowy && parametr_dodatkowy<=9)
        koniec_gry = true;
}
        void Notatnik::zaktualizuj_polozenia_krolow(){
//==============================================================================================================
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            if(aktualna_szachownica[i][j]=='K')
            {
                krol_maszyny_x=j;
                krol_maszyny_y=i;
            }
            if(aktualna_szachownica[i][j]=='k')
            {
                krol_urzytkownika_x=j;
                krol_urzytkownika_y=i;
            }
        }
}
    void Notatnik::nanies_ruch_na_szachownice_i_zapisz_notacje(){
//==============================================================================================================
    if(parametr_dodatkowy == 6 || parametr_dodatkowy == 7)//urzytkownik zakonczyl gre
        return;
    nanies_roszade();
    nanies_promocje_pionka();
    nanies_bicie_w_przelocie();
    nanies_zwykly_ruch();
    if(parametr_dodatkowy == 8) // maszyna spowodowala mata
         notacja += '#';
    else if(czy_jest_szach())
        notacja += '+';
}
        void Notatnik::nanies_roszade(){
//==============================================================================================================
    if(aktualna_szachownica[ruch_zY][ruch_zX] == (ruch_maszyny?'K':'k') && abs(ruch_zX-ruch_naX)==2)//roszada
    {
        naniesiono_ruch = true;
        aktualna_szachownica[ruch_zY][ruch_zX]   = ' ';
        aktualna_szachownica[ruch_naY][ruch_naX] = (ruch_maszyny?'K':'k');
        if(ruch_zX == 3 && ruch_naX == 1) // O-O
        {
            aktualna_szachownica[ruch_zY][0] = ' ';
            aktualna_szachownica[ruch_zY][2] = (ruch_maszyny?'W':'w');
            notacja = "O-O";
        }
        if(ruch_zX == 3 && ruch_naX == 5) // O-O-O
        {
            aktualna_szachownica[ruch_zY][7] = ' ';
            aktualna_szachownica[ruch_zY][4] = (ruch_maszyny?'W':'w');
            notacja = "O-O-O";
        }
        if(ruch_zX == 4 && ruch_naX == 2) // O-O-O
        {
            aktualna_szachownica[ruch_zY][0] = ' ';
            aktualna_szachownica[ruch_zY][3] = (ruch_maszyny?'W':'w');
            notacja = "O-O-O";
        }
        if(ruch_zX == 4 && ruch_naX == 6) // O-O
        {
            aktualna_szachownica[ruch_zY][7] = ' ';
            aktualna_szachownica[ruch_zY][5] = (ruch_maszyny?'W':'w');
            notacja = "O-O";
        }
    }
}
        void Notatnik::nanies_promocje_pionka(){
//==============================================================================================================
    if(1 <= parametr_dodatkowy && parametr_dodatkowy <= 4) //promocja
    {
        naniesiono_ruch = true;
        aktualna_szachownica[ruch_zY][ruch_zX] = ' ';
        if(aktualna_szachownica[ruch_naY][ruch_naX] != ' ')//promocja z biciem
        {
            notacja += podaj_wspolrzedne(ruch_zY, ruch_zY);
            notacja.pop_back();
            notacja += 'x';
        }
        notacja += podaj_wspolrzedne(ruch_naY, ruch_naY);
        notacja += '=';
        switch(parametr_dodatkowy)
        {
            case 1: notacja += 'S'; aktualna_szachownica[ruch_naY][ruch_naX] = (ruch_maszyny?'S':'s'); break;
            case 2: notacja += 'G'; aktualna_szachownica[ruch_naY][ruch_naX] = (ruch_maszyny?'G':'g'); break;
            case 3: notacja += 'W'; aktualna_szachownica[ruch_naY][ruch_naX] = (ruch_maszyny?'W':'w'); break;
            case 4: notacja += 'H'; aktualna_szachownica[ruch_naY][ruch_naX] = (ruch_maszyny?'H':'h'); break;
            default: break;
        }
    }
}
        void Notatnik::nanies_bicie_w_przelocie(){
//==============================================================================================================
    if(aktualna_szachownica[ruch_zY][ruch_zX] == (ruch_maszyny?'P':'p') && aktualna_szachownica[ruch_naY][ruch_naX] == ' ' && abs(ruch_zX-ruch_naX) == 1)
    {
        naniesiono_ruch = true;
        notacja += podaj_wspolrzedne(ruch_zX, ruch_zY);
        notacja.pop_back();
        notacja += 'x';
        notacja += podaj_wspolrzedne(ruch_naX, ruch_naY);
        aktualna_szachownica[ruch_zY][ruch_zX]   = ' ';
        aktualna_szachownica[ruch_naY][ruch_naX] = (ruch_maszyny?'P':'p');
        aktualna_szachownica[ruch_zY][ruch_naX]  = ' ';
    }
}
        void Notatnik::nanies_zwykly_ruch(){
//==============================================================================================================
    if( ! naniesiono_ruch)
    {
        char ruszana_bierka =  aktualna_szachownica[ruch_zY][ruch_zX];
        bool ruch_z_biciem  = (aktualna_szachownica[ruch_naY][ruch_naX] == ' ')? false: true;

        switch(ruszana_bierka)
        {
            case 'P': case 'p':
                if(ruch_z_biciem)
                {
                    notacja += podaj_wspolrzedne(ruch_zX, ruch_zY);
                    notacja.pop_back();
                }
                break;
            case 'S': case 's':
            {
                notacja += 'S';

                int drugi_skoczek_X,
                    drugi_skoczek_Y;
                bool czy_jest_drugi_skoczek = false,
                     czy_drugi_skoczek_jest_blizniaczy = false;
                for(int i=0; i<8; i++)
                    for(int j=0; j<8; j++)
                        if(!(i == ruch_zY && j == ruch_zX) && aktualna_szachownica[i][j] == ruszana_bierka)
                        {
                            czy_jest_drugi_skoczek = true;
                            drugi_skoczek_X = j;
                            drugi_skoczek_Y = i;
                        }
                if(czy_jest_drugi_skoczek)
                {
                    if((abs(ruch_naX-drugi_skoczek_X) == 1 && abs(ruch_naY-drugi_skoczek_Y) == 2)
                    || (abs(ruch_naX-drugi_skoczek_X) == 2 && abs(ruch_naY-drugi_skoczek_Y) == 1))
                        czy_drugi_skoczek_jest_blizniaczy = true;

                    if(czy_drugi_skoczek_jest_blizniaczy)
                    {
                        if(ruch_zX == drugi_skoczek_X)
                        {
                            notacja += podaj_wspolrzedne(ruch_zX, ruch_zY);
                            notacja.erase(1,1);
                        }
                        else
                        {
                            notacja += podaj_wspolrzedne(ruch_zX, ruch_zY);
                            notacja.pop_back();
                        }
                    }
                }
                break;
            }
            case 'G': case 'g':
                notacja += 'G';
                break;
            case 'W': case 'w':
                notacja += 'W';
                if(ruch_zY == ruch_naY)  //ruch w poziomie
                    for(int i=ruch_naX+((ruch_zX<ruch_naX)? +1: -1); 0<=i && i<=7; (ruch_zX<ruch_naX)? i++: i--)
                    {
                        if(aktualna_szachownica[ruch_zY][i] == ' ')
                            continue;
                        if(aktualna_szachownica[ruch_zY][i] == ruszana_bierka)
                        {
                            notacja += podaj_wspolrzedne(ruch_zX, ruch_zY);
                            notacja.pop_back();
                        }
                        else
                            break;
                    }
                if(ruch_zX == ruch_naX)  //ruch w pionie
                    for(int i=ruch_naY+((ruch_zY<ruch_naY)? +1: -1); 0<=i && i <=7; (ruch_zY<ruch_naY)? i++: i--)
                    {
                        if(aktualna_szachownica[i][ruch_zX] == ' ')
                            continue;
                        if(aktualna_szachownica[i][ruch_zX] == ruszana_bierka)
                        {
                            notacja += podaj_wspolrzedne(ruch_zX, ruch_zY);
                            notacja.erase(1,1);
                        }
                        else
                            break;
                    }
                break;
            case 'H': case 'h':
                notacja += 'H';
                break;
            case 'K': case 'k':
                notacja += 'K';
                break;
            default: break;
        }
        if(ruch_z_biciem)
            notacja += 'x';
        notacja += podaj_wspolrzedne(ruch_naX, ruch_naY);
        aktualna_szachownica[ruch_naY][ruch_naX] = ruszana_bierka;
        aktualna_szachownica[ruch_zY][ruch_zX]   = ' ';
    }
}
            string Notatnik::podaj_wspolrzedne(int x, int y){
//==============================================================================================================
    string wspolrzedne;
    if(kolor)
    {
        wspolrzedne += 'a'+abs(x-7);
        wspolrzedne += to_string(y+1);
    }
    if(!kolor)
    {
        wspolrzedne += 'a'+x;
        wspolrzedne += to_string(abs(y-8));
    }
    return wspolrzedne;
}
        bool Notatnik::czy_jest_szach(){
//==============================================================================================================
    int x = ((!ruch_maszyny)? krol_maszyny_x: krol_urzytkownika_x);
    int y = ((!ruch_maszyny)? krol_maszyny_y: krol_urzytkownika_y);

    for(int i=1; 0<=y-i; i++)                                       //12:00 //szach od hetmana/wiezy/gonca
    {
        if(aktualna_szachownica[y-i][x]==' ') continue;
        if(aktualna_szachownica[y-i][x]==(ruch_maszyny? 'H': 'h') || aktualna_szachownica[y-i][x]==(ruch_maszyny? 'W': 'w')) return true;
        else break;
    }
    for(int i=1; 0<=y-i && x+i<=7; i++)                             //1:30 <<== kierunek poszykiwan szachujacej higury
    {
        if(aktualna_szachownica[y-i][x+i]==' ') continue;
        if(aktualna_szachownica[y-i][x+i]==(ruch_maszyny? 'H': 'h') || aktualna_szachownica[y-i][x+i]==(ruch_maszyny? 'G': 'g')) return true;
        else break;
    }
    for(int i=1; x+i<=7; i++)                                       //3:00
    {
        if(aktualna_szachownica[y][x+i]==' ') continue;
        if(aktualna_szachownica[y][x+i]==(ruch_maszyny? 'H': 'h') || aktualna_szachownica[y][x+i]==(ruch_maszyny? 'W': 'w')) return true;
        else break;
    }
    for(int i=1; y+i<=7 && x+i<=7; i++)                             //4:30
    {
        if(aktualna_szachownica[y+i][x+i]==' ') continue;
        if(aktualna_szachownica[y+i][x+i]==(ruch_maszyny? 'H': 'h') || aktualna_szachownica[y+i][x+i]==(ruch_maszyny? 'G': 'g')) return true;
        else break;
    }
    for(int i=1; y+i<=7; i++)                                       //6:00
    {
        if(aktualna_szachownica[y+i][x]==' ') continue;
        if(aktualna_szachownica[y+i][x]==(ruch_maszyny? 'H': 'h') || aktualna_szachownica[y+i][x]==(ruch_maszyny? 'W': 'w')) return true;
        else break;
    }
    for(int i=1; y+i<=7 && 0<=x-i; i++)                             //7:30
    {
        if(aktualna_szachownica[y+i][x-i]==' ') continue;
        if(aktualna_szachownica[y+i][x-i]==(ruch_maszyny? 'H': 'h') || aktualna_szachownica[y+i][x-i]==(ruch_maszyny? 'G': 'g')) return true;
        else break;
    }
    for(int i=1; 0<=x-i; i++)                                       //9:00
    {
        if(aktualna_szachownica[y][x-i]==' ') continue;
        if(aktualna_szachownica[y][x-i]==(ruch_maszyny? 'H': 'h') || aktualna_szachownica[y][x-i]==(ruch_maszyny? 'W': 'w')) return true;
        else break;
    }
    for(int i=1; 0<=y-i && 0<=x-i; i++)                             //10:30
    {
        if(aktualna_szachownica[y-i][x-i]==' ') continue;
        if(aktualna_szachownica[y-i][x-i]==(ruch_maszyny? 'H': 'h') || aktualna_szachownica[y-i][x-i]==(ruch_maszyny? 'G': 'g')) return true;
        else break;
    }

    if(0<=y-2 && x+1<=7 && aktualna_szachownica[y-2][x+1]==(ruch_maszyny? 'S': 's')) return true;                       //szach od skoczka
    if(0<=y-1 && x+2<=7 && aktualna_szachownica[y-1][x+2]==(ruch_maszyny? 'S': 's')) return true;
    if(y+1<=7 && x+2<=7 && aktualna_szachownica[y+1][x+2]==(ruch_maszyny? 'S': 's')) return true;
    if(y+2<=7 && x+1<=7 && aktualna_szachownica[y+2][x+1]==(ruch_maszyny? 'S': 's')) return true;
    if(y+2<=7 && 0<=x-1 && aktualna_szachownica[y+2][x-1]==(ruch_maszyny? 'S': 's')) return true;
    if(y+1<=7 && 0<=x-2 && aktualna_szachownica[y+1][x-2]==(ruch_maszyny? 'S': 's')) return true;
    if(0<=y-1 && 0<=x-2 && aktualna_szachownica[y-1][x-2]==(ruch_maszyny? 'S': 's')) return true;
    if(0<=y-2 && 0<=x-1 && aktualna_szachownica[y-2][x-1]==(ruch_maszyny? 'S': 's')) return true;

    if(ruch_maszyny && y<=5)//szach od pionka
    {
        if(1<=x && aktualna_szachownica[y+1][x-1] == 'P') return true;
        if(x<=6 && aktualna_szachownica[y+1][x+1] == 'P') return true;
    }
    if(!ruch_maszyny && 2<=y)
    {
        if(1<=x && aktualna_szachownica[y-1][x-1] == 'p') return true;
        if(x<=6 && aktualna_szachownica[y-1][x+1] == 'p') return true;
    }

    return false;
}
    void Notatnik::uzupelnij_zapis_gry(){
//==============================================================================================================
    ostringstream zapisek;
    if(koniec_gry)
        obsluga_konca_gry();
    if(nr_ruchu % 2)
    {
        zapisek<<right<<setw(3)<<nr_pary_ruchow<<". "<<left<<setw(8)<<notacja;
        if(parametr_dodatkowy == 8 || parametr_dodatkowy == 9)
            zapisek<<left<<setw(7)<<wynik_gry;
        zapis_calej_gry.push_back(zapisek.str());
    }
    else
    {
        zapisek<<left<<setw(7)<<notacja;
        zapis_calej_gry[nr_pary_ruchow-1] += zapisek.str();
        if(parametr_dodatkowy == 8 || parametr_dodatkowy == 9)
        {
            zapisek.str("");
            zapisek<<right<<setw(3)<<nr_pary_ruchow + 1<<". "<<left<<setw(8)<<wynik_gry;
            zapis_calej_gry.push_back(zapisek.str());
        }
    }
}
        void Notatnik::obsluga_konca_gry(){
//==============================================================================================================
    switch(parametr_dodatkowy)
    {
        case 6://wygrana urzytkownika
            zamien_plus_na_hasztag();
            notacja = (nr_ruchu%2)? "0.1": "1.0";
            break;
        case 7://pat urzytkownika
            notacja = "1/2-1/2";
            break;
        case 8://wygrana maszyny
            wynik_gry = (nr_ruchu%2)? "1.0": "0.1";
            break;
        case 9://pat maszyny
            wynik_gry = "1/2-1/2";
            break;
    }
}
            void Notatnik::zamien_plus_na_hasztag(){
//==============================================================================================================
        int index_edytowanego_stringa = nr_pary_ruchow - ((nr_ruchu % 2)? 2: 1);
        int index_plusa = zapis_calej_gry[index_edytowanego_stringa].rfind('+');
        zapis_calej_gry[index_edytowanego_stringa][index_plusa] = '#';
}
    void Notatnik::wypisz_notacje(){//###eee
//==============================================================================================================
    wyczysc_tablica_notacja();
    if(nr_ruchu%2)
        wypisz_ubiegla_notacje(ubiegly_wiersz_notacji, ubiegla_kolumna_notacji);
    dodaj_tresc_do_tablica_notacja(zapis_calej_gry[nr_pary_ruchow-1]);
    wypisz_tablica_notacja(aktualny_wiersz_notacji, aktualna_kolumna_notacji, true);
    if(!(nr_ruchu%2))
    {
        przepisz_tablice_do_kolumny();
        ubiegla_kolumna_notacji = aktualna_kolumna_notacji;
        ubiegly_wiersz_notacji  = aktualny_wiersz_notacji;
        if(aktualny_wiersz_notacji < 15)
            aktualny_wiersz_notacji++;
        else
        {
            aktualny_wiersz_notacji = 0;
            aktualna_kolumna_notacji++;
            if(aktualna_kolumna_notacji == 3)
                cofnij_kolumny();
        }
        if(parametr_dodatkowy == 8 || parametr_dodatkowy == 9)
        {
            parametr_dodatkowy = 0;
            nr_ruchu++;
            nr_pary_ruchow = (nr_ruchu+1)/2;
            wypisz_notacje();
        }
    }
}
        void Notatnik::wyczysc_tablica_notacja(){//###
//==============================================================================================================
    for(int i=0; i<9; i++)
        tablica_notacja[i].clear();
}
        void Notatnik::wypisz_ubiegla_notacje(int wiersz, int kolumna){//###
//==============================================================================================================
    if(parametr_dodatkowy == 6)
    {
        dodaj_tresc_do_tablica_notacja(zapis_calej_gry[nr_pary_ruchow-2]);
        wypisz_tablica_notacja(wiersz, kolumna, false);
        wyczysc_tablica_notacja();
        return;
    }
    int x = 416 + kolumna*176;
    int y = wiersz*11;
    for(int i=0; i<9; i++)
    {
        ustaw_kursor_na(x, y+i);
        cout<<kolumna_z_notacjami[ubiegla_kolumna_notacji][y+i];
    }
}
        void Notatnik::cofnij_kolumny(){//###
//==============================================================================================================
    skopiuj_kolumne(0, 1);
    skopiuj_kolumne(1, 2);
    oproznij_kolumne(2);
    wypisz_kolumne(0);
    wypisz_kolumne(1);
    wyczysc_kolumne(2);
    ubiegla_kolumna_notacji--;
    aktualna_kolumna_notacji--;
}
            void Notatnik::skopiuj_kolumne(int index_kopi, int index_wzorca){//###
//==============================================================================================================
    for(int i=0; i<176; i++)
        kolumna_z_notacjami[index_kopi][i] = kolumna_z_notacjami[index_wzorca][i];
}
            void Notatnik::oproznij_kolumne(int numer_kolumny){//###
//==============================================================================================================
    for(int i=0; i<176; i++)
        kolumna_z_notacjami[numer_kolumny][i].clear();
}
            void Notatnik::wypisz_kolumne(int nr){//###
//==============================================================================================================
    int x = 416 + nr*176;
    for(int i=0; i<176; i++)
    {
        if(nr == 1 && 164 < i)
            cout<<"\033[32m";
        ustaw_kursor_na(x, i);
        cout<<kolumna_z_notacjami[nr][i];
    }
    cout<<"\033[0m";
}
            void Notatnik::wyczysc_kolumne(int nr){//###
//==============================================================================================================
    int x = 416 + nr*176;
    for(int i=0; i<176; i++)
    {
        ustaw_kursor_na(x, i);
        cout<<string(176, ' ');
    }
}
        void Notatnik::dodaj_tresc_do_tablica_notacja(string tresc){
//==============================================================================================================
    for(auto znak: tresc)
    {
        dodaj_znak_do_tablica_notacja(znak);
        for(int i=0; i<9; i++)
            tablica_notacja[i] += ' ';
    }
}
            void Notatnik::dodaj_znak_do_tablica_notacja(char znak){
//==============================================================================================================
    int index_znaku = podaj_index_znaku(znak);
    int szerokosc_znaku = znaki[index_znaku][0].size();

    for(int i=0; i<9; i++)
    {
        for(int j=0; j<szerokosc_znaku; j++)
        {
            switch(znaki[index_znaku][i][j])
            {
                case ' ': tablica_notacja[i] += ' '   ; break;
                case 'X': tablica_notacja[i] += '\333'; break;
                case 'D': tablica_notacja[i] += '\334'; break;
                case 'G': tablica_notacja[i] += '\337'; break;
                default : break;
            }
        }
    }
}
                int Notatnik::podaj_index_znaku(char znak){
//==============================================================================================================
    if('A'<=znak && znak<='Z')
        return znak-65;
    if('a'<=znak && znak<='z')
        return znak-71;
    if('0'<=znak && znak<='9')
        return znak+4;
    switch(znak)
    {
        case '.': return 62;
        case '+': return 63;
        case '-': return 64;
        case '=': return 65;
        case '#': return 66;
        case ' ': return 67;
        case '/': return 68;
    }
}
        void Notatnik::wypisz_tablica_notacja(int wiersz, int kolumna, bool podswietlenie){//###//////////////////
//==============================================================================================================
    int x = 416 + kolumna*176;
    int y = wiersz*11;
    if(podswietlenie)
        cout<<"\033[32m";
    for(int i=0; i<9; i++)
    {
        ustaw_kursor_na(x, y+i);
        cout<<tablica_notacja[i];
    }
    if(podswietlenie)
        cout<<"\033[0m";
}
            void Notatnik::ustaw_kursor_na(int x, int y){
//==============================================================================================================
    cout<<"\033["<<y+1<<";"<<x+1<<"H";
}
        void Notatnik::przepisz_tablice_do_kolumny(){
//==============================================================================================================
    for(int i=0; i<9; i++)
        kolumna_z_notacjami[aktualna_kolumna_notacji][aktualny_wiersz_notacji*11 + i] = tablica_notacja[i];
}
string Notatnik::podaj_kod_aktualizacji_szachownicy(){
//==============================================================================================================
    kod_aktualizacji_szachownicy.clear();
    oblicz_kod_aktualizacji_szachownicy();
    return kod_aktualizacji_szachownicy;
}
    void Notatnik::oblicz_kod_aktualizacji_szachownicy(){
//==============================================================================================================
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
            if(aktualna_szachownica[i][j] != ubiegla_szachownica[i][j])
            {
                kod_aktualizacji_szachownicy += to_string(j) + to_string(i) + aktualna_szachownica[i][j];
                ubiegla_szachownica[i][j] = aktualna_szachownica[i][j];
            }
}
void Notatnik::zapisz_gre_w_notatniku(){
//==============================================================================================================
    auto teraz = chrono::system_clock::now();
    time_t czas_t = chrono::system_clock::to_time_t(teraz);

    ostringstream oss;
    oss << put_time(localtime(&czas_t), "zapis szachowy %Y-%m-%d %H;%M;%S");
    string nazwa_pliku = oss.str();
    ofstream plik(nazwa_pliku);
    if(plik.is_open())
    plik<<put_time(localtime(&czas_t), ">>> %Y-%m-%d %H:%M:%S <<<")<<endl;
    plik<<"==========================="<<endl;
    for(auto linijka: zapis_calej_gry)
    plik<<linijka<<endl;
    plik.close();

}







