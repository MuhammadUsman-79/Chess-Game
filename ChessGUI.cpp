#include "ChessGUI.h"
#include <iostream>

ChessGUI::ChessGUI(Board* b) : board(b), gc(b) {
    pieceSelected = false;
    selectedRow = -1;
    selectedCol = -1;
    statusMessage = "White's turn. Select a piece to move.";

    // SFML 3.x uses {width, height} instead of (width, height)
    window.create(sf::VideoMode({8 * (unsigned int)TILE_SIZE, 8 * (unsigned int)TILE_SIZE + (unsigned int)STATUS_BAR_HEIGHT}), "OOP Chess - SFML 3.x GUI");
    window.setFramerateLimit(60);

    // SFML 3.x: loadFromFile() is now openFromFile()
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cout << "Error: Arial font could not be loaded from Windows directory!" << std::endl;
    }

    gameOver = false;
    gameOverMessage = "";
    restartButtonBounds = sf::FloatRect({0.f, 0.f}, {0.f, 0.f});
    exitButtonBounds = sf::FloatRect({0.f, 0.f}, {0.f, 0.f});

    legalMoveCount = 0;

    checkPopupActive = false;
    checkPopupMessage = "";
    checkPopupDuration = 2.0f;
    gameOverReason = "";
}

void ChessGUI::drawBoard() {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            sf::RectangleShape tile({TILE_SIZE, TILE_SIZE});
            tile.setPosition({c * TILE_SIZE, r * TILE_SIZE});

            if ((r + c) % 2 == 0) {
                tile.setFillColor(sf::Color(240, 217, 181));
            } else {
                tile.setFillColor(sf::Color(181, 136, 99));
            }
            window.draw(tile);
        }
    }

    if (pieceSelected) {
        sf::RectangleShape highlight({TILE_SIZE, TILE_SIZE});
        highlight.setPosition({selectedCol * TILE_SIZE, selectedRow * TILE_SIZE});
        highlight.setFillColor(sf::Color(255, 255, 0, 100));
        window.draw(highlight);
    }
}

void ChessGUI::drawPieces() {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Piece* p = board->getPiece(r, c);
            if (p != nullptr) {
                sf::CircleShape piecePlate(TILE_SIZE / 2.0f - 8.0f);
                piecePlate.setPosition({c * TILE_SIZE + 8.0f, r * TILE_SIZE + 8.0f});

                if (p->isWhite()) {
                    piecePlate.setFillColor(sf::Color(255, 253, 245));
                    piecePlate.setOutlineColor(sf::Color(100, 100, 100));
                } else {
                    piecePlate.setFillColor(sf::Color(45, 45, 45));
                    piecePlate.setOutlineColor(sf::Color(200, 200, 200));
                }
                piecePlate.setOutlineThickness(2.0f);
                window.draw(piecePlate);

                // SFML 3.x: sf::Text requires font in constructor
                sf::Text text(font, p->getSymbol(), 34);

                if (p->isWhite()) {
                    text.setFillColor(sf::Color(0, 0, 0));
                } else {
                    text.setFillColor(sf::Color(255, 255, 255));
                }

                // SFML 3.x: FloatRect uses .position and .size instead of .left/.top/.width/.height
                sf::FloatRect textBounds = text.getLocalBounds();
                sf::Vector2f origin = {
                    textBounds.position.x + textBounds.size.x / 2.0f,
                    textBounds.position.y + textBounds.size.y / 2.0f
                };
                text.setOrigin(origin);
                text.setPosition({c * TILE_SIZE + TILE_SIZE / 2.0f, r * TILE_SIZE + TILE_SIZE / 2.0f});

                window.draw(text);
            }
        }
    }
}

void ChessGUI::drawStatusBar() {
    sf::RectangleShape bar({8 * TILE_SIZE, STATUS_BAR_HEIGHT});
    bar.setPosition({0, 8 * TILE_SIZE});
    bar.setFillColor(sf::Color(35, 35, 35));
    window.draw(bar);

    // SFML 3.x: sf::Text requires font in constructor
    sf::Text text(font, statusMessage, 18);
    text.setFillColor(sf::Color(255, 255, 255));
    text.setPosition({20.f, 8 * TILE_SIZE + 18.f});
    window.draw(text);
}

// ADDED: START A TEMPORARY CHECK POPUP
void ChessGUI::triggerCheckPopup(const std::string& message) {
    checkPopupActive = true;
    checkPopupMessage = message;
    checkPopupClock.restart();
}

