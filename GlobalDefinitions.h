#ifndef GLOBAL_DEFINITIONS_H
#define GLOBAL_DEFINITIONS_H
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <chrono>
#include <thread>
#ifdef _WIN32
    #include <Windows.h>
    #include <conio.h>
#elif __APPLE__
    #include <termios.h>
    #include <unistd.h>
#elif __linux__
    #include <termios.h>
    #include <unistd.h>
#else
    #error the program only supports(Windosw/Mac_OS/Linux)
#endif

namespace globalType{

    extern double(*userKingBehaviorPoints1)   (double,double);
    extern double(*engineKnightBehaviorPoints)(double,double);
    extern double(*engineBishopBehaviorPoints)(double,double);
    extern double(*engineRookBehaviorPoints1) (double,double);
    extern double(*engineRookBehaviorPoints2) (double,double);
    extern double(*engineQueenBehaviorPoints1)(double,double);
    extern double(*engineQueenBehaviorPoints2)(double,double);
    extern double(*engineKingBehaviorPoints)  (double,double);

    extern double *userKingBehaviorPriority1;
    extern double *engineKnightBehaviorPriority;
    extern double *engineBishopBehaviorPriority;
    extern double *engineRookBehaviorPriority1;
    extern double *engineRookBehaviorPriority2;
    extern double *engineQueenBehaviorPriority1;
    extern double *engineQueenBehaviorPriority2;
    extern double *engineKingBehaviorPriority;

    extern double userKingX;
    extern double userKingY;

    enum{
        numberOfLanguages      = 2,
        numberOfNotationColumn = 3,
        numberOfPieces         = 7,
        letterHeight           = 9,
        notationLineHeight     = 11,
        communiqueHeight       = 13,
        numberOfCommuniques    = 48,
        checkboxHeight         = 73,
        numberOfChars          = 74,
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
        polish,
        english
    };
    enum GameStage{
        opening,
        middlegame,
        endgame
    };
    enum KindOfEndgame{
        rooksAndQueensMate,
        rookOrQueenMate,
        unspecifiedMate
    };
    enum UserKingSideLocation{
        engineRightSide,
        engineDownSide,
        engineLeftSide,
        engineUpSide
    };
    enum UserKingCornerLocation{
        engineUpRightCorner,
        engineDownRightCorner,
        engineDownLeftCorner,
        engineUpLeftCorner
    };

    using chessboardPointer = char(*)[8];
    struct errorType{std::string errorMessage;};
    extern std::string communiquesArray[][numberOfLanguages];

    extern Color menu;
    extern Color notation;
    extern Color underlightedSquare;
    extern Color chsenOption;
    extern Languages setLanguage;
    extern GameStage gameStage;
    extern KindOfEndgame choosenEndgame;
    extern UserKingSideLocation userKingSideLocation;
    extern UserKingCornerLocation userKingCornerLocation;
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
}
#endif//GLOBAL_DEFINITIONS_H