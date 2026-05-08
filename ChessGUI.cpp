#include "ChessGUI.h"
#include <iostream>

ChessGUI::ChessGUI(Board* b) : board(b), gc(b) {
    pieceSelected = false;
    selectedRow = -1;
    selectedCol = -1;
    statusMessage = "White's turn. Select a piece to move.";

    // SFML 3.x uses {width, height} instead of (width, height)
    window.create(
    sf::VideoMode({
        static_cast<unsigned int>(8 * TILE_SIZE + ROW_LABEL_GUTTER),
        static_cast<unsigned int>(8 * TILE_SIZE + STATUS_BAR_HEIGHT)
    }),
    "OOP Chess - SFML 3.x GUI"
);
    window.setFramerateLimit(60);

    // SFML 3.x: loadFromFile() is now openFromFile()
    if (!font.openFromFile("C:\\Windows\\Fonts\\arial.ttf")) {
        std::cout << "Error: Arial font could not be loaded from Windows directory!" << std::endl;
    }
    //loading chess pieces textures
     if (!loadPieceTextures()) {
        std::cout << "Error: one or more chess piece textures failed to load." << std::endl;
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

    invalidMovePopupActive = false;
    invalidMovePopupMessage = "";
    invalidMovePopupDuration = 2.0f;
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
    const float textureSize = 512.f;
    const float padding = 12.f;
    const float scale = (TILE_SIZE - 2.f * padding) / textureSize;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Piece* p = board->getPiece(r, c);
            if (p == nullptr) continue;

            sf::Texture* texture = getTextureForPiece(p);
            if (texture == nullptr) continue;

            sf::Sprite sprite(*texture);
            sprite.setScale({scale, scale});
            sprite.setPosition({c * TILE_SIZE + padding, r * TILE_SIZE + padding});

            window.draw(sprite);
        }
    }
}

void ChessGUI::drawStatusBar() {
    // ADDED: STATUS BAR NOW COVERS THE FULL WINDOW WIDTH
    sf::RectangleShape bar({
        static_cast<float>(window.getSize().x),
        STATUS_BAR_HEIGHT
    });
    bar.setPosition({0, 8 * TILE_SIZE});
    bar.setFillColor(sf::Color(35, 35, 35));
    window.draw(bar);

    // ADDED: DRAW COLUMN LABELS
    drawColumnLabels();

    // MOVE STATUS TEXT LOWER SO IT DOES NOT OVERLAP LABELS
    sf::Text text(font, statusMessage, 18);
    text.setFillColor(sf::Color::White);
    text.setPosition({20.f, 8 * TILE_SIZE + 42.f});
    window.draw(text);
}

// ADDED: START A TEMPORARY CHECK POPUP
void ChessGUI::triggerCheckPopup(const std::string& message) {
    checkPopupActive = true;
    checkPopupMessage = message;
    checkPopupClock.restart();
}

// ADDED: DRAW TEMPORARY CHECK POPUP
// ADDED: DRAW PRETTIER TEMPORARY CHECK POPUP
void ChessGUI::drawCheckPopup() {
    if (!checkPopupActive) return;

    // AUTO-HIDE AFTER TIME EXPIRES
    if (checkPopupClock.getElapsedTime().asSeconds() > checkPopupDuration) {
        checkPopupActive = false;
        return;
    }

    sf::Vector2u winSize = window.getSize();

    float popupW = 520.f;
    float popupH = 90.f;
    float x = (winSize.x - popupW) / 2.f;
    float y = 35.f;

    // SHADOW
    sf::RectangleShape shadow({popupW, popupH});
    shadow.setPosition({x + 6.f, y + 6.f});
    shadow.setFillColor(sf::Color(0, 0, 0, 80));
    window.draw(shadow);

    // MAIN POPUP PANEL
    sf::RectangleShape popup({popupW, popupH});
    popup.setPosition({x, y});
    popup.setFillColor(sf::Color(255, 245, 245));
    popup.setOutlineColor(sf::Color(180, 30, 30));
    popup.setOutlineThickness(3.f);
    window.draw(popup);

    // LEFT ACCENT BAR
    sf::RectangleShape accent({10.f, popupH});
    accent.setPosition({x, y});
    accent.setFillColor(sf::Color(190, 30, 30));
    window.draw(accent);

    // WARNING ICON CIRCLE
    sf::CircleShape icon(18.f);
    icon.setPosition({x + 22.f, y + 27.f});
    icon.setFillColor(sf::Color(190, 30, 30));
    window.draw(icon);

    sf::Text iconText(font, "!", 24);
    iconText.setFillColor(sf::Color::White);
    iconText.setPosition({x + 31.f, y + 19.f});
    window.draw(iconText);

    // MESSAGE TEXT
    sf::Text text(font, checkPopupMessage, 24);
    text.setFillColor(sf::Color(120, 0, 0));
    text.setPosition({x + 65.f, y + 28.f});
    window.draw(text);
}

