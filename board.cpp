#include "board.h"
#include "piece.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"
#include <iostream>
#include <cstring>
using namespace std;

Board::Board() {
    whiteTurn = true;
    enPassantCol = -1;
    enPassantRow = -1;

    whiteKingSideCastle  = true;
    whiteQueenSideCastle = true;
    blackKingSideCastle  = true;
    blackQueenSideCastle = true;

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            squares[i][j] = NULL;
        }
    }
}

Board::~Board() {
    clearBoard();
}

void Board::clearBoard() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (squares[i][j] != NULL) {
                delete squares[i][j];
                squares[i][j] = NULL;
            }
        }
    }
}

// -------------------------------------------------------
// Board Initialization
// -------------------------------------------------------

void Board::initializeBoard() {
    clearBoard();

    // Place black pieces on row 0
    squares[0][0] = new Rook(false);
    squares[0][1] = new Knight(false);
    squares[0][2] = new Bishop(false);
    squares[0][3] = new Queen(false);
    squares[0][4] = new King(false);
    squares[0][5] = new Bishop(false);
    squares[0][6] = new Knight(false);
    squares[0][7] = new Rook(false);

    // Black pawns on row 1
    for (int j = 0; j < BOARD_SIZE; j++) {
        squares[1][j] = new Pawn(false);
    }

    // Empty squares in the middle
    for (int i = 2; i < 6; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            squares[i][j] = NULL;
        }
    }

    // White pawns on row 6
    for (int j = 0; j < BOARD_SIZE; j++) {
        squares[6][j] = new Pawn(true);
    }

    // Place white pieces on row 7
    squares[7][0] = new Rook(true);
    squares[7][1] = new Knight(true);
    squares[7][2] = new Bishop(true);
    squares[7][3] = new Queen(true);
    squares[7][4] = new King(true);
    squares[7][5] = new Bishop(true);
    squares[7][6] = new Knight(true);
    squares[7][7] = new Rook(true);

    whiteTurn = true;
    enPassantCol = -1;
    enPassantRow = -1;

    whiteKingSideCastle  = true;
    whiteQueenSideCastle = true;
    blackKingSideCastle  = true;
    blackQueenSideCastle = true;
}

// -------------------------------------------------------
// Display
// -------------------------------------------------------

void Board::printColumnLabels() {
    cout << "   ";
    for (int j = 0; j < BOARD_SIZE; j++) {
        cout << "  " << (char)('a' + j) << " ";
    }
    cout << endl;
}

void Board::displayBoard() {
    cout << endl;
    printColumnLabels();
    cout << "   +---+---+---+---+---+---+---+---+" << endl;

    for (int i = 0; i < BOARD_SIZE; i++) {
        // Row number (8 down to 1 from white's view)
        cout << " " << (8 - i) << " |";

        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << " ";
            if (squares[i][j] == NULL) {
                // Checkerboard pattern for empty squares
                if ((i + j) % 2 == 0) {
                    cout << ".";
                } else {
                    cout << " ";
                }
            } else {
                cout << squares[i][j]->getSymbol();
            }
            cout << " |";
        }

        cout << " " << (8 - i) << endl;
        cout << "   +---+---+---+---+---+---+---+---+" << endl;
    }

    printColumnLabels();
    cout << endl;
}

void Board::displayBoardFlipped() {
    cout << endl;

    // Column labels from h to a
    cout << "   ";
    for (int j = BOARD_SIZE - 1; j >= 0; j--) {
        cout << "  " << (char)('a' + j) << " ";
    }
    cout << endl;

    cout << "   +---+---+---+---+---+---+---+---+" << endl;

    for (int i = BOARD_SIZE - 1; i >= 0; i--) {
        cout << " " << (8 - i) << " |";

        for (int j = BOARD_SIZE - 1; j >= 0; j--) {
            cout << " ";
            if (squares[i][j] == NULL) {
                if ((i + j) % 2 == 0) {
                    cout << ".";
                } else {
                    cout << " ";
                }
            } else {
                cout << squares[i][j]->getSymbol();
            }
            cout << " |";
        }

        cout << " " << (8 - i) << endl;
        cout << "   +---+---+---+---+---+---+---+---+" << endl;
    }

    cout << "   ";
    for (int j = BOARD_SIZE - 1; j >= 0; j--) {
        cout << "  " << (char)('a' + j) << " ";
    }
    cout << endl << endl;
}