// ADDED: DRAW TEMPORARY CHECK POPUP
void ChessGUI::drawCheckPopup() {
    if (!checkPopupActive) return;

    // ADDED: AUTO-HIDE POPUP AFTER A FEW SECONDS
    if (checkPopupClock.getElapsedTime().asSeconds() > checkPopupDuration) {
        checkPopupActive = false;
        return;
    }

    sf::Vector2u winSize = window.getSize();

    float popupW = 420.f;
    float popupH = 90.f;
    float x = (winSize.x - popupW) / 2.f;
    float y = 60.f;

    sf::RectangleShape popup({popupW, popupH});
    popup.setPosition({x, y});
    popup.setFillColor(sf::Color(255, 235, 235, 235));
    popup.setOutlineColor(sf::Color(180, 0, 0));
    popup.setOutlineThickness(3.f);
    window.draw(popup);

    sf::Text text(font, checkPopupMessage, 22);
    text.setFillColor(sf::Color(140, 0, 0));
    text.setPosition({x + 20.f, y + 28.f});
    window.draw(text);
}

// ADDED: CLEAR LEGAL MOVE LIST
void ChessGUI::clearLegalMoveHighlights() {
    legalMoveCount = 0;
}

// ADDED: COMPUTE ONLY LEGAL MOVES FOR THE SELECTED PIECE
void ChessGUI::computeLegalMovesForSelectedPiece() {
    legalMoveCount = 0;

    if (!pieceSelected) return;

    Piece* p = board->getPiece(selectedRow, selectedCol);
    if (p == nullptr) return;

    Position moves[28];
    int moveCount = 0;

    // ADDED: SET CORRECT POSITION BEFORE GETVALIDMOVES
    Position savedPos = p->getPosition();
    p->setPosition(Position(selectedRow, selectedCol));
    p->getValidMoves(*board, moves, moveCount);
    p->setPosition(savedPos);

    for (int i = 0; i < moveCount; i++) {
        int tr = moves[i].row;
        int tc = moves[i].col;

        Piece* temp = board->getPiece(tr, tc);

        // ADDED: DETECT SPECIAL MOVES FOR SIMULATION
        bool isCastling = false;
        bool isEnPassant = false;
        Piece* capturedEnPassant = nullptr;
        int rookFromCol = -1, rookToCol = -1;

        if (p->getType() == KING && abs(tc - selectedCol) == 2) {
            isCastling = true;
            rookFromCol = (tc > selectedCol) ? 7 : 0;
            rookToCol   = (tc > selectedCol) ? 5 : 3;
        }

        if (p->getType() == PAWN && tc != selectedCol && temp == nullptr) {
            isEnPassant = true;
            int direction = (p->getColor() == WHITE) ? -1 : 1;
            capturedEnPassant = board->getPiece(tr - direction, tc);
        }

        // ADDED: SIMULATE MOVE
        board->setPiece(tr, tc, p);
        board->removePiece(selectedRow, selectedCol);

        if (isCastling) {
            Piece* rook = board->getPiece(selectedRow, rookFromCol);
            board->setPiece(selectedRow, rookToCol, rook);
            board->removePiece(selectedRow, rookFromCol);
            if (rook) rook->setPosition(Position(selectedRow, rookToCol));
        }

        if (isEnPassant && capturedEnPassant) {
            int direction = (p->getColor() == WHITE) ? -1 : 1;
            board->removePiece(tr - direction, tc);
        }

        p->setPosition(Position(tr, tc));

        bool still_in_check = gc.is_in_check(p->getColor());

        // ADDED: UNDO SIMULATION
        board->setPiece(selectedRow, selectedCol, p);
        board->setPiece(tr, tc, temp);

        if (isCastling) {
            Piece* rook = board->getPiece(selectedRow, rookToCol);
            board->setPiece(selectedRow, rookFromCol, rook);
            board->removePiece(selectedRow, rookToCol);
            if (rook) rook->setPosition(Position(selectedRow, rookFromCol));
        }

        if (isEnPassant && capturedEnPassant) {
            int direction = (p->getColor() == WHITE) ? -1 : 1;
            board->setPiece(tr - direction, tc, capturedEnPassant);
        }

        p->setPosition(Position(selectedRow, selectedCol));

        if (!still_in_check) {
            legalMoves[legalMoveCount++] = Position(tr, tc);
        }
    }
}

