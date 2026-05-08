#ifndef CHESSGUI_H
#define CHESSGUI_H

#include <SFML/Graphics.hpp>
#include "board.h"
#include "GameConditions.h"
#include <string>

class ChessGUI {
private:
    sf::RenderWindow window;
    Board* board;
    GameConditions gc;
    sf::Font font;

    // GUI Dimensions
    const float TILE_SIZE = 80.f;
    const float STATUS_BAR_HEIGHT = 60.f;

    // Selection & Interaction State
    bool pieceSelected;
    int selectedRow;
    int selectedCol;
    std::string statusMessage;

    // Helper Draw Methods (Private)
    void drawBoard();
    void drawPieces();
    void drawStatusBar();
    void handleMouseClick();

public:
    // Constructor
    ChessGUI(Board* b);

    // Starts the main graphical loop
    void run();
};

#endif