// -------------------------------------------------------
// Piece Access
// -------------------------------------------------------

Piece* Board::getPiece(int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return NULL;
    }
    return squares[row][col];
}

void Board::setPiece(int row, int col, Piece* piece) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return;
    }
    squares[row][col] = piece;
}

void Board::removePiece(int row, int col) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return;
    }
    squares[row][col] = NULL;
}

// -------------------------------------------------------
// Turn Management
// -------------------------------------------------------

bool Board::isWhiteTurn() {
    return whiteTurn;
}

void Board::switchTurn() {
    whiteTurn = !whiteTurn;
}

// -------------------------------------------------------
// En Passant
// -------------------------------------------------------

int Board::getEnPassantRow() {
    return enPassantRow;
}

int Board::getEnPassantCol() {
    return enPassantCol;
}

void Board::setEnPassant(int row, int col) {
    enPassantRow = row;
    enPassantCol = col;
}

void Board::clearEnPassant() {
    enPassantRow = -1;
    enPassantCol = -1;
}

// -------------------------------------------------------
// Castling Rights
// -------------------------------------------------------

bool Board::getWhiteKingSideCastle() {
    return whiteKingSideCastle;
}

bool Board::getWhiteQueenSideCastle() {
    return whiteQueenSideCastle;
}

bool Board::getBlackKingSideCastle() {
    return blackKingSideCastle;
}

bool Board::getBlackQueenSideCastle() {
    return blackQueenSideCastle;
}

void Board::updateCastlingRights(int fromRow, int fromCol) {
    // If the king moved, remove both castling rights for that side
    if (fromRow == 7 && fromCol == 4) {
        whiteKingSideCastle  = false;
        whiteQueenSideCastle = false;
    }
    if (fromRow == 0 && fromCol == 4) {
        blackKingSideCastle  = false;
        blackQueenSideCastle = false;
    }

    // If a rook moved, remove that side's castling right
    if (fromRow == 7 && fromCol == 0) whiteQueenSideCastle = false;
    if (fromRow == 7 && fromCol == 7) whiteKingSideCastle  = false;
    if (fromRow == 0 && fromCol == 0) blackQueenSideCastle = false;
    if (fromRow == 0 && fromCol == 7) blackKingSideCastle  = false;
}

// -------------------------------------------------------
// Square Under Attack Check
// -------------------------------------------------------

bool Board::isSquareUnderAttack(int row, int col, bool byWhite) {
    // Check if any piece of color 'byWhite' attacks the given square
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            Piece* piece = squares[i][j];
            if (piece == NULL) continue;
            if (piece->isWhite() != byWhite) continue;

            // Check if this piece can move to (row, col)
            if (piece->canAttack(i, j, row, col, this)) {
                return true;
            }
        }
    }
    return false;
}

Piece* Board::findKing(bool white) {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (squares[i][j] == NULL) continue;
            if (squares[i][j]->isWhite() == white &&
                squares[i][j]->getType() == KING) {
                return squares[i][j];
            }
        }
    }
    return NULL;
}

// -------------------------------------------------------
// Move Execution
// -------------------------------------------------------

