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
    const float TILE_SIZE = 170.f;
    const float STATUS_BAR_HEIGHT = 85.f;
    const float ROW_LABEL_GUTTER = 45.f;

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

    bool invalidMovePopupActive;
    std::string invalidMovePopupMessage;
    sf::Clock invalidMovePopupClock;
    float invalidMovePopupDuration; 

    
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
    //for pieces pics
    bool loadPieceTextures();
    sf::Texture* getTextureForPiece(Piece* p);

    void drawInvalidMovePopup();
    void triggerInvalidMovePopup(const std::string& message);
    //labeling funcs
    void drawColumnLabels();
    void drawRowLabels();

    //for game ending
    bool gameOver;
    std::string gameOverMessage;

    //buttons
    sf::FloatRect restartButtonBounds;
    sf::FloatRect exitButtonBounds;

    //valid moves highlightinig 
    Position legalMoves[28];
    int legalMoveCount;

    //pieces textures / PNGs
    sf::Texture whiteKingTexture;
    sf::Texture whiteQueenTexture;
    sf::Texture whiteRookTexture;
    sf::Texture whiteBishopTexture;
    sf::Texture whiteKnightTexture;
    sf::Texture whitePawnTexture;

    sf::Texture blackKingTexture;
    sf::Texture blackQueenTexture;
    sf::Texture blackRookTexture;
    sf::Texture blackBishopTexture;
    sf::Texture blackKnightTexture;
    sf::Texture blackPawnTexture;

public:
    // Constructor
    ChessGUI(Board* b);

    // Starts the main graphical loop
    void run();
};

#endif