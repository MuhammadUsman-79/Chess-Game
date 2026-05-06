#include "king.h"
#include "board.h"
//Default Constructor
//Initializes king using base class default values
King::King() : Piece() {
    /*BASE CLASS already sets:
    color = WHITE
    position = (0,0)
    hasMoved = false
    */
    pieceType = KING;
}


/*Parameterized Constructor
Used when placing king on board during initialization*/
King::King(Color color, Position pos) : Piece(color, pos) {
    //Sends values to Piece class to set them
    pieceType = KING;
}


//Copy Constructor (Shallow Copy)
//Copies all values from another king object
King::King(const King &other) : Piece(other) {
    //Base class handles copying:
    //pieceColor, currentPos, hasMoved
    pieceType = KING;
}


//getValidMoves()
//Generates all 8 one square moves for the king
//King moves exactly one step in any direction
void King::getValidMoves(Board &board, Position moves[], int &moveCount) {

    moveCount = 0;

    int r = currentPos.row;
    int c = currentPos.col;

    //All 8 directions the king can move one square
    //Up, Down, Left, Right, and all 4 diagonals
    int rowDir[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int colDir[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    //Check each of the 8 surrounding squares
    for (int d=0; d<8; d++) {

        int newRow = r + rowDir[d];
        int newCol = c + colDir[d];

        //Check board boundaries (must stay within 0-7)
        if (newRow<0 || newRow>7 || newCol<0 || newCol>7){
            continue; //Skip out of bounds squares
        }

        //Check if destination has a friendly piece
        //Cannot land on a square occupied by same color
        Piece* dest = board.getPiece(newRow, newCol);
        if (dest != nullptr && dest->getColor() == pieceColor){
            continue;//Skip squares with friendly pieces
        }

        //Square is either empty or has an enemy piece to capture
        moves[moveCount++] = Position(newRow, newCol);
    }
}

//Returns character used for displaying king on board
char King::getSymbol() {

    // Uppercase = White piece
    // Lowercase = Black piece
    if (pieceColor == WHITE){
        return 'K';
    }
    else{
        return 'k';
    }
}