bool Board::movePiece(int fromRow, int fromCol, int toRow, int toCol) {
    Piece* piece = squares[fromRow][fromCol];

    if (piece == NULL) {
        cout << "No piece at that position." << endl;
        return false;
    }

    if (piece->isWhite() != whiteTurn) {
        cout << "That is not your piece." << endl;
        return false;
    }

    // Ask Ali's piece if this move is valid
    if (!piece->isValidMove(fromRow, fromCol, toRow, toCol, this)) {
        cout << "Invalid move for this piece." << endl;
        return false;
    }

    // Save old state in case we need to undo (for check detection)
    Piece* capturedPiece = squares[toRow][toCol];
    bool wasEnPassant = false;
    bool wasCastle    = false;
    int castleRookFromCol = -1;
    int castleRookToCol   = -1;

    // --- En Passant capture ---
    if (piece->getType() == PAWN) {
        if (toCol == enPassantCol && toRow == enPassantRow) {
            wasEnPassant = true;
            // The captured pawn is on the same row as the moving pawn
            capturedPiece = squares[fromRow][toCol];
        }
    }

    // --- Castling detection ---
    if (piece->getType() == KING) {
        int colDiff = toCol - fromCol;
        if (colDiff == 2) {
            // King side castle
            wasCastle = true;
            castleRookFromCol = 7;
            castleRookToCol   = 5;
        } else if (colDiff == -2) {
            // Queen side castle
            wasCastle = true;
            castleRookFromCol = 0;
            castleRookToCol   = 3;
        }
    }

    // --- Perform the move on the board ---

    // Handle en passant removal
    if (wasEnPassant) {
        squares[fromRow][toCol] = NULL;
    }

    // Handle castling rook move
    if (wasCastle) {
        squares[fromRow][castleRookToCol]   = squares[fromRow][castleRookFromCol];
        squares[fromRow][castleRookFromCol] = NULL;
    }

    // Move the piece
    squares[toRow][toCol]   = piece;
    squares[fromRow][fromCol] = NULL;
    piece->setMoved(true);

    // --- Check if our own king is in check after this move ---
    // We use Usman's interface here
    // Find king position
    int kingRow = -1, kingCol = -1;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (squares[i][j] != NULL &&
                squares[i][j]->isWhite() == whiteTurn &&
                squares[i][j]->getType() == KING) {
                kingRow = i;
                kingCol = j;
            }
        }
    }

    bool movedIntoCheck = isSquareUnderAttack(kingRow, kingCol, !whiteTurn);

    if (movedIntoCheck) {
        // Undo the move
        squares[fromRow][fromCol] = piece;
        squares[toRow][toCol]     = (wasEnPassant ? NULL : capturedPiece);
        piece->setMoved(false);

        if (wasEnPassant) {
            squares[fromRow][toCol] = capturedPiece;
        }

        if (wasCastle) {
            squares[fromRow][castleRookFromCol] = squares[fromRow][castleRookToCol];
            squares[fromRow][castleRookToCol]   = NULL;
        }

        cout << "That move puts your king in check!" << endl;
        return false;
    }

    // Clean up captured piece
    if (capturedPiece != NULL && !wasEnPassant) {
        delete capturedPiece;
    } else if (wasEnPassant && capturedPiece != NULL) {
        delete capturedPiece;
    }

    // --- Update en passant target ---
    clearEnPassant();
    if (piece->getType() == PAWN) {
        int rowDiff = toRow - fromRow;
        // Two-square pawn push
        if (rowDiff == -2) {
            // White pawn moved two squares up
            setEnPassant(fromRow - 1, fromCol);
        } else if (rowDiff == 2) {
            // Black pawn moved two squares down
            setEnPassant(fromRow + 1, fromCol);
        }
    }

    // --- Update castling rights ---
    updateCastlingRights(fromRow, fromCol);

    // --- Handle pawn promotion ---
    if (piece->getType() == PAWN) {
        if (toRow == 0 || toRow == 7) {
            handlePromotion(toRow, toCol);
        }
    }

    return true;
}

// -------------------------------------------------------
// Pawn Promotion
// -------------------------------------------------------

void Board::handlePromotion(int row, int col) {
    cout << endl;
    cout << "Pawn promotion! Choose a piece:" << endl;
    cout << "  1. Queen" << endl;
    cout << "  2. Rook" << endl;
    cout << "  3. Bishop" << endl;
    cout << "  4. Knight" << endl;
    cout << "Enter choice (1-4): ";

    bool isWhitePiece = squares[row][col]->isWhite();
    int choice;
    cin >> choice;

    // Delete the pawn first
    delete squares[row][col];
    squares[row][col] = NULL;

    if (choice == 1) {
        squares[row][col] = new Queen(isWhitePiece);
    } else if (choice == 2) {
        squares[row][col] = new Rook(isWhitePiece);
    } else if (choice == 3) {
        squares[row][col] = new Bishop(isWhitePiece);
    } else if (choice == 4) {
        squares[row][col] = new Knight(isWhitePiece);
    } else {
        // Default to queen if invalid input
        cout << "Invalid choice. Defaulting to Queen." << endl;
        squares[row][col] = new Queen(isWhitePiece);
    }

    cout << "Promoted to ";
    cout << squares[row][col]->getSymbol();
    cout << "!" << endl;
}