// ADDED: START A TEMPORARY INVALID MOVE POPUP
void ChessGUI::triggerInvalidMovePopup(const std::string& message) {
    invalidMovePopupActive = true;
    invalidMovePopupMessage = message;
    invalidMovePopupClock.restart();
}

// ADDED: DRAW TEMPORARY INVALID MOVE POPUP
void ChessGUI::drawInvalidMovePopup() {
    if (!invalidMovePopupActive) return;

    // AUTO-HIDE AFTER TIME EXPIRES
    if (invalidMovePopupClock.getElapsedTime().asSeconds() > invalidMovePopupDuration) {
        invalidMovePopupActive = false;
        return;
    }

    sf::Vector2u winSize = window.getSize();

    float popupW = 520.f;
    float popupH = 90.f;
    float x = (winSize.x - popupW) / 2.f;
    float y = 140.f;

    // SHADOW
    sf::RectangleShape shadow({popupW, popupH});
    shadow.setPosition({x + 6.f, y + 6.f});
    shadow.setFillColor(sf::Color(0, 0, 0, 80));
    window.draw(shadow);

    // MAIN POPUP PANEL
    sf::RectangleShape popup({popupW, popupH});
    popup.setPosition({x, y});
    popup.setFillColor(sf::Color(255, 250, 235));
    popup.setOutlineColor(sf::Color(200, 120, 0));
    popup.setOutlineThickness(3.f);
    window.draw(popup);

    // LEFT ACCENT BAR
    sf::RectangleShape accent({10.f, popupH});
    accent.setPosition({x, y});
    accent.setFillColor(sf::Color(200, 120, 0));
    window.draw(accent);

    // WARNING ICON CIRCLE
    sf::CircleShape icon(18.f);
    icon.setPosition({x + 22.f, y + 27.f});
    icon.setFillColor(sf::Color(200, 120, 0));
    window.draw(icon);

    sf::Text iconText(font, "!", 24);
    iconText.setFillColor(sf::Color::White);
    iconText.setPosition({x + 31.f, y + 19.f});
    window.draw(iconText);

    // MESSAGE TEXT
    sf::Text text(font, invalidMovePopupMessage, 24);
    text.setFillColor(sf::Color(120, 70, 0));
    text.setPosition({x + 65.f, y + 28.f});
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

// ADDED: DRAW PRETTIER GAME OVER POPUP
void ChessGUI::drawGameOverPopup() {
    if (!gameOver) return;

    sf::Vector2u winSize = window.getSize();

    // DARK TRANSPARENT OVERLAY
    sf::RectangleShape overlay({(float)winSize.x, (float)winSize.y});
    overlay.setPosition({0.f, 0.f});
    overlay.setFillColor(sf::Color(0, 0, 0, 170));
    window.draw(overlay);

    // POPUP SIZE AND CENTER POSITION
    float popupW = 600.f;
    float popupH = 320.f;
    float x = (winSize.x - popupW) / 2.f;
    float y = (winSize.y - popupH) / 2.f;

    // SHADOW
    sf::RectangleShape shadow({popupW, popupH});
    shadow.setPosition({x + 8.f, y + 8.f});
    shadow.setFillColor(sf::Color(0, 0, 0, 90));
    window.draw(shadow);

    // MAIN PANEL
    sf::RectangleShape popup({popupW, popupH});
    popup.setPosition({x, y});
    popup.setFillColor(sf::Color(248, 248, 248));
    popup.setOutlineColor(sf::Color(40, 40, 40));
    popup.setOutlineThickness(3.f);
    window.draw(popup);

    // TOP TITLE BAR
    sf::RectangleShape topBar({popupW, 55.f});
    topBar.setPosition({x, y});
    topBar.setFillColor(sf::Color(35, 35, 55));
    window.draw(topBar);

    // TITLE
    sf::Text title(font, "GAME OVER", 34);
    title.setFillColor(sf::Color::White);
    title.setPosition({x + 200.f, y + 10.f});
    window.draw(title);

    // MAIN RESULT TEXT
    sf::Text message(font, gameOverMessage, 26);
    message.setFillColor(sf::Color(20, 20, 20));
    message.setPosition({x + 40.f, y + 85.f});
    window.draw(message);

    // REASON TEXT
    sf::Text reason(font, gameOverReason, 20);
    reason.setFillColor(sf::Color(90, 90, 90));
    reason.setPosition({x + 40.f, y + 130.f});
    window.draw(reason);

    // SMALL INSTRUCTION LINE
    sf::Text hint(font, "Choose what to do next:", 18);
    hint.setFillColor(sf::Color(100, 100, 100));
    hint.setPosition({x + 40.f, y + 170.f});
    window.draw(hint);

    // RESTART BUTTON SHADOW
    sf::RectangleShape restartShadow({160.f, 50.f});
    restartShadow.setPosition({x + 72.f, y + 225.f});
    restartShadow.setFillColor(sf::Color(0, 0, 0, 70));
    window.draw(restartShadow);

    // RESTART BUTTON
    sf::RectangleShape restartButton({160.f, 50.f});
    restartButton.setPosition({x + 68.f, y + 221.f});
    restartButton.setFillColor(sf::Color(70, 160, 70));
    restartButton.setOutlineColor(sf::Color::Black);
    restartButton.setOutlineThickness(2.f);
    window.draw(restartButton);

    sf::Text restartText(font, "Restart", 22);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition({x + 110.f, y + 232.f});
    window.draw(restartText);

    // EXIT BUTTON SHADOW
    sf::RectangleShape exitShadow({160.f, 50.f});
    exitShadow.setPosition({x + 372.f, y + 225.f});
    exitShadow.setFillColor(sf::Color(0, 0, 0, 70));
    window.draw(exitShadow);

    // EXIT BUTTON
    sf::RectangleShape exitButton({160.f, 50.f});
    exitButton.setPosition({x + 368.f, y + 221.f});
    exitButton.setFillColor(sf::Color(180, 60, 60));
    exitButton.setOutlineColor(sf::Color::Black);
    exitButton.setOutlineThickness(2.f);
    window.draw(exitButton);

    sf::Text exitText(font, "Exit", 22);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition({x + 429.f, y + 232.f});
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
                triggerInvalidMovePopup("Invalid move! Try again.");
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
        drawRowLabels();             // ADDED: SHOW ROW NUMBERS 1-8
        drawCheckHighlight();
        drawLegalMoveHighlights();
        drawPieces();
        drawStatusBar();
        drawCheckPopup();
        drawInvalidMovePopup();
        drawGameOverPopup();

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

        // ADDED: CLEAR INVALID MOVE POPUP ON RESTART
        invalidMovePopupActive = false;
        invalidMovePopupMessage = "";

        statusMessage = "White's turn. Select a piece to move.";
    }
    else if (exitButtonBounds.contains(point)) {
        window.close();
    }
}

