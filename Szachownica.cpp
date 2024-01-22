#include "Szachownica.h"
using namespace std;
using namespace globalType;

Szachownica::Szachownica(bool k): kolor{k}{//3
//==============================================================================================================
    try{
        wczytaj_tablica_figor();
        wczytaj_tablica_szachownica();
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
    void Szachownica::wczytaj_tablica_figor(){//0+
//==============================================================================================================
    try{
        ifstream odczyt;
        odczyt.open("figury.txt");
        if (!odczyt.is_open())
            throw ifstream::failure("Nie mozna otworzyc pliku 'figury.txt'.");
        for(int i=0; i<7; i++)
            if (!getline(odczyt, tablica_figor[i]))
                throw ifstream::failure("Blad odczytywania znaku z pliku 'figury.txt'.");
        odczyt.close();
//#########################################################################
    }
    catch(const ifstream::failure &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
    void Szachownica::wczytaj_tablica_szachownica(){//2
//==============================================================================================================
    try{
        wczytaj_deske_szachowa();
        wczytaj_wspolrzedne();
        wczytaj_ulozenie_poczatkowe();
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        void Szachownica::wczytaj_deske_szachowa()noexcept{
//==============================================================================================================
    string pole_biale       (szerokosc_pola, '\333');
    string pole_czarne      (szerokosc_pola,    ' ');
    string margines_boczny  (20,    ' ');
    string ramka_boczna     (4,  '\333');
    string margines_podstawa(globalType::szerokosc_szachownicy,   ' ');

    string podstawa_ramki = margines_boczny + string(376,'\333') + margines_boczny;
    string pola_bcbcbcbc  = margines_boczny + ramka_boczna + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + ramka_boczna + margines_boczny;
    string pola_cbcbcbcb  = margines_boczny + ramka_boczna + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + pole_czarne + pole_biale + ramka_boczna + margines_boczny;

    for(int i=0; i<=166; i+=166)
        for(int j=0; j<10; j++)
            margines_podstawa.copy(tablica_szachownica[j+i],globalType::szerokosc_szachownicy);
    for(int i=0; i<=154; i+=154)
        for(int j=10; j<10+2; j++)
            podstawa_ramki.copy(tablica_szachownica[j+i],globalType::szerokosc_szachownicy);
    for(int i=0; i<152; i+=38)
    {
        for(int j=12; j<31; j++)
            pola_bcbcbcbc.copy(tablica_szachownica[j+i],globalType::szerokosc_szachownicy);
        for(int j=31; j<50; j++)
            pola_cbcbcbcb.copy(tablica_szachownica[j+i],globalType::szerokosc_szachownicy);
    }
}
        void Szachownica::wczytaj_wspolrzedne(){//1
//==============================================================================================================
    try{
        ifstream odczyt;
        string wspolrzedne[liczba_znakow_wspolzednych];
        odczyt.open("wspolzedne.txt");
        if (!odczyt.is_open())
            throw ifstream::failure("Nie mozna otworzyc pliku 'wspolzedne.txt'.");
        for(int i=0; i<16; i++)
            if (!getline(odczyt, wspolrzedne[i]))
                throw ifstream::failure("Blad odczytywania znaku z pliku 'wspolzedne.txt'.");
        odczyt.close();
        for(int x=43, i=kolor?8:15 ; x<=365; x+=szerokosc_pola, kolor?i++:i--)
        {
            wczytaj_znak_wspolrzedny(x, 3,   wspolrzedne[i]);
            wczytaj_znak_wspolrzedny(x, 168, wspolrzedne[i]);
        }
        for(int y=19, i=kolor?7:0 ; y<=152; y+=wysokosc_pola, kolor?i--:i++)
        {
            wczytaj_znak_wspolrzedny(6,   y, wspolrzedne[i]);
            wczytaj_znak_wspolrzedny(402, y, wspolrzedne[i]);
        }
//#########################################################################
    }
    catch(const ifstream::failure &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
            void Szachownica::wczytaj_znak_wspolrzedny(int wsp_x, int wsp_y, string znak){//0+
//==============================================================================================================
    try{
        if(wsp_x<0 || globalType::szerokosc_szachownicy<wsp_x || wsp_y<0 || globalType::wysokosc_szachownicy<wsp_y)
            throw invalid_argument("Nieprawidlowe wspolzedne.");
        if(znak.size() != 40)
            throw invalid_argument("Blad w dlugosci wiersza pliku wspolzedne.txt.");
        for(int i=0, y=wsp_y; i<40; y++ )
            for(int x=wsp_x; x<wsp_x+8; i++, x++)
            {
                switch(znak[i])
                {
                    case 'X': tablica_szachownica[y][x] = '\333'; break;
                    case 'D': tablica_szachownica[y][x] = '\334'; break;
                    case 'G': tablica_szachownica[y][x] = '\337'; break;
                    case ' ':                                     break;
                    default : throw invalid_argument("Blad w tresci pliku wspolzedne.txt.");
                }
            }
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
        void Szachownica::wczytaj_ulozenie_poczatkowe(){//1
//==============================================================================================================
    try{
        char ulozenie_poczatkowe[8][8]{{'w','s','g',kolor?'k':'h',kolor?'h':'k','g','s','w'},
                                      {'p','p','p','p','p','p','p','p'},
                                      {' ',' ',' ',' ',' ',' ',' ',' '},
                                      {' ',' ',' ',' ',' ',' ',' ',' '},
                                      {' ',' ',' ',' ',' ',' ',' ',' '},
                                      {' ',' ',' ',' ',' ',' ',' ',' '},
                                      {'P','P','P','P','P','P','P','P'},
                                      {'W','S','G',kolor?'K':'H',kolor?'H':'K','G','S','W'}};

        for(int i=0; i<8; i++)
            for(int j=0; j<8; j++)
            {
                switch (ulozenie_poczatkowe[i][j])
                {
                    case 'p': wczytaj_figure(7-j,7-i , pion,     kolor); break;
                    case 'P': wczytaj_figure(7-j,7-i , pion,    !kolor); break;
                    case 's': wczytaj_figure(7-j,7-i , skoczek,  kolor); break;
                    case 'S': wczytaj_figure(7-j,7-i , skoczek, !kolor); break;
                    case 'g': wczytaj_figure(7-j,7-i , goniec,   kolor); break;
                    case 'G': wczytaj_figure(7-j,7-i , goniec,  !kolor); break;
                    case 'w': wczytaj_figure(7-j,7-i , wierza,   kolor); break;
                    case 'W': wczytaj_figure(7-j,7-i , wierza,  !kolor); break;
                    case 'h': wczytaj_figure(7-j,7-i , hetman,   kolor); break;
                    case 'H': wczytaj_figure(7-j,7-i , hetman,  !kolor); break;
                    case 'k': wczytaj_figure(7-j,7-i , krol,     kolor); break;
                    case 'K': wczytaj_figure(7-j,7-i , krol,    !kolor); break;
                    case ' ': wczytaj_figure(7-j,7-i , brak,     kolor); break;
                    default: throw invalid_argument("Blad w ulozeniu poczatkowym.");
                }
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
            void Szachownica::wczytaj_figure(int wsp_pola_poziom, int wsp_pola_pion, int figura, bool kolor_figury, bool czy_podswietlic){//0+
//==============================================================================================================
    try{
        if(wsp_pola_poziom<0 || 7<wsp_pola_poziom || wsp_pola_pion<0 || 7<wsp_pola_pion)
            throw invalid_argument("Nieprawidlowe wspolzedne.");
        if(figura<0 || 6<figura || wsp_pola_pion<0 || 7<wsp_pola_pion)
            throw invalid_argument("Bledna figura.");
        int wsp_x = (wsp_pola_poziom)*szerokosc_pola+boczny_margines;
        int wsp_y = (wsp_pola_pion)*wysokosc_pola+gorny_margines;
        bool kolor_pola;
        if(!czy_podswietlic)
            kolor_pola = !((wsp_pola_poziom+wsp_pola_pion)%2);
        else
            kolor_pola = true;
        for(int i=0, y=wsp_y; i<874; y++ )
            for(int x=wsp_x; x<wsp_x+szerokosc_pola; i++, x++)
            {
                switch(tablica_figor[figura][i])
                {
                    case 'T': tablica_szachownica[y][x] = kolor_pola  ?'\333'                   :' '   ;                       break;
                    case 'G': tablica_szachownica[y][x] = kolor_pola  ?' '                      :'\333';                       break;
                    case 'S': tablica_szachownica[y][x] = kolor_figury?'\333'                   :' '   ;                       break;
                    case '1': tablica_szachownica[y][x] = kolor_pola  ?(kolor_figury?'\334':' '):(kolor_figury?'\333':'\337'); break;
                    case '2': tablica_szachownica[y][x] = kolor_pola  ?(kolor_figury?'\337':' '):(kolor_figury?'\333':'\334'); break;
                    case '3': tablica_szachownica[y][x] = kolor_pola  ?'\334'                   :'\337' ;                      break;
                    case '4': tablica_szachownica[y][x] = kolor_pola  ?'\337'                   :'\334' ;                      break;
                    default :  throw invalid_argument("Blad w tresci pliku figury.txt.");
                }
            }
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
void Szachownica::usun_obwieszczenie(int szerokosc_obwieszczenia){//*1
//==============================================================================================================
    try{
        if(szerokosc_obwieszczenia<0 || globalType::szerokosc_szachownicy<szerokosc_obwieszczenia)
            throw invalid_argument("Bledna szerokosc obwieszczenia.");
        int szerokosc_marginesu = (globalType::szerokosc_szachownicy-szerokosc_obwieszczenia)/2;
        odswiez_tablica_szachownica_miejscowo(szerokosc_marginesu, 81, szerokosc_marginesu+szerokosc_obwieszczenia-1, 93);
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
void Szachownica::usun_pole_wyboru(int szerokosc_pola, int wysokosc_pola, bool czy_zakryc_szachownica){//*0
//==============================================================================================================
    try{
        int margines_bok =  (globalType::szerokosc_szachownicy-szerokosc_pola)/2;
        int margines_gora = (globalType::wysokosc_szachownicy-wysokosc_pola) /2;

        for(int i=0; i<wysokosc_pola; i++)
        {
            SystemInfo::setCursorPosition(margines_bok, margines_gora + i);
            for(int j=0; j<szerokosc_pola; j++)
            {
                if(czy_zakryc_szachownica)
                    cout<<tablica_szachownica[margines_gora + i][margines_bok + j];
                else
                    cout<<' ';
            }
        }
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
void Szachownica::wypisz_tablica_szachownica(){//*0
//==============================================================================================================
    try{
        SystemInfo::setCursorPosition(0, 0);
        for(int i=0; i<globalType::wysokosc_szachownicy; i++)
        {
            for(int j=0; j<globalType::szerokosc_szachownicy; j++)
                cout<<tablica_szachownica[i][j];
            cout<<endl;
        }
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
void Szachownica::zaktualizuj_pole(int wsp_pola_poziom, int wsp_pola_pion, int figura, bool kolor_figury, bool czy_podswietlic){//*2
//==============================================================================================================
    try{
        if(wsp_pola_poziom<0 || 7<wsp_pola_poziom || wsp_pola_pion<0 || 7<wsp_pola_pion)
            throw invalid_argument("Nieprawidlowe wspolzedne.");
        if(figura<0 || 6<figura || wsp_pola_pion<0 || 7<wsp_pola_pion)
            throw invalid_argument("Bledna figura.");
        wczytaj_figure(wsp_pola_poziom, wsp_pola_pion, figura, kolor_figury, czy_podswietlic);
        wypisz_figure(wsp_pola_poziom, wsp_pola_pion, czy_podswietlic);
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
    void Szachownica::wypisz_figure(int wsp_pola_poziom, int wsp_pola_pion, bool czy_podswietlic){//*1
//==============================================================================================================
    try{
        if(wsp_pola_poziom<0 || 7<wsp_pola_poziom || wsp_pola_pion<0 || 7<wsp_pola_pion)
            throw invalid_argument("Nieprawidlowe wspolzedne.");
        if(czy_podswietlic)
            SystemInfo::setConsoleColor(SystemInfo::zaznaczone_pole);
        int wsp_x = (wsp_pola_poziom)*46+24;
        int wsp_y = (wsp_pola_pion)*19+12;
        odswiez_tablica_szachownica_miejscowo(wsp_x, wsp_y, wsp_x+45, wsp_y+18);
        SystemInfo::setConsoleColor(SystemInfo::bialy);
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
        void Szachownica::odswiez_tablica_szachownica_miejscowo(int od_X, int od_Y, int do_X, int do_Y){//*0
//==============================================================================================================
    try{
        if(od_X<0 || globalType::szerokosc_szachownicy<od_X || od_Y<0 || globalType::wysokosc_szachownicy<od_Y || do_X<0 || globalType::szerokosc_szachownicy<do_X || do_Y<0 || globalType::wysokosc_szachownicy<do_Y || od_X>do_X || od_Y>do_Y)
            throw invalid_argument("Nieprawidlowe wspolzedne.");
        for(int i=od_Y; i<=do_Y; i++)
        {
            SystemInfo::setCursorPosition(od_X, i);
            for(int j=od_X; j<=do_X; j++)
                cout<<tablica_szachownica[i][j];
        }
        SystemInfo::setCursorPosition(0, 180);
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