// ADDED: DRAW HIGHLIGHT MARKERS ON LEGAL DESTINATION SQUARES
void ChessGUI::drawLegalMoveHighlights() {
    if (!pieceSelected || legalMoveCount <= 0) return;

    for (int i = 0; i < legalMoveCount; i++) {
        int row = legalMoves[i].row;
        int col = legalMoves[i].col;

        sf::CircleShape dot(TILE_SIZE / 6.0f);
        dot.setPosition({
            col * TILE_SIZE + TILE_SIZE / 2.0f - (TILE_SIZE / 6.0f),
            row * TILE_SIZE + TILE_SIZE / 2.0f - (TILE_SIZE / 6.0f)
        });

        dot.setFillColor(sf::Color(0, 180, 0, 150));
        window.draw(dot);
    }
}

// ADDED: HIGHLIGHT THE KING THAT IS CURRENTLY IN CHECK
void ChessGUI::drawCheckHighlight() {
    // WHITE KING CHECK HIGHLIGHT
    if (gc.is_in_check(WHITE)) {
        Piece* king = board->findKing(true);
        if (king != nullptr) {
            for (int r = 0; r < BOARD_SIZE; r++) {
                for (int c = 0; c < BOARD_SIZE; c++) {
                    if (board->getPiece(r, c) == king) {
                        sf::RectangleShape highlight({TILE_SIZE, TILE_SIZE});
                        highlight.setPosition({c * TILE_SIZE, r * TILE_SIZE});
                        highlight.setFillColor(sf::Color(255, 0, 0, 120));
                        window.draw(highlight);
                    }
                }
            }
        }
    }

    // BLACK KING CHECK HIGHLIGHT
    if (gc.is_in_check(BLACK)) {
        Piece* king = board->findKing(false);
        if (king != nullptr) {
            for (int r = 0; r < BOARD_SIZE; r++) {
                for (int c = 0; c < BOARD_SIZE; c++) {
                    if (board->getPiece(r, c) == king) {
                        sf::RectangleShape highlight({TILE_SIZE, TILE_SIZE});
                        highlight.setPosition({c * TILE_SIZE, r * TILE_SIZE});
                        highlight.setFillColor(sf::Color(255, 0, 0, 120));
                        window.draw(highlight);
                    }
                }
            }
        }
    }
}

