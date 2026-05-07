#include "pawn.h"
#include "board.h"

// Default Constructor
//Initializes pawn using base class default values
Pawn::Pawn() : Piece() {
    /*BASE CLASS already sets:
    color = WHITE
    position = (0,0)
    hasMoved = false
    */
    pieceType = PAWN;
}

Pawn::Pawn(bool isWhite) : Piece(isWhite ? WHITE : BLACK, Position(0,0)) {
    pieceType = PAWN;
}

/* Parameterized Constructor
Used when placing pawn on board during initialization*/
Pawn::Pawn(Color color, Position pos) : Piece(color, pos) {
    //Sends values to Piece class to set them
    pieceType = PAWN;
}

//Copy Constructor (Shallow Copy)
//Copies all values from another pawn object
Pawn::Pawn(const Pawn &other) : Piece(other) {
    //Base class handles copying:
    //pieceColor, currentPos, hasMoved
    pieceType = PAWN;
}

//getValidMoves()
//Generates possible moves for pawn without validation
void Pawn::getValidMoves(Board& board, Position moves[], int& moveCount) {
    moveCount = 0;
    
    int row = getPosition().row;
    int col = getPosition().col;
    
    int direction = (getColor() == WHITE) ? -1 : 1;  // White moves up (-1), Black moves down (+1)

    // === ONE STEP FORWARD ===
    int oneStep = row + direction;
    if (oneStep >= 0 && oneStep < BOARD_SIZE) {
        if (board.getPiece(oneStep, col) == nullptr) {   // Square must be EMPTY
            moves[moveCount++] = Position(oneStep, col);
        }
    }

    // === TWO STEPS FORWARD (only from starting position) ===
    if ((getColor() == WHITE && row == 6) || (getColor() == BLACK && row == 1)) {
        int twoStep = row + (2 * direction);
        if (twoStep >= 0 && twoStep < BOARD_SIZE) {
            if (board.getPiece(oneStep, col) == nullptr && 
                board.getPiece(twoStep, col) == nullptr) {
                moves[moveCount++] = Position(twoStep, col);
            }
        }
    }

    // === DIAGONAL CAPTURES ===
    int captureCols[2] = {col - 1, col + 1};
    for (int i = 0; i < 2; i++) {
        int c = captureCols[i];
        if (c >= 0 && c < BOARD_SIZE) {
            Piece* target = board.getPiece(oneStep, c);
            if (target != nullptr && target->getColor() != getColor()) {
                moves[moveCount++] = Position(oneStep, c);
            }
        }
    }
}

//getSymbol()
//Returns character used for console representation
char Pawn::getSymbol() {

    // Uppercase = White piece
    // Lowercase = Black piece
    if (pieceColor == WHITE) {
        return 'P';
    } else {
        return 'p';
    }
}