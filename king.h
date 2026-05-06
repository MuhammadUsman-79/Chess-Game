#include "piece.h"

/* King class inherits from Piece
    This means King automatically gets:
      color
      position
      hasMoved flag

    Polymorphism is used because King overrides movement rules
    King moves exactly one square in any of the 8 directions
    hasMoved is used for castling logic
*/

class King : public Piece {

public:

    /*Default Constructor
    Used when king is created without initial values
    Example: temporary object creation*/
    King();


    /*Parameterized Constructor
    Used when placing king on board at start of game
    */
    King(Color color, Position pos);


    /*Copy Constructor (Shallow Copy since no dynamic memory)
    Copies values from another king object*/
    King(const King &other);


    /*Generates all one square moves for the king in 8 directions
    Does not check for check (Board class handles that)

    board: checks current game state (friendly pieces on destination)
    moves[]: stores possible moves
    moveCount: number of moves found
    board passed by reference to avoid copying
    */
    void getValidMoves(Board &board, Position moves[], int &moveCount) override;

    //Returns character used to display king on the board
    char getSymbol() override;
};