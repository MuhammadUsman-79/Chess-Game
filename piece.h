#include <iostream>
using namespace std;

//Enum for color of the Piece
enum Color {
    WHITE, //0
    BLACK //1
};

//Enum for type of the Piece
enum PieceType {
    KING, //0
    QUEEN, //1
    ROOK, //2
    BISHOP, //3
    KNIGHT, //4
    PAWN //5
};

//Represents a position on the board
//Row and column values are from 0 to 7
struct Position {
    int row;
    int col;

    //Constructor (both default and parameterized)
    Position(int r =0, int c =0) {}

    //Copy Constructor (Shallow copy)
    Position(const Position &other) {} //Copies values of row and col

};


//Forward declaration
//Used because Piece needs Board but Board includes Piece
class Board;


//=============== ABSTRACT BASE CLASS ===============
//Cannot be instantiated //Cannot directly create objects directly
//This class will be inherited by ALL chess pieces
class Piece{
protected:
    Color pieceColor;    //stores color of the piece (white or black)
    PieceType pieceType; //stores the type of the piece (king, queen, etc.)
    Position currentPos; //current location on the board
    bool hasMoved; //used for pawn first move and then castling later

public:

    //Default Constructor
    //Useful when object is created without parameters
    Piece() {}

    //Parameterized Constructor (when creating a piece with Specific values)
    Piece(Color color, Position pos) {}

    //Copy Constructor (Shallow Copy)
    //Copies values directly (safe here because no dynamic memory inside Piece)
    Piece(const Piece &other) {}


    //Virtual destructor
    virtual ~Piece() {} //Both Piece and derived class's destructor runs


    //PURE VIRTUAL FUNCTION (Polymorphism)
    //As each derived class MUST implement ITS OWN movement logic
    // moves[] is array to store the possible moves
    //moveCount is the number of valid moves found
    virtual void getValidMoves(Board &board, Position moves[], int &moveCount) = 0;


    //Returns the symbol used in printing board for console display
    virtual char getSymbol() = 0; //(Polymorphism as each piece has its own symbol/character)


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

    //Checks if moving from one square to another is inside the valid moves list
    //Returns true if the move is valid, false otherwise
    bool isValidMove(int fromRow, int fromCol, int toRow, int toCol, Board* board) {
        Position moves[28];
        int moveCount = 0;
        getValidMoves(*board, moves, moveCount);
        for (int i = 0; i < moveCount; i++) {
            if (moves[i].row == toRow && moves[i].col == toCol) {
                return true;
            }
        }
        return false;
    }
};