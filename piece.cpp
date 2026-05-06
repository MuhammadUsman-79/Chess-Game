#include "piece.h"


//====== Positon Class implementation ======

//Constructor for initiailization of row and column with given values
//used when placing or calculating moves
Position::Position(int r, int c) {
    row = r;
    col = c;
}

//Copy Constructor (Shallow Copy since simple structure)
Position::Position(const Position &other) {
    row = other.row;
    col = other.col;
}



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

//VIRTUAL Destructor, so derived class destructors run properly
Piece::~Piece() {
//No dynamic memory,so nothing to free
}


//GETTER FUNCTIONS

Color Piece::getColor() {
    return pieceColor;
}

Position Piece::getPosition() {
    return currentPos;
}

bool Piece::getHasMoved() {
    return hasMoved;
}



//SETTER FUNCTIONS

void Piece::setPosition(Position pos) {
    currentPos = pos;
}

void Piece::setHasMoved(bool moved) {
    hasMoved = moved;
}