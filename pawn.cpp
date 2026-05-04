#include "pawn.h"

// Default Constructor
//Initializes pawn using base class default values
Pawn::Pawn() : Piece() {
    /*BASE CLASS already sets:
    color = WHITE
    position = (0,0)
    hasMoved = false
    */
}


/* Parameterized Constructor
Used when placing pawn on board during initialization*/
Pawn::Pawn(Color color, Position pos) : Piece(color, pos) {
    //Sends values to Piece class to set them
}


//Copy Constructor (Shallow Copy)
//Copies all values from another pawn object
Pawn::Pawn(const Pawn &other) : Piece(other) {
    //Base class handles copying:
    //pieceColor, currentPos, hasMoved
}


//getValidMoves()
//Generates possible moves for pawn without validation
void Pawn::getValidMoves(Board &board, Position moves[], int &moveCount) {

    moveCount = 0;

    // Determines movement direction based on color
    // White moves upward (-1 row)
    //Black moves downward (+1 row)
    int direction;
    if (pieceColor == WHITE) {
        direction = -1;
    }
    else {
        direction = 1;
    }

    int r = currentPos.row; //row
    int c = currentPos.col; //col

    //Forward move (1 step)
    Position forward(r + direction, c);
    moves[moveCount++] = forward;


    //Forward move (2 steps ONLY if pawn has NOT moved yet)
    if (!hasMoved) {
        Position doubleForward(r + 2*direction, c);
        moves[moveCount++] = doubleForward;
    }

    //Diagonal capture moves (left and right)
    //Board will later check if enemy piece exists there
    Position leftCapture(r +direction, c-1); //col-1
    Position rightCapture(r+ direction, c+1); //col+1

    moves[moveCount++] = leftCapture;
    moves[moveCount++] = rightCapture;
}

//getSymbol()
//Returns character used for console representation
char Pawn::getSymbol() {

    // Uppercase = White piece
    // Lowercase = Black piece
    if (pieceColor == WHITE) {
        return 'P';
    }
    else {
        return 'p';
    }
}