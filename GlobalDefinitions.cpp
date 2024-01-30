#include "GlobalDefinitions.h"
using namespace std;

namespace globalType{
    Languages setLanguage;
    Color menu;//               = red;
    Color notation;//           = green;
    Color underlightedSquare;// = blue;
    Color chsenOption;//        = yellow;
//********************************************************************************
    void readConfigFile(){
//==============================================================================================================
    try{
        ifstream reading;
        string data;
        reading.open("config.txt");
        if (!reading.is_open())
            throw ifstream::failure("The file 'config.txt' cannot be opened .");
        if (!getline(reading, data))
            throw ifstream::failure("Error reading character from 'config.txt' file .");
        if (data.size() != 5)
            throw ifstream::failure("Wrong content of 'config.txt' file.");
        reading.close();

        setLanguage        = static_cast<Languages>(data[0]-'0');
        menu               = static_cast<Color>    (data[1]-'0');
        notation           = static_cast<Color>    (data[2]-'0');
        underlightedSquare = static_cast<Color>    (data[3]-'0');
        chsenOption        = static_cast<Color>    (data[4]-'0');
//#########################################################################
    }
    catch(const ifstream::failure &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
    void writeConfigFile(){
//==============================================================================================================
    try{
        string data{};
        data += static_cast<char>(setLanguage        +'0');
        data += static_cast<char>(menu               +'0');
        data += static_cast<char>(notation           +'0');
        data += static_cast<char>(underlightedSquare +'0');
        data += static_cast<char>(chsenOption        +'0');

        ofstream file("config.txt");
        if (!file.is_open())
            throw ofstream::failure("The file could not be opened for writing.");

        file << data;
        file.close();
//#########################################################################
    }
    catch(const ofstream::failure &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
   // void readCommuniqueFile()


}
namespace systemInfo {
    void setConsoleColor(globalType::Color color){//0+
    //==============================================================================================================
    try{
        #ifdef _WIN32
            switch(color)
            {
                case globalType::white: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  break;
                case globalType::red:   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);  break;
                case globalType::green: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);  break;
                case globalType::yellow:SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);  break;
                case globalType::blue:  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);  break;
                default:    throw invalid_argument("Unknown color.");
            }
        #elif __APPLE__
                switch(color)
            {
                case globalType::white: cout << "\033[0m";   break;
                case globalType::red:   cout << "\033[31m";  break;
                case globalType::green: cout << "\033[32m";  break;
                case globalType::yellow:cout << "\033[33m";  break;
                case globalType::blue:  cout << "\033[34m";  break;
                default:    throw invalid_argument("Unknown color.");
            }
        #elif __linux__
                switch(color)
            {
                case globalType::white: cout << "\033[0m";   break;
                case globalType::red:   cout << "\033[31m";  break;
                case globalType::green: cout << "\033[32m";  break;
                case globalType::yellow:cout << "\033[33m";  break;
                case globalType::blue:  cout << "\033[34m";  break;
                default:    throw invalid_argument("Unknown color.");
            }
        #else
            #error the program only supports(Windosw/Mac_OS/Linux)
        #endif
//#########################################################################
    }
    catch(const invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
    void setCursorPosition(int x, int y){//0+
    //==============================================================================================================
    try{
        if (x < 0 || y < 0)
            throw invalid_argument("Negative coordinates.");
        #ifdef _WIN32
            COORD coord;
            coord.X = static_cast<SHORT>(x);
            coord.Y = static_cast<SHORT>(y);
            if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord))
                throw runtime_error("Error setting cursor position on Windows console");
        #elif __APPLE__
            cout<<"\033["<<y+1<<";"<<x+1<<"H";
        #elif __linux__
            cout<<"\033["<<y+1<<";"<<x+1<<"H";
        #else
            #error the program only supports(Windosw/Mac_OS/Linux)
        #endif
//#########################################################################
    }
    catch(const invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
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
                throw runtime_error("Terminal configuration error");
            ch = getchar();
            if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) != 0)
                throw runtime_error("Error restoring terminal settings");
            return ch;
        #elif __linux__
            struct termios oldt, newt;
            char ch;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0)
                throw runtime_error("Terminal configuration error");
            ch = getchar();
            if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) != 0)
                throw runtime_error("Error restoring terminal settings");
            return ch;
        #else
            #error the program only supports(Windosw/Mac_OS/Linux)
        #endif
//#########################################################################
    }
    catch(const runtime_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
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
        #error the program only supports(Windosw/Mac_OS/Linux)
    #endif
}
    void delay(int milliseconds){//0+
//==============================================================================================================
    try{
        if (milliseconds < 0)
            throw invalid_argument("Negative delay time value.");
        #ifdef _WIN32
            Sleep(milliseconds);
        #elif __APPLE__
            this_thread::sleep_for(chrono::milliseconds(milliseconds));
        #elif __linux__
            this_thread::sleep_for(chrono::milliseconds(milliseconds));
        #else
            #error the program only supports(Windosw/Mac_OS/Linux)
        #endif
//#########################################################################
    }
    catch(const invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
}
