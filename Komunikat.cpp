#include "Komunikat.h"
using namespace std;
using namespace globalType;

Komunikat::Komunikat(){//1
//==============================================================================================================
    try{
        wczytaj_znaki();
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
    void Komunikat::wczytaj_znaki(){//0+
//==============================================================================================================
    try{
        ifstream odczyt;
        string znak;
        odczyt.open("znaki.txt");
        if (!odczyt.is_open())
            throw ifstream::failure("Nie mozna otworzyc pliku 'znaki.txt'.");

        for(int i=0; i<69; i++)
        {
            if (!getline(odczyt, znak))
                throw ifstream::failure("Blad odczytywania znaku z pliku 'znaki.txt'.");
            znak.erase(0, 2);
            for(int j=0, k=0; j<9; j++, k++)
            {
                for(; znak[k] != '$'; k++)
                {
                    if (k >= znak.size())
                        throw ifstream::failure("Niespodziewany koniec linii w pliku 'znaki.txt'.");
                    znaki_ASCII[i][j] += znak[k];
                }
            }
        }
        odczyt.close();
//#########################################################################
    }
    catch(const ifstream::failure &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
void Komunikat::ustawienie_wielkosci_obrazu()noexcept{
//==============================================================================================================
    for(int i=0; i<176; i++)
        cout<<string(416,'\333')<<endl;
    cout<<"1. Prosze, wyswietl konsole na pelnym ekranie."<<endl
        <<"2. Ustaw wieklosc wyswietlanego obrazu tak, aby powyzszy bialy kwadrat byl widoczny w calosi."<<endl
        <<"3. Wcisnij dowolny klawisz, aby rozpoczac."
        <<"\n\n\n\n\n\n\n\n\n";
    SystemInfo::getChar();
    SystemInfo::clearScreen();
}
void Komunikat::obwieszczenie(string tresc, int milliseconds){//*3
//==============================================================================================================
    try{
        wyczysc_tablica_obwieszczenie();
        dodaj_tresc_do_tablica_obwieszczenie(tresc);
        wypisz_obwieszczenie();
        if(milliseconds)
            SystemInfo::delay(milliseconds);
        else
            SystemInfo::getChar();
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
    void Komunikat::wyczysc_tablica_obwieszczenie()noexcept{
//==============================================================================================================
    for(int i=0; i<13; i++)
        tablica_obwieszczenie[i].clear();
}
    void Komunikat::dodaj_tresc_do_tablica_obwieszczenie(string tresc){//2
//==============================================================================================================
    try{
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
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        void Komunikat::dodaj_znak_do_tablica_obwieszczenie(char znak){//1
//==============================================================================================================
    try{
        int index_znaku = podaj_index_znaku(znak);
        int szerokosc_znaku = znaki_ASCII[index_znaku][0].size();

        tablica_obwieszczenie[0]  += string(szerokosc_znaku, '\333');
        tablica_obwieszczenie[1]  += string(szerokosc_znaku,    ' ');
        tablica_obwieszczenie[11] += string(szerokosc_znaku,    ' ');
        tablica_obwieszczenie[12] += string(szerokosc_znaku, '\333');

        for(int i=2; i<11; i++)
        {
            for(int j=0; j<szerokosc_znaku; j++)
            {
                switch(znaki_ASCII[index_znaku][i-2][j])
                {
                    case ' ': tablica_obwieszczenie[i] += ' '   ; break;
                    case 'X': tablica_obwieszczenie[i] += '\333'; break;
                    case 'D': tablica_obwieszczenie[i] += '\334'; break;
                    case 'G': tablica_obwieszczenie[i] += '\337'; break;
                    default : throw invalid_argument("Blad w tresci pliku znaki.txt.");
                }
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
            int Komunikat::podaj_index_znaku(char znak){//0+
//==============================================================================================================
    try{

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
    void Komunikat::wypisz_obwieszczenie(){//*0
//==============================================================================================================
    try{
        int szerokosc_obwieszczenia = tablica_obwieszczenie[0].size();
        if(370 < szerokosc_obwieszczenia)
            throw invalid_argument("Za dluga tresc komunikatu.");
        int margines_boczny = (416-szerokosc_obwieszczenia)/2;
        SystemInfo::setConsoleColor(SystemInfo::menu);
        for(int i=0; i<13; i++)
        {
            SystemInfo::setCursorPosition(margines_boczny, 81+i);
            cout<<tablica_obwieszczenie[i];
        }
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
int Komunikat::pole_wyboru(vector<string> opcje){//3
//==============================================================================================================
    try{
        if(opcje.size() == 0)
            throw invalid_argument("Pusty wektor.");
        if(opcje.size() > 4)
            throw invalid_argument("Wektor o za duzym rozmiarze.");
        nadaj_wartosci_parametrom_pola(opcje);
        wyczysc_tablica_pole_wyboru();
        wczytaj_obramowanie_pola();
        wczytaj_tresc_pola_wyboru(opcje);
        wypisz_pole_wyboru();
        return wybor_opcji();
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
    void Komunikat::nadaj_wartosci_parametrom_pola(vector<string> opcje){//2
//==============================================================================================================
    try{
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
        if(370 < szerokosc_pola)
            throw invalid_argument("Za dluga tresc opcji.");
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
        int Komunikat::oblicz_szerokosc_tekstu(string tresc){//1
//==============================================================================================================
    try{
        int dlugosc = 0;
        for(int i=0; i<tresc.size(); i++)
            dlugosc += podaj_szerokosc_znaku(tresc[i]);
        dlugosc += tresc.size()-1;
        return dlugosc;
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
            int Komunikat::podaj_szerokosc_znaku(char znak){//0+
//==============================================================================================================
    try{
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
                throw invalid_argument("Niewlasciwy znak.");
        }
//#########################################################################
    }
    catch(const invalid_argument &e){
        obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
    void Komunikat::wyczysc_tablica_pole_wyboru()noexcept{
//==============================================================================================================
    for(int i=0; i<53; i++)
    {
        tablica_pole_wyboru[i].clear();
        tablica_pole_wyboru[i] = string(szerokosc_pola, ' ');
    }
}
    void Komunikat::wczytaj_obramowanie_pola()noexcept{
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
    void Komunikat::wczytaj_tresc_pola_wyboru(vector<string> opcje){//2
//==============================================================================================================
    try{
        for(int i=0; i<liczba_opcji; i++)
            wczytaj_linijke_tekstu(opcje[i], i);
        int wysrotkowanie_komunikatu = ((205 < szerokosc_pola)? ((szerokosc_pola-205)/14): 0);
        wczytaj_linijke_tekstu(string(wysrotkowanie_komunikatu, ' ') + "wybierz 1-" + to_string(liczba_opcji) + " oraz Enter", liczba_opcji);
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
        void Komunikat::wczytaj_linijke_tekstu(string tresc, int nr_linijki){//1
//==============================================================================================================
    try{
        string linijka[9];
        for(auto znak: tresc)
        {
            dodaj_znak_do_linijki(znak, linijka);
            for(int i=0; i<9; i++)
                linijka[i] += ' ';
        }
        przepisz_linijke_do_tablica_pola_wyboru(linijka, nr_linijki);
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
            void Komunikat::dodaj_znak_do_linijki(char znak, string linijka[]){//0+
//==============================================================================================================
    try{
        int index_znaku = podaj_index_znaku(znak);
        int szerokosc_znaku = znaki_ASCII[index_znaku][0].size();

        for(int i=0; i<9; i++)
        {
            for(int j=0; j<szerokosc_znaku; j++)
            {
                switch(znaki_ASCII[index_znaku][i][j])
                {
                    case ' ': linijka[i] += ' '   ; break;
                    case 'X': linijka[i] += '\333'; break;
                    case 'D': linijka[i] += '\334'; break;
                    case 'G': linijka[i] += '\337'; break;
                    default : throw invalid_argument("Blad w tresci pliku znaki.txt.");
                }
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
            void Komunikat::przepisz_linijke_do_tablica_pola_wyboru(string linijka[], int nr_linijki)noexcept{
//==============================================================================================================
    int index_linijki_Y =  nr_linijki*10 + 2;
    int index_linijki_X = (nr_linijki == liczba_opcji? 10: 25);
    int szerokosc_linijki = linijka[0].size();
    for(int i=0; i<9; i++)
        for(int j=0; j<szerokosc_linijki; j++)
            tablica_pole_wyboru[i+index_linijki_Y][j+index_linijki_X] = linijka[i][j];
}
    void Komunikat::wypisz_pole_wyboru(){//*0
//==============================================================================================================
    try{
        int margines_boczny = (416-szerokosc_pola)/2;
        int margines_gorny  = (176-wysokosc_pola) /2;
        SystemInfo::setConsoleColor(SystemInfo::menu);
        for(int i=0; i<wysokosc_pola; i++)
        {
            SystemInfo::setCursorPosition(margines_boczny, margines_gorny+i);
            cout<<tablica_pole_wyboru[i];
        }
        SystemInfo::setConsoleColor(SystemInfo::bialy);
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
    int Komunikat::wybor_opcji(){//*2
//==============================================================================================================
    try{
        char znak;
        int wybrana_opcja;
        bool wczytano_cyfre = false;
        while(true)
        {
            znak = SystemInfo::getChar();
            if('1'<=znak && znak<='0'+liczba_opcji)
            {
                zaznacz_opcje(znak - '1');
                wybrana_opcja = znak - '0';
                wczytano_cyfre = true;
                continue;
            }
            if(wczytano_cyfre == true && znak == 13)
                return wybrana_opcja;
        }
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }

}
        void Komunikat::zaznacz_opcje(int nr_opcji){//*1
//==============================================================================================================
    try{
        if (nr_opcji<0 || 3<nr_opcji)
            throw invalid_argument("Numer opcji poza zakresem.");
        anuluj_podswietlenie();
        podswietl_opcje(nr_opcji);
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
            void Komunikat::anuluj_podswietlenie(){//*0
//==============================================================================================================
    try{
        int X = (416-szerokosc_pola)/2 + 10;
        int Y  = (176-wysokosc_pola)/2 + 2;
        for(int i=0; i<liczba_opcji*10-1; i++)
        {
            SystemInfo::setCursorPosition(X, Y+i);
            cout<<string(9, ' ');
        }
//#########################################################################
    }
    catch(obslugiwany_blad &e){
        e.komunikat = __PRETTY_FUNCTION__ + string(" >>\n") + e.komunikat;
        throw;
    }
}
            void Komunikat::podswietl_opcje(int nr_opcji){//*0
//==============================================================================================================
    try{
        if (nr_opcji<0 || 3<nr_opcji)
            throw invalid_argument("Numer opcji poza zakresem.");
        int X = (416-szerokosc_pola)/2 + 10;
        int Y = (176-wysokosc_pola)/ 2 + 2 + nr_opcji*10;
        SystemInfo::setConsoleColor(SystemInfo::wybierana_opcja);
        SystemInfo::setCursorPosition(X, Y+0);    cout<<string(1, '\333') + string(1, '\334');
        SystemInfo::setCursorPosition(X, Y+1);    cout<<string(3, '\333') + string(1, '\334');
        SystemInfo::setCursorPosition(X, Y+2);    cout<<string(5, '\333') + string(1, '\334');
        SystemInfo::setCursorPosition(X, Y+3);    cout<<string(7, '\333')                    ;
        SystemInfo::setCursorPosition(X, Y+4);    cout<<string(5, '\333') + string(1, '\337');
        SystemInfo::setCursorPosition(X, Y+5);    cout<<string(3, '\333') + string(1, '\337');
        SystemInfo::setCursorPosition(X, Y+6);    cout<<string(1, '\333') + string(1, '\337');
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





















