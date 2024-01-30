#include "Notice.h"
using namespace std;
using namespace globalType;

Notice::Notice(){//1
//==============================================================================================================
    try{
        loadChars();
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Notice::loadChars(){//0+
//==============================================================================================================
    try{
        ifstream reading;
        string charArray;
        reading.open("chars.txt");
        if (!reading.is_open())
            throw ifstream::failure("The 'chars.txt' file cannot be opened.");

        for(int i=0; i<globalType::numberOfChars; i++)
        {
            if (!getline(reading, charArray))
                throw ifstream::failure("Error reading character from 'chars.txt' file.");
            charArray.erase(0, 2);
            for(int j=0, k=0; j<globalType::letterHeight; j++, k++)
            {
                for(; charArray[k] != '$'; k++)
                {
                    if (k >= charArray.size())
                        throw ifstream::failure("Unexpected line ending in 'chars.txt' file.");
                    pixelArtCharacterArray[i][j] += charArray[k];
                }
            }
        }
        reading.close();
//#########################################################################
    }
    catch(const ifstream::failure &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
void Notice::setConsoleSize()noexcept{
//==============================================================================================================
    for(int i=0; i<globalType::chessboardHeight; i++)
        cout<<string(globalType::chessboardwidth,'\333')<<endl;
    cout<<"1. Prosze, wyswietl konsole na pelnym ekranie."<<endl
        <<"2. Ustaw wieklosc wyswietlanego obrazu tak, aby powyzsze biale pole bylo widoczne w calej okazalosci."<<endl
        <<"3. Wcisnij dowolny klawisz, aby rozpoczac."
        <<"\n\n\n\n\n\n";
    systemInfo::getChar();
    systemInfo::clearScreen();
}
void Notice::communique(string text, int milliseconds){//*3
//==============================================================================================================
    try{
        clearCommuniqueArray();
        addTextToCommuniqueArray(text);
        writeCommunique();
        if(milliseconds)
            systemInfo::delay(milliseconds);
        else
            systemInfo::getChar();
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Notice::clearCommuniqueArray()noexcept{
//==============================================================================================================
    for(int i=0; i<globalType::communiqueHeight; i++)
        communiqueArray[i].clear();
}
    void Notice::addTextToCommuniqueArray(string text){//2
//==============================================================================================================
    try{
        communiqueArray[0]  += string(10, '\333');
        communiqueArray[12] += string(10, '\333');
        for(int i=1; i<12; i++)
            communiqueArray[i] += string(2, '\333') + string(8, ' ');
        for(auto sign: text)
        {
            addCharToCommuniqueArray(sign);
            communiqueArray[0]  += '\333';
            communiqueArray[12] += '\333';
            for(int i=1; i<12; i++)
                communiqueArray[i] += ' ';
        }
        communiqueArray[0]  += string(9, '\333');
        communiqueArray[12] += string(9, '\333');
        for(int i=1; i<12; i++)
            communiqueArray[i] += string(7, ' ') + string(2, '\333');
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Notice::addCharToCommuniqueArray(char cHar){//1
//==============================================================================================================
    try{
        int charIndex = getCharIndex(cHar);
        int charWidth = pixelArtCharacterArray[charIndex][0].size();

        communiqueArray[0]  += string(charWidth, '\333');
        communiqueArray[1]  += string(charWidth,    ' ');
        communiqueArray[11] += string(charWidth,    ' ');
        communiqueArray[12] += string(charWidth, '\333');

        for(int i=2; i<11; i++)
        {
            for(int j=0; j<charWidth; j++)
            {
                switch(pixelArtCharacterArray[charIndex][i-2][j])
                {
                    case ' ': communiqueArray[i] += ' '   ; break;
                    case 'X': communiqueArray[i] += '\333'; break;
                    case 'D': communiqueArray[i] += '\334'; break;
                    case 'G': communiqueArray[i] += '\337'; break;
                    default : throw invalid_argument("Error in 'chars.txt' file content.");
                }
            }
        }
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
            int Notice::getCharIndex(char cHar){//0+
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
            default: throw invalid_argument("Wrong char.");
        }
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
    void Notice::writeCommunique(){//*0
//==============================================================================================================
    try{
        int communiqueWidth = communiqueArray[0].size();
        if(370 < communiqueWidth)
            throw invalid_argument("Communique too long.");
        int sideBorderWidth = (globalType::chessboardwidth-communiqueWidth)/2;
        systemInfo::setConsoleColor(globalType::menu);
        for(int i=0; i<globalType::communiqueHeight; i++)
        {
            systemInfo::setCursorPosition(sideBorderWidth, 81+i);
            cout<<communiqueArray[i];
        }
        systemInfo::setConsoleColor(globalType::white);
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
int Notice::checkbox(vector<string> options){//3
//==============================================================================================================
    try{
        if(options.size() == 0)
            throw invalid_argument("Empty vector.");
        if(options.size() > 6)
            throw invalid_argument("Vector too large.");
        addValuesOfCheckbox(options);
        clearCheckboxArray();
        loadCheckboxBorder();
        addTextToCheckboxArray(options);
        writeCheckbox();
        return selectOption();
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
    void Notice::addValuesOfCheckbox(vector<string> options){//2
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
            throw invalid_argument("Option content too long.");
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
        int Notice::countTextWidth(string text){//1
//==============================================================================================================
    try{
        int width = 0;
        for(int i=0; i<text.size(); i++)
            width += getCharWidth(text[i]);
        width += text.size()-1;
        return width;
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
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
                return 8;
            case 'c':
            case 'e':
            case 'k':
            case 't':
            case '-':
            case '=':
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
                return 4;
            default:
                throw invalid_argument("Wrong char.");
        }
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
    void Notice::clearCheckboxArray()noexcept{
//==============================================================================================================
    for(int i=0; i<globalType::checkboxHeight; i++)
    {
        checkboxArray[i].clear();
        checkboxArray[i] = string(noticeWidth, ' ');
    }
}
    void Notice::loadCheckboxBorder()noexcept{
//==============================================================================================================
    checkboxArray[0]                = string(noticeWidth, '\333');
    checkboxArray[1]                = string(noticeWidth, '\333');
    checkboxArray[noticeHeight -12] = string(noticeWidth, '\333');
    checkboxArray[noticeHeight - 2] = string(noticeWidth, '\333');
    checkboxArray[noticeHeight - 1] = string(noticeWidth, '\333');
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
    void Notice::addTextToCheckboxArray(vector<string> options){//2
//==============================================================================================================
    try{
        for(int i=0; i<numberOfOptions; i++)
            addLineOfText(options[i], i);
        int CheckboxCentering = ((205 < noticeWidth)? ((noticeWidth-205)/14): 0);
        addLineOfText(string(CheckboxCentering, ' ') + "wybierz 1-" + to_string(numberOfOptions) + " oraz Enter", numberOfOptions);
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
        void Notice::addLineOfText(string text, int lineNumber){//1
//==============================================================================================================
    try{
        string line[globalType::letterHeight];
        for(auto cHar: text)
        {
            addCharToLineOfText(cHar, line);
            for(int i=0; i<globalType::letterHeight; i++)
                line[i] += ' ';
        }
        rewriteLineOfTextToCheckboxArray(line, lineNumber);
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Notice::addCharToLineOfText(char cHar, string line[]){//0+
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
                    default : throw invalid_argument("Error in 'chars.txt' file content.");
                }
            }
        }
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
}
            void Notice::rewriteLineOfTextToCheckboxArray(string line[], int lineNumber)noexcept{
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
            cout<<checkboxArray[i];
        }

        systemInfo::setCursorPosition(0, 0);
        systemInfo::setConsoleColor(globalType::white);
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
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
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }

}
        void Notice::markOption(int optionNumber){//*1
//==============================================================================================================
    try{
        if (optionNumber<0 || 5<optionNumber)
            throw invalid_argument("Number of option out of range.");
        cancelBacklight();
        backlightOption(optionNumber);
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
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
            cout<<string(9, ' ');
        }
//#########################################################################
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}
            void Notice::backlightOption(int optionNumber){//*0
//==============================================================================================================
    try{
        if (optionNumber<0 || 5<optionNumber)
            throw invalid_argument("Number of option out of range.");
        int X = (globalType::chessboardwidth-noticeWidth)/2 + 10;
        int Y = (globalType::chessboardHeight-noticeHeight)/ 2 + 2 + optionNumber*10;
        systemInfo::setConsoleColor(globalType::chsenOption);
        systemInfo::setCursorPosition(X, Y+0);  cout<<string(1, '\333') + string(1, '\334');
        systemInfo::setCursorPosition(X, Y+1);  cout<<string(3, '\333') + string(1, '\334');
        systemInfo::setCursorPosition(X, Y+2);  cout<<string(5, '\333') + string(1, '\334');
        systemInfo::setCursorPosition(X, Y+3);  cout<<string(7, '\333')                    ;
        systemInfo::setCursorPosition(X, Y+4);  cout<<string(5, '\333') + string(1, '\337');
        systemInfo::setCursorPosition(X, Y+5);  cout<<string(3, '\333') + string(1, '\337');
        systemInfo::setCursorPosition(X, Y+6);  cout<<string(1, '\333') + string(1, '\337');
        systemInfo::setCursorPosition(0, 0);
        systemInfo::setConsoleColor(globalType::white);
//#########################################################################
    }
    catch(const invalid_argument &e){
        errorType x;
        x.errorMessage = __PRETTY_FUNCTION__ + string(" >> error: ") + e.what();
        throw x;
    }
    catch(errorType &e){
        e.errorMessage = __PRETTY_FUNCTION__ + string(" >>\n") + e.errorMessage;
        throw;
    }
}





















