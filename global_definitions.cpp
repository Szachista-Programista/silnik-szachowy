#include "global_definitions.h"
using namespace std;

namespace SystemInfo {
    Kolor menu = czerwony;
    Kolor notacja = zielony;
    Kolor zaznaczone_pole = niebieski;
    Kolor wybierana_opcja = zolty;

    void setConsoleColor(Kolor x){//0+
    //==============================================================================================================
    try{
        #ifdef _WIN32
            switch(x)
            {
                case bialy:     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  break;
                case czerwony:  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);  break;
                case zielony:   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);  break;
                case zolty:     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);  break;
                case niebieski: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);  break;
                default:        throw invalid_argument("Nieznany kolor.");
            }
        #elif __APPLE__
                switch(x)
            {
                case bialy:     cout << "\033[0m";   break;
                case czerwony:  cout << "\033[31m";  break;
                case zielony:   cout << "\033[32m";  break;
                case zolty:     cout << "\033[33m";  break;
                case niebieski: cout << "\033[34m";  break;
                default:        throw invalid_argument("Nieznany kolor.");
            }
        #elif __linux__
                switch(x)
            {
                case bialy:     cout << "\033[0m";   break;
                case czerwony:  cout << "\033[31m";  break;
                case zielony:   cout << "\033[32m";  break;
                case zolty:     cout << "\033[33m";  break;
                case niebieski: cout << "\033[34m";  break;
                default:        throw invalid_argument("Nieznany kolor.");
            }
        #else
            #error program_obsluguje_jedynie_systemy(Windosw/Mac_OS/Linux)
        #endif
//#########################################################################
    }
    catch(const invalid_argument &e){
        globalType::obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
    void setCursorPosition(int x, int y){//0+
    //==============================================================================================================
    try{
        if (x < 0 || y < 0)
            throw invalid_argument("Ujemne wpolrzedne ekranu.");
        #ifdef _WIN32
                COORD coord;
                coord.X = static_cast<SHORT>(x);
                coord.Y = static_cast<SHORT>(y);
                if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord))
                    throw runtime_error("Blad ustawiania pozycji kursora na konsoli Windows");
        #elif __APPLE__
            cout<<"\033["<<y+1<<";"<<x+1<<"H";
        #elif __linux__
            cout<<"\033["<<y+1<<";"<<x+1<<"H";
        #else
            #error program_obsluguje_jedynie_systemy(Windosw/Mac_OS/Linux)
        #endif
//#########################################################################
    }
    catch(const invalid_argument &e){
        globalType::obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
    char getChar(){//0+
    //==============================================================================================================
    try{
        #ifdef _WIN32
            return static_cast<char>(getch());
        #elif __APPLE__
            struct termios oldt, newt;
            char ch;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0)
                throw runtime_error("Blad konfiguracji terminala");
            ch = getchar();
            if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) != 0)
                throw runtime_error("Blad przywracania ustawien terminala");
            return ch;
        #elif __linux__
            struct termios oldt, newt;
            char ch;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0)
                throw runtime_error("Blad konfiguracji terminala");
            ch = getchar();
            if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) != 0)
                throw runtime_error("Blad przywracania ustawien terminala");
            return ch;
        #else
            #error program_obsluguje_jedynie_systemy(Windosw/Mac_OS/Linux)
        #endif
//#########################################################################
    }
    catch(const runtime_error &e){
        globalType::obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}
    void clearScreen()noexcept{
//==============================================================================================================
    #ifdef _WIN32
        system("cls");
    #elif __APPLE__
        cout << "\033[2J\033[1;1H";
    #elif __linux__
        cout << "\033[2J\033[1;1H";
    #else
        #error program_obsluguje_jedynie_systemy(Windosw/Mac_OS/Linux)
    #endif
}
    void delay(int milliseconds){//0+
//==============================================================================================================
    try{
        if (milliseconds < 0)
            throw invalid_argument("Negatywna wartosc czasu opoznienia.");
        #ifdef _WIN32
            Sleep(milliseconds);
        #elif __APPLE__
            this_thread::sleep_for(chrono::milliseconds(milliseconds));
        #elif __linux__
            this_thread::sleep_for(chrono::milliseconds(milliseconds));
        #else
            #error program_obsluguje_jedynie_systemy(Windosw/Mac_OS/Linux)
        #endif
//#########################################################################
    }
    catch(const invalid_argument &e){
        globalType::obslugiwany_blad blad;
        blad.komunikat = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw blad;
    }
}




}