//drawing pieces funcions 
// ADDED: LOAD ALL PIECE TEXTURES FROM ASSETS/Pieces
bool ChessGUI::loadPieceTextures() {
    std::string base = "assets/pieces/";

    bool ok = true;

    ok = ok && whiteKingTexture.loadFromFile(base + "w_king_png_512px.png");
    ok = ok && whiteQueenTexture.loadFromFile(base + "w_queen_png_512px.png");
    ok = ok && whiteRookTexture.loadFromFile(base + "w_rook_png_512px.png");
    ok = ok && whiteBishopTexture.loadFromFile(base + "w_bishop_png_512px.png");
    ok = ok && whiteKnightTexture.loadFromFile(base + "w_knight_png_512px.png");
    ok = ok && whitePawnTexture.loadFromFile(base + "w_pawn_png_512px.png");

    ok = ok && blackKingTexture.loadFromFile(base + "b_king_png_512px.png");
    ok = ok && blackQueenTexture.loadFromFile(base + "b_queen_png_512px.png");
    ok = ok && blackRookTexture.loadFromFile(base + "b_rook_png_512px.png");
    ok = ok && blackBishopTexture.loadFromFile(base + "b_bishop_png_512px.png");
    ok = ok && blackKnightTexture.loadFromFile(base + "b_knight_png_512px.png");
    ok = ok && blackPawnTexture.loadFromFile(base + "b_pawn_png_512px.png");

    return ok;
}

