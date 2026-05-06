#include "piece.h"

/* Queen class inherits from Piece
    This means Queen automatically gets:
      color
      position
      hasMoved flag

    Polymorphism is used because Queen overrides movement rules
    Queen combines rook and bishop movement
    It moves any number of squares horizontally, vertically, or diagonally
    It is blocked by any piece in its path
*/

class Queen : public Piece {

public:

    /*Default Constructor
    Used when queen is created without initial values
    Example: temporary object creation*/
    Queen();


    //Parameterized Constructor
    //Used when placing queen on board at start of game
    Queen(Color color, Position pos);


    /*Copy Constructor (Shallow Copy since no dynamic memory)
    Copies values from another queen object*/
    Queen(const Queen &other);


    /*Generates all moves for the queen (straight + diagonal)
    Stops when hitting board edge, friendly piece, or after capturing enemy

    board: checks current game state (occupied squares, piece colors)
    moves[]: stores possible moves
    moveCount: number of moves found
    board passed by reference to avoid copying
    */
    void getValidMoves(Board &board, Position moves[], int &moveCount) override;

    //Returns character used to display queen on the board
    char getSymbol() override;
};