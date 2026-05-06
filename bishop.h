#include "piece.h"

/* Bishop class inherits from Piece
    This means Bishop automatically gets:
      color
      position
      hasMoved flag

    Polymorphism is used because Bishop overrides movement rules
    Bishop moves diagonally any number of squares
    It is blocked by any piece in its path
*/

class Bishop : public Piece {

public:

    /*Default Constructor
    Used when bishop is created without initial values
    Example: temporary object creation*/
    Bishop();


    /*Parameterized Constructor
    Used when placing bishop on board at start of game
    */
    Bishop(Color color, Position pos);


    /*Copy Constructor (Shallow Copy since no dynamic memory)
    Copies values from another bishop object*/
    Bishop(const Bishop &other);


    /*Generates all diagonal moves for the bishop
    Stops when hitting board edge, friendly piece, or after capturing enemy

    board: checks current game state (occupied squares, piece colors)
    moves[]: stores possible moves
    moveCount: number of moves found
    board passed by reference to avoid copying
    */
    void getValidMoves(Board &board, Position moves[], int &moveCount) override;

    //Symbol Function for board
    //Returns character used to display bishop
    char getSymbol() override;
};