// ADDED: GET THE CORRECT TEXTURE FOR EACH PIECE
sf::Texture* ChessGUI::getTextureForPiece(Piece* p) {
    if (p == nullptr) return nullptr;

    bool white = p->isWhite();

    switch (p->getType()) {
        case KING:   return white ? &whiteKingTexture   : &blackKingTexture;
        case QUEEN:  return white ? &whiteQueenTexture  : &blackQueenTexture;
        case ROOK:   return white ? &whiteRookTexture    : &blackRookTexture;
        case BISHOP: return white ? &whiteBishopTexture  : &blackBishopTexture;
        case KNIGHT: return white ? &whiteKnightTexture  : &blackKnightTexture;
        case PAWN:   return white ? &whitePawnTexture    : &blackPawnTexture;
        default:     return nullptr;
    }
}

// ADDED: DRAW COLUMN LABELS a-h ON THE GUI BOARD
void ChessGUI::drawColumnLabels() {
    for (int c = 0; c < BOARD_SIZE; c++) {
        std::string file(1, static_cast<char>('a' + c));

        sf::Text label(font, file, 18);
        label.setFillColor(sf::Color(230, 230, 230));

        // CENTER THE LABEL OVER EACH COLUMN
        sf::FloatRect bounds = label.getLocalBounds();
        label.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });

        // PLACE LABELS JUST UNDER THE BOARD INSIDE THE STATUS BAR
        label.setPosition({
            c * TILE_SIZE + TILE_SIZE / 2.f,
            8.f * TILE_SIZE + 10.f
        });

        window.draw(label);
    }
}

// ADDED: DRAW ROW LABELS 1-8 ON THE RIGHT SIDE OF THE BOARD
void ChessGUI::drawRowLabels() {
    // ADDED: SIDE GUTTER BACKGROUND
    sf::RectangleShape gutter({ROW_LABEL_GUTTER, 8.f * TILE_SIZE});
    gutter.setPosition({8.f * TILE_SIZE, 0.f});
    gutter.setFillColor(sf::Color(45, 45, 45));
    window.draw(gutter);

    for (int r = 0; r < BOARD_SIZE; r++) {
        std::string labelStr = std::to_string(8 - r);

        sf::Text label(font, labelStr, 22);
        label.setFillColor(sf::Color(230, 230, 230));

        // CENTER THE LABEL IN THE GUTTER
        sf::FloatRect bounds = label.getLocalBounds();
        label.setOrigin({
            bounds.position.x + bounds.size.x / 2.f,
            bounds.position.y + bounds.size.y / 2.f
        });

        label.setPosition({
            8.f * TILE_SIZE + ROW_LABEL_GUTTER / 2.f,
            r * TILE_SIZE + TILE_SIZE / 2.f
        });

        window.draw(label);
    }
}