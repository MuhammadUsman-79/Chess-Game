#ifndef PIECE_H  // CHANGED: BOARD_H TO PIECE_H
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
        row = other.row; // ADDED: ACTUALLY COPY THE VALUES
        col = other.col; // ADDED: ACTUALLY COPY THE VALUES
    }
};

class Board;

//=============== ABSTRACT BASE CLASS ===============
class Piece {
protected:
    Color pieceColor;
    PieceType pieceType;
    Position currentPos;
    bool hasMoved;

public:

    //Default Constructor
    Piece(); // CHANGED: REMOVED EMPTY BODY, DEFINITION IS IN piece.cpp

    //Parameterized Constructor
    Piece(Color color, Position pos); // CHANGED: REMOVED EMPTY BODY, DEFINITION IS IN piece.cpp

    //Copy Constructor
    Piece(const Piece &other); // CHANGED: REMOVED EMPTY BODY, DEFINITION IS IN piece.cpp

    //Virtual destructor
    virtual ~Piece() {}

    //PURE VIRTUAL FUNCTIONS
    virtual void getValidMoves(Board &board, Position moves[], int &moveCount) = 0;
    virtual char getSymbol() = 0;

    //GETTER FUNCTIONS
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

    //SETTER FUNCTIONS
    void setPosition(Position pos) {
        currentPos = pos;
    }

    void setHasMoved(bool moved) {
        hasMoved = moved;
    }

    // ADDED: setMoved() NEEDED BY board.cpp WHEN UNDOING MOVES
    void setMoved(bool moved) {
        hasMoved = moved;
    }

    //Checks if moving from one square to another is inside the valid moves list
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Board* board) {
    // ADDED: TEMPORARILY SET POSITION TO fromRow,fromCol BEFORE GENERATING MOVES
    Position savedPos = currentPos;
    currentPos = Position(fromRow, fromCol);
    Position moves[28];
    int moveCount = 0;
    getValidMoves(*board, moves, moveCount);
    currentPos = savedPos; // ADDED: RESTORE ORIGINAL POSITION AFTER GENERATING MOVES
    for (int i = 0; i < moveCount; i++) {
        if (moves[i].row == toRow && moves[i].col == toCol) {
            return true;
        }
    }
    return false;
    }

    // ADDED: canAttack() NEEDED BY board.cpp isSquareUnderAttack
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