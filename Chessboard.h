#ifndef CHESSBOARD_H
#define CHESSBOARD_H
#include "GlobalDefinitions.h"

class Chessboard{
private:char chessboardArray[globalType::chessboardHeight][globalType::chessboardwidth];
        std::string piecesArray[globalType::numberOfPieces];
        bool color;
//******************************************************************************************************
public: Chessboard(bool k = true);
private:    void loadPiecesArray();
            void loadChessboardArray();
                void loadChessboard()noexcept;
                void loadCoodinates();
                    void loadSingeCoordinateChar(int coordX, int coordY, std::string cHar);
                void loadPieces();
                    void loadSinglePiece(int squareCoordX, int squareCoordY, int piece, bool pieceColor, bool underlight = 0);
public: void deleteCommunique(int szerokosc_obwieszczenia);
        void deleteCheckbox(int szerokosc_pola, int wysokosc_pola, bool deleteByChessboardWrite = true);
        void writeChessboardArray();
        void updateSquare(int squareCoordX, int squareCoordY, int piece, bool pieceColor, bool underlight);
private:    void writePiece(int squareCoordX, int squareCoordY, bool underlight);
                void refreshChessboardPartially(int fromX, int fromY, int toX, int toY);
//******************************************************************************************************
        enum Pieces{
            no     = 0,
            pawn   = 1,
            knight = 2,
            bishop = 3,
            rook   = 4,
            queen  = 5,
            king   = 6
        };
        enum Dimensions{
            topMargin    = 12,
            sideMargin   = 24,
            squareHeight = 19,
            squareWidth  = 46
        };
        enum Values{
            numberOfCoordinatesChars = 16
        };
};
#endif//CHESSBOARD_H