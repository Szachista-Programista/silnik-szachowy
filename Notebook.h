#ifndef NOTEBOOK_H
#define NOTEBOOK_H
#include <iomanip>
#include <sstream>
#include "GlobalDefinitions.h"
/**
 * @brief The Notebook class handles the generation and printing of chess notation
 * for the current game.
 * 
 * This class is responsible for generating and printing chess notation
 * for the current game. It receives information about each move made during
 * the game and stores the notation in an array, displaying it on the screen.
 * In summary, it handles the printing of game notation on the console and offers
 * the option to save this notation to a text file at the end of the game,
 * if desired by the user.
 */
class Notebook{
    bool color;
    bool engineMove;
    bool moveMarked;
    bool gameOver = false;

    int machineKingLocationX;
    int machineKingLocationY;
    int userKingLocationX;
    int userKingLocationY;

    int moveCode;
    int moveFromX;
    int moveFromY;
    int moveToX;
    int moveToY;
    int gameOverParameter;
    int promotionParameter;

    int semiMoveNumber = 0;
    int moveNumber;

    int currentNotationLine    = 0;
    int currentNotationColumn  = 0;
    int previousNotationLine   = 0;
    int previousNotationColumn = 0;

    std::string lastMoveNotation;
    std::string gameResult;
    std::string chessboardUpdateCode;

    std::string notationArray[globalType::letterHeight];

    std::string pixelArtCharacterArray[globalType::numberOfChars]          [globalType::letterHeight];
    std::string notationColumnArray   [globalType::numberOfNotationColumn] [globalType::columnHeight];

    std::vector<std::string>entireNotation;

    globalType::chessboardPointer previousChessboard;
public: globalType::chessboardPointer currentChessboard;
//********************************************************************************
public: Notebook(bool k);
private:    globalType::chessboardPointer loadPiecesArrangement();
            void loadChars();
public:~Notebook()noexcept;
        void generateAndWriteNotation (int moveCode);
private:    void updateParameterValues(int moveCode);
                void decipherMoveCode();
                void updateKingsLocation()noexcept;
            void markMoveAndWriteNotation();
                void markCastle()noexcept;
                void markPawnPromotion()noexcept;
                void markEnPassant()noexcept;
                void markCommonMove();
                    void markKnightMove(char movedPiece);
                    void markRookMove  (char movedPiece);
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
public: std::string saveGameInNotebook();
};
#endif//NOTEBOOK_H
