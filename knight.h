#ifndef KNIGHT_H
#define KNIGHT_H
#include "piece.h"

/* Knight class inherits from Piece
This means Knight automatically gets:
color
position
hasMoved flag

Polymorphism is used because Knight overrides movement rules
Knight moves in an L-shape: 2 squares in one direction and 1 square perpendicular
It is the only piece that can "jump" over other pieces
*/

class Knight : public Piece {
    
    public:
    
    /*Default Constructor
    Used when knight is created without initial values
    Example: temporary object creation*/
    Knight();
    
    
    /*Parameterized Constructor
    Used when placing knight on board at start of game
    */
   Knight(Color color, Position pos);
   
   
   /*Copy Constructor (Shallow Copy since no dynamic memory)
   Copies values from another knight object*/
   Knight(const Knight &other);
   
   
   /*Generates all L-shaped moves for the knight
   Knight jumps over pieces, so no blocking check needed
   board: checks current game state (friendly pieces on final sqaure)
   moves[]: stores possible moves
   moveCount: number of moves found
   board passed by reference to avoid copying
   */
  void getValidMoves(Board &board, Position moves[], int &moveCount) override;
  
  //Returns character used to display knight on board
  char getSymbol() override;
};
  #endif