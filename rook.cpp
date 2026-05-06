#include "rook.h"

// Default Constructor
//Initializes rook using base class default values
Rook::Rook() : Piece() {
    /*BASE CLASS already sets:
    color = WHITE
    position = (0,0)
    hasMoved = false
    */
    pieceType = ROOK;
}


/* Parameterized Constructor
Used when placing rook on board during initialization*/
Rook::Rook(Color color, Position pos) : Piece(color, pos) {
    //Sends values to Piece class to set them
    pieceType = ROOK;
}


//Copy Constructor (Shallow Copy)
//Copies all values from another rook object
Rook::Rook(const Rook &other) : Piece(other) {
    //Base class handles copying:
    //pieceColor, currentPos, hasMoved
    pieceType = ROOK;
}


//getValidMoves()
//Generates all horizontal and vertical moves for the rook
//Slides along each axis until blocked or out of bounds
void Rook::getValidMoves(Board &board, Position moves[], int &moveCount) {

    moveCount = 0;

    int r = currentPos.row;
    int c = currentPos.col;

    //Up, Down, Left, Right
    int rowDir[4] = {-1, 1, 0, 0 };
    int colDir[4] = { 0, 0, -1, 1 };

    //Move along each straight direction
    for (int d=0; d<4; d++) {

        int newRow = r + rowDir[d];
        int newCol = c + colDir[d];

        //Keep moving until edge of board or blocked
        while (newRow>= 0 && newRow<=7 && newCol>= 0 && newCol<= 7) {

            Piece* dest = board.getPiece(newRow, newCol);

            if (dest != nullptr) {

                //Friendly piece blocks the path, cannot move here
                if (dest->getColor() == pieceColor) {
                    break; //Stop sliding in this direction
                }

                //Enemy piece: can capture it, then must stop
                moves[moveCount++] = Position(newRow, newCol);
                break; //Cannot move further after capture
            }

            //Empty square: add and continue move
            moves[moveCount++] = Position(newRow, newCol);

            newRow += rowDir[d];
            newCol += colDir[d];
        }
    }
}

//Returns character used for board display
char Rook::getSymbol() {

    // Uppercase = White piece
    // Lowercase = Black piece
    if (pieceColor == WHITE){
        return 'R';
    }
    else{
        return 'r';
    }
}