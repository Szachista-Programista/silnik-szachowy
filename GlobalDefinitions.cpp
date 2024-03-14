#include "GlobalDefinitions.h"

namespace globalType{
    double(*userKingBehaviorPoints1)   (double,double);
    double(*engineKnightBehaviorPoints)(double,double);
    double(*engineBishopBehaviorPoints)(double,double);
    double(*engineRookBehaviorPoints1) (double,double);
    double(*engineRookBehaviorPoints2) (double,double);
    double(*engineQueenBehaviorPoints1)(double,double);
    double(*engineQueenBehaviorPoints2)(double,double);
    double(*engineKingBehaviorPoints)  (double,double);

    double *userKingBehaviorPriority1;
    double *engineKnightBehaviorPriority;
    double *engineBishopBehaviorPriority;
    double *engineRookBehaviorPriority1;
    double *engineRookBehaviorPriority2;
    double *engineQueenBehaviorPriority1;
    double *engineQueenBehaviorPriority2;
    double *engineKingBehaviorPriority;

    double userKingX;
    double userKingY;

    std::string communiquesArray[numberOfCommuniques][numberOfLanguages];

    Color menu;
    Color notation;
    Color underlightedSquare;
    Color chsenOption;
    Languages setLanguage;
    GameStage gameStage;
    KindOfEndgame choosenEndgame;
    UserKingSideLocation userKingSideLocation;
    UserKingCornerLocation userKingCornerLocation;
//********************************************************************************
    void readConfigFile()
    {
        std::ifstream reading;
        std::string data;
        try
        {
            reading.open("config.txt");
            if (!reading.is_open())
                throw std::ifstream::failure("The file 'config.txt' cannot be opened .");
            if (!getline(reading, data))
                throw std::ifstream::failure("Error reading character from 'config.txt' file .");
            //if (data.size() != 5)
                //throw std::ifstream::failure("Wrong content of 'config.txt' file.");
        }
        catch(const std::ifstream::failure &e)
        {
            errorType x;
            x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
            throw x;
        }
        reading.close();

        setLanguage        = static_cast<Languages>(data[0]-'0');
        menu               = static_cast<Color>    (data[1]-'0');
        notation           = static_cast<Color>    (data[2]-'0');
        underlightedSquare = static_cast<Color>    (data[3]-'0');
        chsenOption        = static_cast<Color>    (data[4]-'0');
    }
    void writeConfigFile()
    {
        std::string data{};
        data += static_cast<char>(setLanguage        +'0');
        data += static_cast<char>(menu               +'0');
        data += static_cast<char>(notation           +'0');
        data += static_cast<char>(underlightedSquare +'0');
        data += static_cast<char>(chsenOption        +'0');
        std::ofstream file("config.txt");
        try
        {
            if (!file.is_open())
                throw std::ofstream::failure("The file could not be opened for writing.");
        }
        catch(const std::ofstream::failure &e)
        {
            errorType x;
            x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
            throw x;
        }
        file << data;
        file.close();
    }
    void readCommuniqueFile()
    {
        try
        {
            std::ifstream reading;
            std::string line;
            reading.open("communique.txt");
            if (!reading.is_open())
                throw std::ifstream::failure("The file 'communique.txt' cannot be opened.");

            for(int i=0; i<numberOfCommuniques; i++)
            {
                if (!getline(reading, line))
                    throw std::ifstream::failure("Error reading content from 'communique.txt' file.");
                for(int j=0, k=0; j<numberOfLanguages; j++, k++)
                    for(; line[k] != '$'; k++)
                    {
                        //if (k >= line.size())
                            //throw std::ifstream::failure("Unexpected line ending in 'communique.txt' file.");
                        communiquesArray[i][j] += line[k];
                    }
            }
            reading.close();
        }
        catch(const std::ifstream::failure &e)
        {
            errorType x;
            x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
            throw x;
        }
    }
    std::vector<std::string> getCommuniqueCotent(const std::vector<int> &indexes)
    {
        std::vector<std::string> result;
        try
        {
            for (int index : indexes) {
                if (index <= 0 && numberOfCommuniques <= index)
                    throw std::invalid_argument("Wrong index.");
                result.push_back(communiquesArray[index][setLanguage]);
            }
        }
        catch(const std::invalid_argument &e)
        {
            errorType x;
            x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
            throw x;
        }
        return result;
    }
}
namespace systemInfo
{
    void setConsoleColor(globalType::Color color)
    {
        #ifdef _WIN32
            try
            {
                switch(color)
                {
                    case globalType::white: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);  break;
                    case globalType::red:   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);  break;
                    case globalType::green: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);  break;
                    case globalType::yellow:SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);  break;
                    case globalType::blue:  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);  break;
                    default: throw std::runtime_error("Unknown color.");
                }
            }
            catch(const std::runtime_error &e)
            {
                globalType::errorType x;
                x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
                throw x;
            }
        #elif __APPLE__
            try
            {
                switch(color)
                {
                    case globalType::white: std::cout << "\033[0m";   break;
                    case globalType::red:   std::cout << "\033[31m";  break;
                    case globalType::green: std::cout << "\033[32m";  break;
                    case globalType::yellow:std::cout << "\033[33m";  break;
                    case globalType::blue:  std::cout << "\033[34m";  break;
                    default: throw std::runtime_error("Unknown color.");
                }
            }
            catch(const std::runtime_error &e)
            {
                globalType::errorType x;
                x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
                throw x;
            }
        #elif __linux__
            try
            {
                switch(color)
                {
                    case globalType::white: std::cout << "\033[0m";   break;
                    case globalType::red:   std::cout << "\033[31m";  break;
                    case globalType::green: std::cout << "\033[32m";  break;
                    case globalType::yellow:std::cout << "\033[33m";  break;
                    case globalType::blue:  std::cout << "\033[34m";  break;
                    default: throw std::runtime_error("Unknown color.");
                }
            }
            catch(const std::runtime_error &e)
            {
                globalType::errorType x;
                x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
                throw x;
            }
            #else
                #error the program only supports(Windosw/Mac_OS/Linux)
            #endif
    }
    void setCursorPosition(int x, int y)
    {
        try
        {
            if (x < 0 || y < 0)
                throw std::invalid_argument("Negative coordinates.");
        }
        catch(const std::invalid_argument &e)
        {
            globalType::errorType x;
            x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
            throw x;
        }
        #ifdef _WIN32
            COORD coord;
            coord.X = static_cast<SHORT>(x);
            coord.Y = static_cast<SHORT>(y);
            try
            {
                if (!SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord))
                    throw std::runtime_error("Error setting cursor position on Windows console");
            }
            catch(const std::runtime_error &e)
            {
                globalType::errorType x;
                x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
                throw x;
            }
        #elif __APPLE__
            std::cout<<"\033["<<y+1<<";"<<x+1<<"H";
        #elif __linux__
            std::cout<<"\033["<<y+1<<";"<<x+1<<"H";
        #else
            #error the program only supports(Windosw/Mac_OS/Linux)
        #endif
    }
    char getChar()
    {
        #ifdef _WIN32
            return static_cast<char>(getch());
        #elif __APPLE__
            struct termios oldt, newt;
            char ch;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            try
            {
                if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0)
                    throw std::runtime_error("Terminal configuration error");
                ch = getchar();
                if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) != 0)
                    throw std::runtime_error("Error restoring terminal settings");
            }
            catch(const std::runtime_error &e)
            {
                globalType::errorType x;
                x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
                throw x;
            }
            return ch;
        #elif __linux__
            struct termios oldt, newt;
            char ch;
            tcgetattr(STDIN_FILENO, &oldt);
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO);
            try
            {
                if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0)
                    throw std::runtime_error("Terminal configuration error");
                ch = getchar();
                if (tcsetattr(STDIN_FILENO, TCSANOW, &oldt) != 0)
                    throw std::runtime_error("Error restoring terminal settings");
            }
            catch(const std::runtime_error &e)
            {
                globalType::errorType x;
                x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
                throw x;
            }
            return ch;
        #else
            #error the program only supports(Windosw/Mac_OS/Linux)
        #endif
    }
    void clearScreen()noexcept
    {
        #ifdef _WIN32
            system("cls");
        #elif __APPLE__
            std::cout << "\033[2J\033[1;1H";
        #elif __linux__
            std::cout << "\033[2J\033[1;1H";
        #else
            #error the program only supports(Windosw/Mac_OS/Linux)
        #endif
    }
}
