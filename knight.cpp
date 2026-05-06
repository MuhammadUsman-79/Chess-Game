#include "Knight.h"

//Default Constructor
Knight::Knight() : Piece() {
    /*BASE CLASS already sets:
    color = WHITE
    position = (0,0)
    hasMoved = false
    */
    pieceType = KNIGHT;
}


/* Parameterized Constructor
Used when placing knight on board during initialization*/
Knight::Knight(Color color, Position pos) : Piece(color, pos) {
    //Sends values to Piece class to set them
    pieceType = KNIGHT;
}


//Copy Constructor (Shallow Copy)
//Copies all values from another knight object
Knight::Knight(const Knight &other) : Piece(other) {
    //Base class handles copying:
    //pieceColor, currentPos, hasMoved
    pieceType = KNIGHT;
}


//getValidMoves()
//Generates all 8 possible L-shaped moves for the knight
//Knight jumps over pieces, so only final square matters
void Knight::getValidMoves(Board &board, Position moves[], int &moveCount) {

    moveCount = 0;

    int r = currentPos.row;
    int c = currentPos.col;

    //All 8 possible L-shaped "jumps" a knight makes
    int rowOffsets[8] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int colOffsets[8] = {-1, 1, -2, 2, -2, 2, -1,  1};

    //Check each of the 8 possible knight spots
    for (int i = 0; i<8; i++) {

        int newRow = r + rowOffsets[i];
        int newCol = c + colOffsets[i];

        //Check board boundaries (must stay within 0-7)
        if (newRow<0 || newRow>7 || newCol<0 || newCol>7) {
            continue; //Skip out of bounds squares
        }

        //Check if destination has a friendly piece (same color)
        Piece* dest = board.getPiece(newRow, newCol);
        if (dest != nullptr && dest->getColor() == pieceColor) {
            continue; //Skip squares with friendly pieces
        }

        //Square is either empty or has an enemy piece to capture
        moves[moveCount++] = Position(newRow, newCol);
    }
}

//Returns character used for board
char Knight::getSymbol() {

    // Uppercase = White piece
    // Lowercase = Black piece
    if (pieceColor == WHITE) {
        return 'N'; //N used because King uses K
    }
    else {
        return 'n';
    }
}