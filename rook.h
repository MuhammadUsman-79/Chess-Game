#ifndef ROOK_H
#define ROOK_H
#include "piece.h"

/* Rook class inherits from Piece
This means Rook automatically gets:
color
position
hasMoved flag

Polymorphism is used because Rook overrides movement rules
Rook moves horizontally or vertically any number of squares
It is blocked by any piece in its path
hasMoved is also used for castling logic
*/

class Rook : public Piece {
    
    public:
    
    /*Default Constructor
    Used when rook is created without initial values
    Example: temporary object creation*/
    Rook();
    
    
    /*Parameterized Constructor
    Used when placing rook on board at start of game
    */
   Rook(Color color, Position pos);
   

   /*Copy Constructor (Shallow Copy since no dynamic memory)
   Copies values from another rook object*/
   Rook(const Rook &other);
   
   
   /*Generates all horizontal and vertical moves for the rook
   Stops when hitting board edge, friendly piece, or after capturing enemy
   board: checks current game state (occupied squares, piece colors)
   moves[]: stores possible moves
   moveCount: number of moves found
   board passed by reference to avoid copying
   */
  void getValidMoves(Board &board, Position moves[], int &moveCount) override;
  
  //Returns character used to display rook on the board
    char getSymbol() override;
};
#endif