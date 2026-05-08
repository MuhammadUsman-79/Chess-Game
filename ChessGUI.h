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
    const float TILE_SIZE = 150.f;
    const float STATUS_BAR_HEIGHT = 85.f;

    // Selection & Interaction State
    bool pieceSelected;
    int selectedRow;
    int selectedCol;
    std::string statusMessage;

    // ADDED: TEMPORARY CHECK POPUP STATE
    bool checkPopupActive;
    std::string checkPopupMessage;
    sf::Clock checkPopupClock;
    float checkPopupDuration;

    // ADDED: GAME OVER REASON TEXT
    std::string gameOverReason;

    // Helper Draw Methods (Private)
    void drawBoard();
    void drawPieces();
    void drawStatusBar();
    void handleMouseClick();
    void drawGameOverPopup();
    void handleGameOverClick(int mouseX, int mouseY);
    void clearLegalMoveHighlights();
    void computeLegalMovesForSelectedPiece();
    void drawLegalMoveHighlights();
    void drawCheckHighlight();
    void drawCheckPopup();
    void triggerCheckPopup(const std::string& message);

    //for game ending
    bool gameOver;
    std::string gameOverMessage;

    //buttons
    sf::FloatRect restartButtonBounds;
    sf::FloatRect exitButtonBounds;

    //valid moves highlightinig 
    Position legalMoves[28];
    int legalMoveCount;

public:
    // Constructor
    ChessGUI(Board* b);

    // Starts the main graphical loop
    void run();
};

#endif