#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <initializer_list>
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
    #error the program only supports(Windosw/Mac_OS/Linux)
#endif





namespace globalType{
    enum{
        numberOfLanguages      = 2,
        numberOfNotationColumn = 3,
        numberOfPieces         = 7,
        letterHeight           = 9,
        notationLineHeight     = 11,
        communiqueHeight       = 13,
        numberOfCommuniques    = 38,
        numberOfChars          = 69,
        checkboxHeight         = 73,
        columnHeight           = 176,
        columnWidth            = 176,
        chessboardHeight       = 176,
        chessboardwidth        = 416
    };
    enum Color{
        white,
        red,
        green,
        yellow,
        blue
    };
    enum Languages{
        polish  = 0,
        english = 1
    };

    using chessboardPointer = char(*)[8];
    struct errorType{std::string errorMessage;};
    extern std::string communiquesArray[][numberOfLanguages];

    extern Color menu;
    extern Color notation;
    extern Color underlightedSquare;
    extern Color chsenOption;

    extern Languages setLanguage;
//********************************************************************************
    void readConfigFile();
    void writeConfigFile();
    void readCommuniqueFile();
    std::vector<std::string> getCommuniqueCotent(const std::vector<int> &indexes);
}
namespace systemInfo {
    void setConsoleColor(globalType::Color color);
    void setCursorPosition(int x, int y);
    char getChar();
    void clearScreen()noexcept;
    void delay(int milliseconds);
}
#endif//GLOBAL_DEFINITIONS_H




















