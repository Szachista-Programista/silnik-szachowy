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
    #error the program only supports(Windosw/Mac_OS/Linux)
#endif





namespace globalType{
    using chessboardPointer = char(*)[8];
    struct errorType{std::string errorMessage;};

    enum{
        numberOfNotationColumn = 3,
        numberOfPieces         = 7,
        letterHeight           = 9,
        notationLineHeight     = 11,
        communiqueHeight       = 13,
        checkboxHeight         = 73,
        numberOfChars          = 69,
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
    extern Color menu;
    extern Color notation;
    extern Color underlightedSquare;
    extern Color chsenOption;
    enum Languages{
        polish  = 0,
        english = 1
    };
    extern Languages setLanguage;
//********************************************************************************
    void readConfigFile();
    void writeConfigFile();
}
namespace systemInfo {
    void setConsoleColor(globalType::Color color);
    void setCursorPosition(int x, int y);
    char getChar();
    void clearScreen()noexcept;
    void delay(int milliseconds);
}
#endif//GLOBAL_DEFINITIONS_H




















