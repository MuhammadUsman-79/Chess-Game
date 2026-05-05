#include "board.h"
#include "piece.h"

class GameConditions {
private:
    Board* board;

public:
    GameConditions(Board* b);

    bool is_in_check(Color color); 
    bool is_checkmate(Color color);
    bool is_stalemate(Color color);
    bool is_draw(Color color);
};