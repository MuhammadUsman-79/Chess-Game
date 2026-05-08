#ifndef GAMECONDITIONS_H
#define GAMECONDITIONS_H

#include "piece.h"
#include "board.h" 

class GameConditions {
private:
    Board* board;

public:
    GameConditions(Board* b);

    bool is_in_check(Color color);
    bool is_checkmate(Color color);
    bool is_stalemate(Color color);
    bool is_draw(Color color);

    Color getOpponentColor(Color color);
    
    bool is_threefold_repetition();
};

#endif