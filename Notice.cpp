#include "Notice.h"

Notice::Notice(){//1
//==============================================================================================================
    try{
        loadChars();
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Notice::loadChars(){//0+
//==============================================================================================================
    try{
        std::ifstream reading;
        std::string charArray;
        reading.open("chars.txt");
        if (!reading.is_open())
            throw std::ifstream::failure("The 'chars.txt' file cannot be opened.");

        for(int i=0; i<globalType::numberOfChars; i++)
        {
            if (!getline(reading, charArray))
                throw std::ifstream::failure("Error reading character from 'chars.txt' file.");
            charArray.erase(0, 2);
            for(int j=0, k=0; j<globalType::letterHeight; j++, k++)
                for(; charArray[k] != '$'; k++)
                {
                    if (k >= charArray.size())
                        throw std::ifstream::failure("Unexpected line ending in 'chars.txt' file.");
                    pixelArtCharacterArray[i][j] += charArray[k];
                }
        }
        reading.close();
    }
//#########################################################################
    catch(const std::ifstream::failure &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
void Notice::setConsoleSize()noexcept{
//==============================================================================================================
    for(int i=0; i<globalType::chessboardHeight; i++)
        std::cout << std::string(globalType::chessboardwidth,'\333') << std::endl;
    std::cout << std::string(3, '\n')
              << globalType::getCommuniqueCotent({45})[0] << std::endl
              << globalType::getCommuniqueCotent({46})[0] << std::endl
              << globalType::getCommuniqueCotent({47})[0]
              << std::string(6, '\n');
    systemInfo::getChar();
    systemInfo::clearScreen();
}
void Notice::communique(std::string text, int milliseconds){//*3
//==============================================================================================================
    try{
        clearCommuniqueArray();
        addTextToCommuniqueArray(text);
        writeCommunique();
        if(milliseconds)
            systemInfo::delay(milliseconds);
        else
            systemInfo::getChar();
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Notice::clearCommuniqueArray()noexcept{
//==============================================================================================================
    for(int i=0; i<globalType::communiqueHeight; i++)
        communiqueArray[i].clear();
}
    void Notice::addTextToCommuniqueArray(std::string text){//2
//==============================================================================================================
    try{
        communiqueArray[0]  += std::string(10, '\333');
        communiqueArray[12] += std::string(10, '\333');
        for(int i=1; i<12; i++)
            communiqueArray[i] += std::string(2, '\333') + std::string(8, ' ');
        for(auto sign: text)
        {
            addCharToCommuniqueArray(sign);
            communiqueArray[0]  += '\333';
            communiqueArray[12] += '\333';
            for(int i=1; i<12; i++)
                communiqueArray[i] += ' ';
        }
        communiqueArray[0]  += std::string(9, '\333');
        communiqueArray[12] += std::string(9, '\333');
        for(int i=1; i<12; i++)
            communiqueArray[i] += std::string(7, ' ') + std::string(2, '\333');
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Notice::addCharToCommuniqueArray (char cHar){//1
//==============================================================================================================
    try{
        int charIndex = getCharIndex(cHar);
        int charWidth = pixelArtCharacterArray[charIndex][0].size();

        communiqueArray[0]  += std::string(charWidth, '\333');
        communiqueArray[1]  += std::string(charWidth,    ' ');
        communiqueArray[11] += std::string(charWidth,    ' ');
        communiqueArray[12] += std::string(charWidth, '\333');

        for(int i=2; i<11; i++)
            for(int j=0; j<charWidth; j++)
                switch(pixelArtCharacterArray[charIndex][i-2][j])
                {
                    case ' ': communiqueArray[i] += ' '   ; break;
                    case 'X': communiqueArray[i] += '\333'; break;
                    case 'D': communiqueArray[i] += '\334'; break;
                    case 'G': communiqueArray[i] += '\337'; break;
                    default : throw std::runtime_error("Error in 'chars.txt' file content.");
                }
    }
//#########################################################################
    catch(const std::runtime_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            int Notice::getCharIndex          (char cHar){//0+
//==============================================================================================================
    try{
        if('A'<=cHar && cHar<='Z')
            return cHar-65;
        if('a'<=cHar && cHar<='z')
            return cHar-71;
        if('0'<=cHar && cHar<='9')
            return cHar+4;
        switch(cHar)
        {
            case '.': return 62;
            case '+': return 63;
            case '-': return 64;
            case '=': return 65;
            case '#': return 66;
            case ' ': return 67;
            case '/': return 68;
            case '!': return 69;
            case '?': return 70;
            case '"': return 71;
            case ':': return 72;
            case ';': return 73;
            default: throw std::runtime_error("Wrong char.");
        }
    }
//#########################################################################
    catch(const std::runtime_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    void Notice::writeCommunique(){//*0
//==============================================================================================================
    try{
        int communiqueWidth = communiqueArray[0].size();
        if(370 < communiqueWidth)
            throw std::length_error("Communique too long.");
        int sideBorderWidth = (globalType::chessboardwidth-communiqueWidth)/2;
        systemInfo::setConsoleColor(globalType::menu);
        for(int i=0; i<globalType::communiqueHeight; i++)
        {
            systemInfo::setCursorPosition(sideBorderWidth, 81+i);
            std::cout<<communiqueArray[i];
        }
        systemInfo::setConsoleColor(globalType::white);
    }
//#########################################################################
    catch(const std::length_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
int Notice::checkbox(std::vector<std::string> options){//3
//==============================================================================================================
    try{
        if(options.size() == 0)
            throw std::length_error("Empty vector.");
        if(options.size() > 6)
            throw std::length_error("Vector too large.");
        addValuesOfCheckbox(options);
        clearCheckboxArray();
        loadCheckboxBorder();
        addTextToCheckboxArray(options);
        writeCheckbox();
        return selectOption();
    }
//#########################################################################
    catch(const std::length_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Notice::addValuesOfCheckbox(std::vector<std::string> options){//2
//==============================================================================================================
    try{
        longestOptionWidth = 0;
        numberOfOptions = options.size();
        for(int i=0; i<numberOfOptions; i++)
        {
            int width = countTextWidth(options[i]);
            if(longestOptionWidth < width)
                longestOptionWidth = width;
        }
        noticeWidth = 35 + ((longestOptionWidth <= 171)? 170: longestOptionWidth);
        noticeHeight  = numberOfOptions*10 + 13;
        if(370 < noticeWidth)
            throw std::out_of_range("Option content too long.");
    }
//#########################################################################
    catch(const std::out_of_range &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        int Notice::countTextWidth(std::string text){//1
//==============================================================================================================
    try{
        int width = 0;
        for(int i=0; i<text.size(); i++)
            width += getCharWidth(text[i]);
        width += text.size()-1;
        return width;
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            int Notice::getCharWidth(char cHar){//0+
//==============================================================================================================
    try{
        switch(cHar)
        {
            case 'A':
            case 'M':
            case 'O':
            case 'Q':
            case 'V':
            case 'W':
            case 'X':
            case 'Y':
            case 'm':
            case 'w':
            case '#':
                return 10;
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F':
            case 'G':
            case 'H':
            case 'J':
            case 'K':
            case 'L':
            case 'N':
            case 'P':
            case 'R':
            case 'S':
            case 'T':
            case 'U':
            case 'Z':
            case 'a':
            case 'b':
            case 'd':
            case 'f':
            case 'g':
            case 'h':
            case 'n':
            case 'o':
            case 'p':
            case 'q':
            case 'r':
            case 'u':
            case 'v':
            case 'x':
            case 'y':
            case 'z':
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '+':
            case '?':
                return 8;
            case 'c':
            case 'e':
            case 'k':
            case 't':
            case '-':
            case '=':
            case '"':
                return 7;
            case 'I':
            case 'j':
            case 's':
            case ' ':
            case '/':
                return 6;
            case 'i':
            case 'l':
            case '.':
            case '!':
            case ':':
            case ';':
                return 4;
            default:
                throw std::runtime_error("Wrong char.");
        }
    }
//#########################################################################
    catch(const std::runtime_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
    void Notice::clearCheckboxArray()noexcept{
//==============================================================================================================
    for(int i=0; i<globalType::checkboxHeight; i++)
    {
        checkboxArray[i].clear();
        checkboxArray[i] = std::string(noticeWidth, ' ');
    }
}
    void Notice::loadCheckboxBorder()noexcept{
//==============================================================================================================
    checkboxArray[0]                = std::string(noticeWidth, '\333');
    checkboxArray[1]                = std::string(noticeWidth, '\333');
    checkboxArray[noticeHeight -12] = std::string(noticeWidth, '\333');
    checkboxArray[noticeHeight - 2] = std::string(noticeWidth, '\333');
    checkboxArray[noticeHeight - 1] = std::string(noticeWidth, '\333');
    for(int i=0; i<noticeHeight; i++)
    {
        checkboxArray[i][0] = '\333';
        checkboxArray[i][1] = '\333';
        checkboxArray[i][2] = '\333';
        checkboxArray[i][3] = '\333';
        checkboxArray[i][noticeWidth - 4] = '\333';
        checkboxArray[i][noticeWidth - 3] = '\333';
        checkboxArray[i][noticeWidth - 2] = '\333';
        checkboxArray[i][noticeWidth - 1] = '\333';
    }
}
    void Notice::addTextToCheckboxArray(std::vector<std::string> options){//2
//==============================================================================================================
    try{
        std::string prefix  = globalType::getCommuniqueCotent({36})[0];
        std::string postfix = globalType::getCommuniqueCotent({37})[0];
        for(int i=0; i<numberOfOptions; i++)
            addLineOfText(options[i], i);
        int CheckboxCentering = ((205 < noticeWidth)? ((noticeWidth-205)/14): 0);
        addLineOfText(std::string(CheckboxCentering, ' ') + prefix + std::to_string(numberOfOptions) + postfix, numberOfOptions);
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Notice::addLineOfText(std::string text, int lineNumber){//1
//==============================================================================================================
    try{
        std::string line[globalType::letterHeight];
        for(auto cHar: text)
        {
            addCharToLineOfText(cHar, line);
            for(int i=0; i<globalType::letterHeight; i++)
                line[i] += ' ';
        }
        rewriteLineOfTextToCheckboxArray(line, lineNumber);
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Notice::addCharToLineOfText(char cHar, std::string line[]){//0+
//==============================================================================================================
    try{
        int charIndex = getCharIndex(cHar);
        int charWidth = pixelArtCharacterArray[charIndex][0].size();

        for(int i=0; i<globalType::letterHeight; i++)
        {
            for(int j=0; j<charWidth; j++)
            {
                switch(pixelArtCharacterArray[charIndex][i][j])
                {
                    case ' ': line[i] += ' '   ; break;
                    case 'X': line[i] += '\333'; break;
                    case 'D': line[i] += '\334'; break;
                    case 'G': line[i] += '\337'; break;
                    default : throw std::runtime_error("Error in 'chars.txt' file content.");
                }
            }
        }
    }
//#########################################################################
    catch(const std::runtime_error &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
}
            void Notice::rewriteLineOfTextToCheckboxArray(std::string line[], int lineNumber)noexcept{
//==============================================================================================================
    int lineCoordinateY =  lineNumber*10 + 2;
    int lineCoordinateX = ((lineNumber == numberOfOptions)? 10: 25);
    int lineWidth = line[0].size();
    for(int i=0; i<globalType::letterHeight; i++)
        for(int j=0; j<lineWidth; j++)
            checkboxArray[i+lineCoordinateY][j+lineCoordinateX] = line[i][j];
}
    void Notice::writeCheckbox(){//*0
//==============================================================================================================
    try{
        int sideBorderWidth = (globalType::chessboardwidth-noticeWidth)/2;
        int topBorderWidth  = (globalType::chessboardHeight-noticeHeight)  /2;
        systemInfo::setConsoleColor(globalType::menu);
        for(int i=0; i<noticeHeight; i++)
        {
            systemInfo::setCursorPosition(sideBorderWidth, topBorderWidth+i);
            std::cout<<checkboxArray[i];
        }

        systemInfo::setCursorPosition(0, 0);
        systemInfo::setConsoleColor(globalType::white);
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
    int Notice::selectOption(){//*2
//==============================================================================================================
    try{
        char cHar;
        int  chosenOption;
        bool isDigitLoaded = false;
        while(true)
        {
            cHar = systemInfo::getChar();
            if('1'<=cHar && cHar<='0'+numberOfOptions)
            {
                markOption(cHar - '1');
                chosenOption = cHar - '0';
                isDigitLoaded = true;
                continue;
            }
            if(isDigitLoaded == true && cHar == 13)
                return chosenOption;
        }
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }

}
        void Notice::markOption(int optionNumber){//*1
//==============================================================================================================
    try{
        if (optionNumber<0 || 5<optionNumber)
            throw std::invalid_argument("Number of option out of range.");
        cancelBacklight();
        backlightOption(optionNumber);
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Notice::cancelBacklight(){//*0
//==============================================================================================================
    try{
        int x = (globalType::chessboardwidth-noticeWidth)/2 + 10;
        int y = (globalType::chessboardHeight-noticeHeight)/2 + 2;
        for(int i=0; i<numberOfOptions*10-1; i++)
        {
            systemInfo::setCursorPosition(x, y+i);
            std::cout<<std::string(9, ' ');
        }
    }
//#########################################################################
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Notice::backlightOption(int optionNumber){//*0
//==============================================================================================================
    try{
        if (optionNumber<0 || 5<optionNumber)
            throw std::invalid_argument("Number of option out of range.");
        int X = (globalType::chessboardwidth-noticeWidth)/2 + 10;
        int Y = (globalType::chessboardHeight-noticeHeight)/ 2 + 2 + optionNumber*10;
        systemInfo::setConsoleColor(globalType::chsenOption);
        systemInfo::setCursorPosition(X, Y+0);  std::cout<<std::string(1, '\333') + std::string(1, '\334');
        systemInfo::setCursorPosition(X, Y+1);  std::cout<<std::string(3, '\333') + std::string(1, '\334');
        systemInfo::setCursorPosition(X, Y+2);  std::cout<<std::string(5, '\333') + std::string(1, '\334');
        systemInfo::setCursorPosition(X, Y+3);  std::cout<<std::string(7, '\333')                    ;
        systemInfo::setCursorPosition(X, Y+4);  std::cout<<std::string(5, '\333') + std::string(1, '\337');
        systemInfo::setCursorPosition(X, Y+5);  std::cout<<std::string(3, '\333') + std::string(1, '\337');
        systemInfo::setCursorPosition(X, Y+6);  std::cout<<std::string(1, '\333') + std::string(1, '\337');
        systemInfo::setCursorPosition(0, 0);
        systemInfo::setConsoleColor(globalType::white);
    }
//#########################################################################
    catch(const std::invalid_argument &e){
        globalType::errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + std::string(" >> error: ") + e.what();
        throw x;
    }
    catch(globalType::errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + std::string(" >>\n") + e.errorMessage;
        throw;
    }
}





















