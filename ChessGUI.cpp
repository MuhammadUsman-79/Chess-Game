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

void ChessGUI::handleMouseClick() {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

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

                Color activeColor = board->isWhiteTurn() ? WHITE : BLACK;
                std::string activeColorStr = (activeColor == WHITE) ? "White" : "Black";

                if (gc.is_checkmate(activeColor)) {
                    statusMessage = "CHECKMATE! " + std::string(activeColor == WHITE ? "Black" : "White") + " wins!";
                } else if (gc.is_stalemate(activeColor)) {
                    statusMessage = "DRAW BY STALEMATE!";
                } else if (gc.is_draw(activeColor)) {
                    statusMessage = "DRAW BY INSUFFICIENT MATERIAL!";
                } else if (gc.is_in_check(activeColor)) {
                    statusMessage = activeColorStr + " is in CHECK!";
                } else {
                    statusMessage = activeColorStr + "'s turn.";
                }
            } else {
                statusMessage = "Invalid move! Try again.";
            }

            pieceSelected = false;
            selectedRow = -1;
            selectedCol = -1;
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
        drawPieces();
        drawStatusBar();

        window.display();
    }
}