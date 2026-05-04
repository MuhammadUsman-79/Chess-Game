#include "piece.h"

/* Pawn class inherits from Piece
    This means Pawn automatically gets:
      color
      position
      hasMoved flag

    Polymorphism is used because Pawn overrides movement rules
    Every piece has different movement behavior
*/

class Pawn : public Piece {

public:

    /*Default Constructor
    Used when pawn is created without initial values
    Example: temporary object creation*/
    Pawn();


    /*Parameterized Constructor
    Used when placing pawn on board at start of game
    */
    Pawn(Color color, Position pos);


    /*Copy Constructor (Shallow Copy since no dynamic memory)
    Copies values from another pawn object*/
    Pawn(const Pawn &other);


    /*Generates possible pawn moves (not fully validated, Only generates moves, Board class handles validation)
    board: checks current game state (empty squares, enemies, blocks)
    moves[]: stores possible moves
    moveCount: number of moves found
    board passed by reference to avoid copying
    */
    void getValidMoves(Board &board, Position moves[], int &moveCount) override;

    //Symbol Function for Console Representation
    //Returns character used to display pawn on the board
    char getSymbol() override;
};
