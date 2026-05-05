#ifndef BOARD_H
#define BOARD_H

#include "piece.h"

// Board size constant
#define BOARD_SIZE 8

class Board {
private:
    Piece* squares[BOARD_SIZE][BOARD_SIZE];
    bool whiteTurn;
    int enPassantCol;  // column where en passant is possible, -1 if none
    int enPassantRow;

    // castling rights
    bool whiteKingSideCastle;
    bool whiteQueenSideCastle;
    bool blackKingSideCastle;
    bool blackQueenSideCastle;

public:
    Board();
    ~Board();

    void initializeBoard();
    void displayBoard();
    void displayBoardFlipped(); // from black's perspective

    Piece* getPiece(int row, int col);
    void setPiece(int row, int col, Piece* piece);
    void removePiece(int row, int col);

    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);
    bool isWhiteTurn();
    void switchTurn();

    // for game condition checks (Usman will use these)
    bool isSquareUnderAttack(int row, int col, bool byWhite);
    Piece* findKing(bool white);

    // en passant getters
    int getEnPassantRow();
    int getEnPassantCol();
    void setEnPassant(int row, int col);
    void clearEnPassant();

    // castling getters and setters
    bool getWhiteKingSideCastle();
    bool getWhiteQueenSideCastle();
    bool getBlackKingSideCastle();
    bool getBlackQueenSideCastle();
    void updateCastlingRights(int fromRow, int fromCol);

    // game loop
    void startGame();
    void printWelcome();

private:
    void clearBoard();
    bool parseInput(char input[], int &row, int &col);
    void handlePromotion(int row, int col);
    void printColumnLabels();
};

#endif