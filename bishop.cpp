#include "Bishop.h"

// Default Constructor
//Initializes bishop using base class default values
Bishop::Bishop() : Piece() {
    /*BASE CLASS already sets:
    color = WHITE
    position = (0,0)
    hasMoved = false
    */
    pieceType = BISHOP;
}


/* Parameterized Constructor
Used when placing bishop on board during initialization*/
Bishop::Bishop(Color color, Position pos) : Piece(color, pos) {
    //Sends values to Piece class to set them
    pieceType = BISHOP;
}


//Copy Constructor (Shallow Copy)
//Copies all values from another bishop object
Bishop::Bishop(const Bishop &other) : Piece(other) {
    //Base class handles copying:
    //pieceColor, currentPos, hasMoved
    pieceType = BISHOP;
}


//getValidMoves()
//Generates all diagonal moves for the bishop
//Slides along each diagonal until blocked or out of bounds
void Bishop::getValidMoves(Board &board, Position moves[], int &moveCount) {

    moveCount = 0;

    int r = currentPos.row;
    int c = currentPos.col;

    //(Top left, Top right, Bottom left, Bottom right)
    int rowDir[4] = {-1, -1, 1, 1};
    int colDir[4] = {-1, 1, -1, 1};

    //Slide along each diagonal direction
    for (int d=0; d<4; d++) {

        int newRow = r + rowDir[d];
        int newCol = c + colDir[d];

        //Keep moving until edge of board or blocked
        while (newRow >= 0 && newRow <= 7 && newCol >= 0 && newCol <= 7) {

            Piece* dest = board.getPiece(newRow, newCol);

            if (dest != nullptr){

                //Friendly piece blocks the path, CANNOT move here
                if (dest->getColor() == pieceColor) {
                    break; //Stop moving in this direction
                }

                //Enemy piece: can capture it, then must stop
                moves[moveCount++] = Position(newRow, newCol);
                break; //Cannot slide further after capture
            }

            //Empty square: add and continue moving
            moves[moveCount++] = Position(newRow, newCol);

            newRow += rowDir[d];
            newCol += colDir[d];
        }
    }
}

//Returns character on board
char Bishop::getSymbol() {

    // Uppercase = White piece
    // Lowercase = Black piece
    if (pieceColor == WHITE){
        return 'B';
    }
    else {
        return 'b';
    }
}