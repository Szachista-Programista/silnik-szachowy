#ifndef NOTEBOOK_H
#define NOTEBOOK_H
#include <iomanip>
#include <sstream>
#include "GlobalDefinitions.h"

class Notebook{
public: globalType::chessboardPointer currentChessboard;
private:globalType::chessboardPointer previousChessboard
;

        std::vector<std::string>entireNotation;

        std::string lastMoveNotation;
        std::string pixelArtCharacterArray[globalType::numberOfChars][globalType::letterHeight];
        std::string notationColumnArray[globalType::numberOfNotationColumn][globalType::columnHeight];
        std::string notationArray[globalType::letterHeight];
        std::string chessboardUpdateCode;
        std::string gameResult;

        int machineKingLocationX;
        int machineKingLocationY;
        int userKingLocationX;
        int userKingLocationY;

        int moveCode;
        int moveFromX;
        int moveFromY;
        int moveToX;
        int moveToY;
        int additionalParameter;

        int semiMoveNumber = 0;
        int moveNumber;

        int currentNotationLine    = 0;
        int currentNotationColumn  = 0;
        int previousNotationLine   = 0;
        int previousNotationColumn = 0;

        bool color;
        bool engineMove;
        bool moveMarked;
        bool gameOver = false;
//********************************************************************************
public: Notebook(bool k);
private:    globalType::chessboardPointer initializeChessboard();
            void loadChars();
public:~Notebook()noexcept;
        void generateAndWriteNotation(int moveCode);
private:    void updateParameterValues(int moveCode);
                void decipherMoveCode();
                void updateKingsLocation()noexcept;
            void markMoveAndWriteNotation();
                void markCastle()noexcept;
                void markPawnPromotion()noexcept;
                void markEnPassant()noexcept;
                void markCommonMove();
                std::string getCoordinates(int x, int y);
                bool isThereCheck()noexcept;
            void updateEntireNotation();
                void endgameService();
                    void replacePlusWithHashtag();
            void writeNotation();
                void clearNotationArray ()noexcept;
                void writePreviousNotation(int line, int column);
                void undoNotationColumns();
                    void copyNotationColumnArray(int copyIndex, int patternIndex);
                    void emptyNotationColumnArray(int columnNumber);
                    void writeNotationColumn(int columnNumber);
public:             void clearNotationColumn(int columnNumber);
private:        void addNotationArrayContent(std::string content);
                    void addCharToNotationArray(char cHar);
                        int getCharIndex(char cHar);
                void writeNotationArray(int line, int column, bool backlight);
                void rewriteNotationToColumn()noexcept;
public: std::string getChessboardUpdateCode()noexcept;
private:    void generateChessboardUpdateCode()noexcept;
public: void saveGameInNotebook();
};
#endif//NOTEBOOK_H
