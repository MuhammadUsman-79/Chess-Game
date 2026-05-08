#ifndef PIECE_H  
#define PIECE_H
#include <iostream>
using namespace std;

enum Color {
    WHITE, //0
    BLACK //1
};

enum PieceType {
    KING, //0
    QUEEN, //1
    ROOK, //2
    BISHOP, //3
    KNIGHT, //4
    PAWN //5
};

struct Position {
    int row;
    int col;
    
    //Constructor (both default and parameterized)
    Position(int r = 0, int c = 0) {
        row = r; // ADDED: ACTUALLY SET THE VALUES
        col = c; // ADDED: ACTUALLY SET THE VALUES
    }
    
    //Copy Constructor (Shallow copy)
    Position(const Position &other) {
        row = other.row;
        col = other.col;
    }
};

class Board;

// ABSTRACT BASE CLASS
class Piece {
protected:
    Color pieceColor;
    PieceType pieceType;
    Position currentPos;
    bool hasMoved;

public:

    //Default Constructor
    Piece();

    //Parameterized Constructor
    Piece(Color color, Position pos);

    //Copy Constructor
    Piece(const Piece &other);

    //Virtual destructor
    virtual ~Piece() {}

    virtual void getValidMoves(Board &board, Position moves[], int &moveCount) = 0;
    virtual char getSymbol() = 0;

    Color getColor() {
        return pieceColor;
    }

    PieceType getType() {
        return pieceType;
    }

    bool isWhite() {
        return pieceColor == WHITE;
    }

    Position getPosition() {
        return currentPos;
    }

    bool getHasMoved() {
        return hasMoved;
    }

    void setPosition(Position pos) {
        currentPos = pos;
    }

    void setHasMoved(bool moved) {
        hasMoved = moved;
    }

    void setMoved(bool moved) {
        hasMoved = moved;
    }

    //Checks if moving from one square to another is inside the valid moves list
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Board* board) {
    Position savedPos = currentPos;
    currentPos = Position(fromRow, fromCol);
    Position moves[28];
    int moveCount = 0;
    getValidMoves(*board, moves, moveCount);
    currentPos = savedPos; 
    for (int i = 0; i < moveCount; i++) {
        if (moves[i].row == toRow && moves[i].col == toCol) {
            return true;
        }
    }
    return false;
    }

    bool canAttack(int fromRow, int fromCol, int toRow, int toCol, Board* board) {
        Position savedPos = currentPos;
        currentPos = Position(fromRow, fromCol);
        Position moves[28];
        int moveCount = 0;
        getValidMoves(*board, moves, moveCount);
        currentPos = savedPos;
        for (int i = 0; i < moveCount; i++) {
            if (moves[i].row == toRow && moves[i].col == toCol) {
                return true;
            }
        }
        return false;
    }
};

#endif