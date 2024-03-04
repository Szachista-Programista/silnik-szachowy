#ifndef NOTICE_H
#define NOTICE_H
#include "GlobalDefinitions.h"
/**
* @brief The Notice class serves as a notifier for displaying messages and options
* on the chessboard or console, providing notifications and user interaction
* during the game.
*/
class Notice
{
    int noticeWidth;
    int noticeHeight;
    int longestOptionWidth;
    int numberOfOptions;

    std::string communiqueArray[globalType::communiqueHeight];
    std::string checkboxArray  [globalType::checkboxHeight];

    std::string pixelArtCharacterArray [globalType::numberOfChars] [globalType::letterHeight];
//******************************************************************************************************
public: Notice();
private:    void loadChars();
public: void setConsoleSize()noexcept;
        void communique(std::string text, int milliseconds = 0);
private:    void clearCommuniqueArray()noexcept;
            void addTextToCommuniqueArray(std::string text);
                void addCharToCommuniqueArray(char cHar);
                    int getCharIndex         (char cHar);
            void writeCommunique();
public: int checkbox(std::vector<std::string> options);
private:    void addValuesOfCheckbox(std::vector<std::string> options);
                int countTextWidth(std::string text);
                    int getCharWidth(char cHar);
            void clearCheckboxArray()noexcept;
            void loadCheckboxBorder()noexcept;
            void addTextToCheckboxArray(std::vector<std::string> options);
                void addLineOfText(std::string text, int lineNumber);
                    void addCharToLineOfText(char cHar, std::string line[]);
                    void rewriteLineOfTextToCheckboxArray(std::string line[], int lineNumber)noexcept;
            void writeCheckbox();
            int selectOption();
                void markOption(int optionNumber);
                    void cancelBacklight();
                    void backlightOption(int optionNumber);
};
#endif//NOTICE_H