// -------------------------------------------------------
// Input Parsing
// -------------------------------------------------------

bool Board::parseInput(char input[], int &row, int &col) {
    // Expected format: "e2" or "e 2"
    // We'll handle both "e2e4" style as two separate calls
    // This function handles a single square like "e2"

    int len = strlen(input);
    if (len < 2) return false;

    char colChar = input[0];
    char rowChar = input[1];

    if (colChar < 'a' || colChar > 'h') return false;
    if (rowChar < '1' || rowChar > '8') return false;

    col = colChar - 'a';
    row = 8 - (rowChar - '0');  // Convert to array index

    return true;
}

// -------------------------------------------------------
// Game Loop
// -------------------------------------------------------

void Board::printWelcome() {
    cout << "===============================" << endl;
    cout << "      CHESS GAME - OOP Project " << endl;
    cout << "===============================" << endl;
    cout << "  Ali   - Pieces & Move Logic  " << endl;
    cout << "  Subhan - Board & Game Loop   " << endl;
    cout << "  Usman  - Game Conditions     " << endl;
    cout << "===============================" << endl;
    cout << endl;
    cout << "How to play:" << endl;
    cout << "  Enter moves like: e2 e4" << endl;
    cout << "  Type 'quit' to exit" << endl;
    cout << "  Type 'flip' to flip the board" << endl;
    cout << "  Type 'help' for commands" << endl;
    cout << endl;
}

void Board::startGame() {
    // External game controller (Usman) will call this
    // This is the main game loop

    printWelcome();
    initializeBoard();

    // We need access to Usman's game condition functions
    // They will be passed or the Game class will call them
    // For now this loop handles input and display

    bool flipped   = false;
    bool gameOver  = false;
    char from[20];
    char to[20];

    while (!gameOver) {
        if (flipped && !whiteTurn) {
            displayBoardFlipped();
        } else {
            displayBoard();
        }

        // Show whose turn it is
        if (whiteTurn) {
            cout << "White's turn" << endl;
        } else {
            cout << "Black's turn" << endl;
        }

        cout << "Enter move (from to) or command: ";
        cin >> from;

        // Handle commands
        if (strcmp(from, "quit") == 0 || strcmp(from, "exit") == 0) {
            cout << "Thanks for playing!" << endl;
            gameOver = true;
            break;
        }

        if (strcmp(from, "flip") == 0) {
            flipped = !flipped;
            cout << "Board flipped." << endl;
            continue;
        }

        if (strcmp(from, "help") == 0) {
            cout << endl;
            cout << "Commands:" << endl;
            cout << "  e2 e4   - move piece from e2 to e4" << endl;
            cout << "  flip    - flip the board view" << endl;
            cout << "  quit    - exit the game" << endl;
            cout << endl;
            continue;
        }

        // Read destination
        cin >> to;

        // Parse from and to squares
        int fromRow, fromCol, toRow, toCol;
        fromRow = fromCol = toRow = toCol = -1;

        if (!parseInput(from, fromRow, fromCol)) {
            cout << "Invalid input format. Try something like: e2 e4" << endl;
            continue;
        }

        if (!parseInput(to, toRow, toCol)) {
            cout << "Invalid input format. Try something like: e2 e4" << endl;
            continue;
        }

        // Attempt the move
        bool moved = movePiece(fromRow, fromCol, toRow, toCol);

        if (moved) {
            switchTurn();
            // Note: Usman's code will check for check/checkmate/stalemate
            // after each successful move and set gameOver accordingly
        }
    }
}