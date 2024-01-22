#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#ifdef _WIN32
    #include <Windows.h>
    #include <conio.h>
#elif __APPLE__
    #include <termios.h>
    #include <unistd.h>
    #include <thread>
#elif __linux__
    #include <termios.h>
    #include <unistd.h>
    #include <thread>
#else
    #error program_obsluguje_jedynie_systemy(Windosw/Mac_OS/Linux)
#endif

namespace globalType{
    using T_wsk_szachownica = char(*)[8];
    struct obslugiwany_blad{std::string komunikat;};

    enum{
        liczba_kolumn_notacji     = 3,
        liczba_figor              = 7,
        wysokosc_litery           = 9,
        wysokosc_obwieszczenia   = 13,
        max_wysokosc_pola_wyboru = 53,
        liczba_znakow            = 69,
        wysokosc_kolumny        = 176,
        wysokosc_szachownicy    = 176,
        szerokosc_szachownicy   = 416
    };
}
namespace SystemInfo {
    enum Kolor{
        bialy,
        czerwony,
        zielony,
        zolty,
        niebieski
    };

    extern Kolor menu;
    extern Kolor notacja;
    extern Kolor zaznaczone_pole;
    extern Kolor wybierana_opcja;

    void setConsoleColor(Kolor x);
    void setCursorPosition(int x, int y);
    char getChar();
    void clearScreen()noexcept;
    void delay(int milliseconds);
}
#endif//GLOBAL_DEFINITIONS_H




















