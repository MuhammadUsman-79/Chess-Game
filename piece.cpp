#include "piece.h"
// REMOVED: Position constructors, ALREADY DEFINED INLINE IN piece.h
// REMOVED: #include "board.h" NOT NEEDED HERE

//======== PIECE CLASS IMPLEMENTATION ========

//Default Constructor, used when no values provided
Piece::Piece() {
    pieceColor = WHITE;
    currentPos = Position(0, 0);
    hasMoved = false;
}

//Parameterized Constructor (used when placing on board during initialization)
Piece::Piece(Color color, Position pos) {
    pieceColor = color;
    currentPos = pos;
    hasMoved = false;
}

//Copy Constructor (Shallow Copy)
Piece::Piece(const Piece &other) {
    pieceColor = other.pieceColor;
    currentPos = other.currentPos;
    hasMoved = other.hasMoved;
}