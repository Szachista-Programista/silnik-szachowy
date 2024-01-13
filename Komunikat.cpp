#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include <conio.h>
#include "Komunikat.h"

using namespace std;


Komunikat::Komunikat(){
//==============================================================================================================
    wczytaj_znaki();
}
    void Komunikat::wczytaj_znaki(){
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
void Komunikat::ustawienie_wielkosci_obrazu(){
//==============================================================================================================
    for(int i=0; i<176; i++)
        cout<<string(15,'\333')<<endl;
    cout<<"1. Prosze, wyswietl konsole na pelnym ekranie."<<endl
        <<"2. Ustaw wieklosc wyswietlanego obrazu tak aby powyzszy bialy pasek byl widoczny w calosi."<<endl
        <<"3. Wcisnij dowolny klawisz, aby rozpoczac."
        <<"\n\n\n\n\n\n\n\n\n";
    system("pause");
    system("cls");
}
void Komunikat::obwieszczenie(string tresc, int czas_ms){
//==============================================================================================================
    wyczysc_tablica_obwieszczenie();
    dodaj_tresc_do_tablica_obwieszczenie(tresc);
    wypisz_obwieszczenie();
    Sleep(czas_ms);
}
    void Komunikat::wyczysc_tablica_obwieszczenie(){
//==============================================================================================================
    for(int i=0; i<13; i++)
        tablica_obwieszczenie[i].clear();
}
    void Komunikat::dodaj_tresc_do_tablica_obwieszczenie(string tresc){
//==============================================================================================================
    tablica_obwieszczenie[0]  += string(10, '\333');
    tablica_obwieszczenie[12] += string(10, '\333');
    for(int i=1; i<12; i++)
        tablica_obwieszczenie[i] += string(2, '\333') + string(8, ' ');
    for(auto znak: tresc)
    {
        dodaj_znak_do_tablica_obwieszczenie(znak);
        tablica_obwieszczenie[0]  += '\333';
        tablica_obwieszczenie[12] += '\333';
        for(int i=1; i<12; i++)
            tablica_obwieszczenie[i] += ' ';
    }
    tablica_obwieszczenie[0]  += string(9, '\333');
    tablica_obwieszczenie[12] += string(9, '\333');
    for(int i=1; i<12; i++)
        tablica_obwieszczenie[i] += string(7, ' ') + string(2, '\333');
}
        void Komunikat::dodaj_znak_do_tablica_obwieszczenie(char znak){
//==============================================================================================================
    int index_znaku = podaj_index_znaku(znak);
    int szerokosc_znaku = znaki[index_znaku][0].size();

    tablica_obwieszczenie[0]  += string(szerokosc_znaku, '\333');
    tablica_obwieszczenie[1]  += string(szerokosc_znaku,    ' ');
    tablica_obwieszczenie[11] += string(szerokosc_znaku,    ' ');
    tablica_obwieszczenie[12] += string(szerokosc_znaku, '\333');

    for(int i=2; i<11; i++)
    {
        for(int j=0; j<szerokosc_znaku; j++)
        {
            switch(znaki[index_znaku][i-2][j])
            {
                case ' ': tablica_obwieszczenie[i] += ' '   ; break;
                case 'X': tablica_obwieszczenie[i] += '\333'; break;
                case 'D': tablica_obwieszczenie[i] += '\334'; break;
                case 'G': tablica_obwieszczenie[i] += '\337'; break;
                default : break;
            }
        }
    }
}
            int Komunikat::Komunikat::podaj_index_znaku(char znak){
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
    }
}
    void Komunikat::wypisz_obwieszczenie(){//////////////////
//==============================================================================================================
    int szerokosc_obwieszczenia = tablica_obwieszczenie[0].size();
    int margines_boczny = (416-szerokosc_obwieszczenia)/2;
    cout<<"\033[31m";
    for(int i=0; i<13; i++)
    {
        ustaw_kursor_na(margines_boczny, 81+i);
        cout<<tablica_obwieszczenie[i];
    }
    cout<<"\033[0m";
}
        void Komunikat::ustaw_kursor_na(int x, int y){
//==============================================================================================================
    cout<<"\033["<<y+1<<";"<<x+1<<"H";
}
int Komunikat::pole_wyboru(vector<string> opcje){
//==============================================================================================================
    nadaj_wartosci_parametrom_pola(opcje);
    wyczysc_tablica_pole_wyboru();
    wczytaj_obramowanie_pola();
    wczytaj_tresc_pola_wyboru(opcje);
    wypisz_pole_wyboru();
    return wybor_opcji();
}
    void Komunikat::nadaj_wartosci_parametrom_pola(vector<string> opcje){
//==============================================================================================================
    szerokosc_najdluzszej_opcji = 0;
    liczba_opcji = opcje.size();
    for(int i=0; i<liczba_opcji; i++)
    {
        int szerokosc = oblicz_szerokosc_tekstu(opcje[i]);
        if(szerokosc_najdluzszej_opcji < szerokosc)
            szerokosc_najdluzszej_opcji = szerokosc;
    }
    szerokosc_pola = 35 + ((szerokosc_najdluzszej_opcji <= 171)? 170: szerokosc_najdluzszej_opcji);
    wysokosc_pola  = liczba_opcji*10 + 13;
}
        int Komunikat::oblicz_szerokosc_tekstu(string tresc){
//==============================================================================================================
    int dlugosc = 0;
    for(int i=0; i<tresc.size(); i++)
        dlugosc += podaj_szerokosc_znaku(tresc[i]);
    dlugosc += tresc.size()-1;
    return dlugosc;
}
            int Komunikat::podaj_szerokosc_znaku(char znak){
//==============================================================================================================
    switch(znak)
    {
        case 'A':
        case 'M':
        case 'O':
        case 'Q':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'm':
        case 'w':
        case '#':
            return 10;
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'J':
        case 'K':
        case 'L':
        case 'N':
        case 'P':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'Z':
        case 'a':
        case 'b':
        case 'd':
        case 'f':
        case 'g':
        case 'h':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 'u':
        case 'v':
        case 'x':
        case 'y':
        case 'z':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '+':
            return 8;
        case 'c':
        case 'e':
        case 'k':
        case 't':
        case '-':
        case '=':
            return 7;
        case 'I':
        case 'j':
        case 's':
        case ' ':
        case '/':
            return 6;
        case 'i':
        case 'l':
        case '.':
            return 4;
        default:
            return 0;
    }
}
    void Komunikat::wyczysc_tablica_pole_wyboru(){
//==============================================================================================================
    for(int i=0; i<53; i++)
    {
        tablica_pole_wyboru[i].clear();
        tablica_pole_wyboru[i] = string(szerokosc_pola, ' ');
    }
}
    void Komunikat::wczytaj_obramowanie_pola(){
//==============================================================================================================
    tablica_pole_wyboru[0]                 = string(szerokosc_pola, '\333');
    tablica_pole_wyboru[1]                 = string(szerokosc_pola, '\333');
    tablica_pole_wyboru[wysokosc_pola -12] = string(szerokosc_pola, '\333');
    tablica_pole_wyboru[wysokosc_pola - 2] = string(szerokosc_pola, '\333');
    tablica_pole_wyboru[wysokosc_pola - 1] = string(szerokosc_pola, '\333');
    for(int i=0; i<wysokosc_pola; i++)
    {
        tablica_pole_wyboru[i][0] = '\333';
        tablica_pole_wyboru[i][1] = '\333';
        tablica_pole_wyboru[i][2] = '\333';
        tablica_pole_wyboru[i][3] = '\333';
        tablica_pole_wyboru[i][szerokosc_pola - 4] = '\333';
        tablica_pole_wyboru[i][szerokosc_pola - 3] = '\333';
        tablica_pole_wyboru[i][szerokosc_pola - 2] = '\333';
        tablica_pole_wyboru[i][szerokosc_pola - 1] = '\333';
    }
}
    void Komunikat::wczytaj_tresc_pola_wyboru(std::vector<std::string> opcje){
//==============================================================================================================
    for(int i=0; i<liczba_opcji; i++)
        wczytaj_linijke_tekstu(opcje[i], i);
    int wysrotkowanie_komunikatu = ((205 < szerokosc_pola)? ((szerokosc_pola-205)/14): 0);
    wczytaj_linijke_tekstu(string(wysrotkowanie_komunikatu, ' ') + "wybierz 1-" + to_string(liczba_opcji) + " oraz Enter", liczba_opcji);
}
        void Komunikat::wczytaj_linijke_tekstu(string tresc, int nr_linijki){
//==============================================================================================================
    string linijka[9];
    for(auto znak: tresc)
    {
        dodaj_znak_do_linijki(znak, linijka);
        for(int i=0; i<9; i++)
            linijka[i] += ' ';
    }
    przepisz_linijke_do_tablica_pola_wyboru(linijka, nr_linijki);
}
            void Komunikat::dodaj_znak_do_linijki(char znak, string linijka[]){
//==============================================================================================================
    int index_znaku = podaj_index_znaku(znak);
    int szerokosc_znaku = znaki[index_znaku][0].size();

    for(int i=0; i<9; i++)
    {
        for(int j=0; j<szerokosc_znaku; j++)
        {
            switch(znaki[index_znaku][i][j])
            {
                case ' ': linijka[i] += ' '   ; break;
                case 'X': linijka[i] += '\333'; break;
                case 'D': linijka[i] += '\334'; break;
                case 'G': linijka[i] += '\337'; break;
                default : break;
            }
        }
    }
}
            void Komunikat::przepisz_linijke_do_tablica_pola_wyboru(string linijka[], int nr_linijki){
//==============================================================================================================
    int index_linijki_Y =  nr_linijki*10 + 2;
    int index_linijki_X = (nr_linijki == liczba_opcji? 10: 25);
    int szerokosc_linijki = linijka[0].size();
    for(int i=0; i<9; i++)
        for(int j=0; j<szerokosc_linijki; j++)
            tablica_pole_wyboru[i+index_linijki_Y][j+index_linijki_X] = linijka[i][j];
}
    void Komunikat::wypisz_pole_wyboru(){
//==============================================================================================================
    int margines_boczny = (416-szerokosc_pola)/2;
    int margines_gorny  = (176-wysokosc_pola) /2;
    cout<<"\033[31m";
    for(int i=0; i<wysokosc_pola; i++)
    {
        ustaw_kursor_na(margines_boczny, margines_gorny+i);
        cout<<tablica_pole_wyboru[i];
    }
    cout<<"\033[0m";
}
    int Komunikat::wybor_opcji(){//////////////////////
//==============================================================================================================
    char znak;
    int wybrana_opcja;
    bool wczytano_cyfre = false;
    while(true)
    {
        znak = getch();
        if('1'<=znak && znak<='0'+liczba_opcji)
        {
            zaznacz_opcje(znak-'1');
            wybrana_opcja = znak - '0';
            wczytano_cyfre = true;
            continue;
        }
        if(wczytano_cyfre == true && znak == 13)
            return wybrana_opcja;
    }
}
        void Komunikat::zaznacz_opcje(int nr_opcji){
//==============================================================================================================
    anuluj_podswietlenie();
    podswietl_opcje(nr_opcji);
}
            void Komunikat::anuluj_podswietlenie(){
//==============================================================================================================
    int X = (416-szerokosc_pola)/2 + 10;
    int Y  = (176-wysokosc_pola)/2 + 2;
    for(int i=0; i<liczba_opcji*10-1; i++)
    {
        ustaw_kursor_na(X, Y+i);
        cout<<string(9, ' ');
    }
}
            void Komunikat::podswietl_opcje(int nr_opcji){
//==============================================================================================================
    int X = (416-szerokosc_pola)/2 + 10;
    int Y = (176-wysokosc_pola)/ 2 + 2 + nr_opcji*10;
    cout<<"\033[33m";
    ustaw_kursor_na(X, Y+0);    cout<<string(1, '\333') + string(1, '\334');
    ustaw_kursor_na(X, Y+1);    cout<<string(3, '\333') + string(1, '\334');
    ustaw_kursor_na(X, Y+2);    cout<<string(5, '\333') + string(1, '\334');
    ustaw_kursor_na(X, Y+3);    cout<<string(7, '\333') + string(1, '\334');
    ustaw_kursor_na(X, Y+4);    cout<<string(9, '\333')                    ;
    ustaw_kursor_na(X, Y+5);    cout<<string(7, '\333') + string(1, '\337');
    ustaw_kursor_na(X, Y+6);    cout<<string(5, '\333') + string(1, '\337');
    ustaw_kursor_na(X, Y+7);    cout<<string(3, '\333') + string(1, '\337');
    ustaw_kursor_na(X, Y+8);    cout<<string(1, '\333') + string(1, '\337');
    cout<<"\033[0m";
}




