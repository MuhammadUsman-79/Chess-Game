#include "queen.h"

// Default Constructor
//Initializes queen using base class default values
Queen::Queen() : Piece() {
    /*BASE CLASS already sets:
    color = WHITE
    position = (0,0)
    hasMoved = false
    */
    pieceType = QUEEN;
}


/*Parameterized Constructor
Used when placing queen on board during initialization*/
Queen::Queen(Color color, Position pos) : Piece(color, pos) {
    //Sends values to Piece class to set them
    pieceType = QUEEN;
}


//Copy Constructor (Shallow Copy)
//Copies all values from another queen object
Queen::Queen(const Queen &other) : Piece(other) {
    //Base class handles copying:
    //pieceColor, currentPos, hasMoved
    pieceType = QUEEN;
}


//getValidMoves()
//Generates all moves for the queen
//Combines rook (straight) and bishop (diagonal) movement
//Slides along each direction until blocked or out of bounds
void Queen::getValidMoves(Board &board, Position moves[], int &moveCount) {

    moveCount = 0;

    int r = currentPos.row;
    int c = currentPos.col;

    //All 8 directions: straight (rook) + diagonal (bishop)
    //Up, Down, Left, Right, Top left, Top right, Bottom left, Bottom right
    int rowDir[8] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int colDir[8] = {0, 0, -1, 1, -1, 1, -1, 1};

    //Slide along each of the 8 directions
    for (int d=0; d<8; d++) {

        int newRow = r + rowDir[d];
        int newCol = c + colDir[d];

        //Keep moving until edge of board or blocked
        while (newRow>= 0 && newRow<= 7 && newCol>= 0 && newCol<= 7) {

            Piece* dest = board.getPiece(newRow, newCol);

            if (dest != nullptr){

                //Friendly piece blocks the path, cannot move here
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


//Returns character used for console representation
char Queen::getSymbol() {

    // Uppercase = White piece
    // Lowercase = Black piece
    if (pieceColor == WHITE) {
        return 'Q';
    }
    else {
        return 'q';
    }
}