void ChessGUI::drawGameOverPopup() {
    if (!gameOver) return;

    sf::Vector2u winSize = window.getSize();

    // ADDED: DARK TRANSPARENT OVERLAY
    sf::RectangleShape overlay({(float)winSize.x, (float)winSize.y});
    overlay.setPosition({0.f, 0.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    // ADDED: POPUP BOX
    float popupW = 500.f;
    float popupH = 260.f;
    float x = (winSize.x - popupW) / 2.f;
    float y = (winSize.y - popupH) / 2.f;

    sf::RectangleShape popup({popupW, popupH});
    popup.setPosition({x, y});
    popup.setFillColor(sf::Color(245, 245, 245));
    popup.setOutlineColor(sf::Color(50, 50, 50));
    popup.setOutlineThickness(3.f);
    window.draw(popup);

    // ADDED: TITLE
    sf::Text title(font, "GAME OVER", 32);
    title.setFillColor(sf::Color::Black);
    title.setPosition({x + 145.f, y + 18.f});
    window.draw(title);

    // ADDED: MAIN RESULT MESSAGE
    sf::Text message(font, gameOverMessage, 22);
    message.setFillColor(sf::Color::Black);
    message.setPosition({x + 40.f, y + 75.f});
    window.draw(message);

    // ADDED: HOW THE GAME ENDED
    sf::Text reason(font, gameOverReason, 18);
    reason.setFillColor(sf::Color(70, 70, 70));
    reason.setPosition({x + 40.f, y + 115.f});
    window.draw(reason);

    // RESTART BUTTON
    sf::RectangleShape restartButton({140.f, 45.f});
    restartButton.setPosition({x + 70.f, y + 170.f});
    restartButton.setFillColor(sf::Color(70, 160, 70));
    restartButton.setOutlineColor(sf::Color::Black);
    restartButton.setOutlineThickness(2.f);
    window.draw(restartButton);

    sf::Text restartText(font, "Restart", 20);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition({x + 105.f, y + 180.f});
    window.draw(restartText);

    // EXIT BUTTON
    sf::RectangleShape exitButton({140.f, 45.f});
    exitButton.setPosition({x + 290.f, y + 170.f});
    exitButton.setFillColor(sf::Color(180, 60, 60));
    exitButton.setOutlineColor(sf::Color::Black);
    exitButton.setOutlineThickness(2.f);
    window.draw(exitButton);

    sf::Text exitText(font, "Exit", 20);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition({x + 343.f, y + 180.f});
    window.draw(exitText);

    // SAVE BUTTON BOUNDS FOR CLICK DETECTION
    restartButtonBounds = restartButton.getGlobalBounds();
    exitButtonBounds = exitButton.getGlobalBounds();
}

void ChessGUI::handleMouseClick() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    if (gameOver) {
        handleGameOverClick(mousePos.x, mousePos.y);
        return;
    }

    if (mousePos.y >= 0 && mousePos.y < 8 * TILE_SIZE &&
        mousePos.x >= 0 && mousePos.x < 8 * TILE_SIZE) {

        int col = mousePos.x / TILE_SIZE;
        int row = mousePos.y / TILE_SIZE;

        if (!pieceSelected) {
            Piece* p = board->getPiece(row, col);
            if (p != nullptr && p->isWhite() == board->isWhiteTurn()) {
                pieceSelected = true;
                selectedRow = row;
                selectedCol = col;

                // ADDED: COMPUTE LEGAL MOVES WHEN A PIECE IS SELECTED
                computeLegalMovesForSelectedPiece();

                statusMessage = "Select destination square.";
            }
        } else {
            int fromRow = selectedRow;
            int fromCol = selectedCol;
            int toRow = row;
            int toCol = col;

            bool moved = board->movePiece(fromRow, fromCol, toRow, toCol);

                        if (moved) {
                board->switchTurn();

                //record positon after each move 
                board->recordPosition();

                Color activeColor = board->isWhiteTurn() ? WHITE : BLACK;
                std::string activeColorStr = (activeColor == WHITE) ? "White" : "Black";

                                if (gc.is_checkmate(activeColor)) {
                    gameOver = true;
                    gameOverMessage = "CHECKMATE! " + std::string(activeColor == WHITE ? "Black wins!" : "White wins!");
                    gameOverReason = "Game ended by checkmate.";
                    checkPopupActive = false;
                    statusMessage = gameOverMessage;
                } else if (gc.is_stalemate(activeColor)) {
                    gameOver = true;
                    gameOverMessage = "DRAW! Stalemate.";
                    gameOverReason = "Game ended by stalemate.";
                    checkPopupActive = false;
                    statusMessage = gameOverMessage;
                } else if (gc.is_threefold_repetition()) {
                    gameOver = true;
                    gameOverMessage = "DRAW! Threefold repetition.";
                    gameOverReason = "Game ended by threefold repetition.";
                    checkPopupActive = false;
                    statusMessage = gameOverMessage;
                } else if (gc.is_draw(activeColor)) {
                    gameOver = true;
                    gameOverMessage = "DRAW! Insufficient material.";
                    gameOverReason = "Game ended by insufficient material.";
                    checkPopupActive = false;
                    statusMessage = gameOverMessage;
                } else if (gc.is_in_check(activeColor)) {
                    statusMessage = activeColorStr + " is in CHECK!";
                    triggerCheckPopup(activeColorStr + " is in CHECK!");
                } else {
                    statusMessage = activeColorStr + "'s turn.";
                }

            } 
            else {
                statusMessage = "Invalid move! Try again.";
            }
            pieceSelected = false;
            selectedRow = -1;
            selectedCol = -1;
            
            clearLegalMoveHighlights();
        }
    }
}

void ChessGUI::run() {
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            else if (event->is<sf::Event::MouseButtonPressed>()) {
                handleMouseClick();
            }
        }

        window.clear(sf::Color(30, 30, 30));

        drawBoard();
        drawCheckHighlight();        // ADDED: SHOW KING IN CHECK
        drawLegalMoveHighlights();   // ADDED: SHOW LEGAL MOVES
        drawPieces();
        drawStatusBar();
        drawCheckPopup();            // ADDED: TEMPORARY CHECK POPUP
        drawGameOverPopup();         // ADDED: GAME OVER POPUP

        window.display();
    }
}

void ChessGUI::handleGameOverClick(int mouseX, int mouseY) {
    sf::Vector2f point((float)mouseX, (float)mouseY);

    if (restartButtonBounds.contains(point)) {
        // RESET GAME
        board->initializeBoard();

        pieceSelected = false;
        selectedRow = -1;
        selectedCol = -1;

        gameOver = false;
        gameOverMessage = "";
        gameOverReason = "";

        // ADDED: CLEAR CHECK POPUP ON RESTART
        checkPopupActive = false;
        checkPopupMessage = "";

        statusMessage = "White's turn. Select a piece to move.";
    }
    else if (exitButtonBounds.contains(point)) {
        window.close();
    }
}