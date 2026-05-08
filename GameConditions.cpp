#include <iostream>
#include "GameConditions.h"
#include "board.h"
#include <cstdlib>

using namespace std;

GameConditions::GameConditions(Board* b) {
    board = b;
}

bool GameConditions::is_in_check(Color color) {

    Piece* king = board->findKing(color == WHITE); 
    if (king == NULL) { //king is not found
        return false;
    }

    int kingRow = -1;
    int kingCol = -1;
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (board->getPiece(r, c) == king) {
                kingRow = r;
                kingCol = c;
            }
        }
    }

    if (kingRow == -1) { //king not found
        return false;
    }

    return board->isSquareUnderAttack(kingRow, kingCol, color != WHITE);
}

bool GameConditions::is_checkmate(Color color) {

    // if the king is not in check, checkmate is not possible
    if (!is_in_check(color)) {
        return false;
    }

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {

            Piece* p = board->getPiece(r, c);

            if (p && p->getColor() == color) {

                // replaced by p->getValidMoves(board) when Ali is ready
                // get every square this piece can move to
                Position moves[28];
                int moveCount = 0;
                Position savedPos = p->getPosition();
                p->setPosition(Position(r, c));
                p->getValidMoves(*board, moves, moveCount);
                p->setPosition(savedPos); 
                            for (int i = 0; i < moveCount; i++) {
                int tr = moves[i].row; 
                int tc = moves[i].col;

                Piece* temp = board->getPiece(tr, tc);

                bool isCastling = false;
                bool isEnPassant = false;
                Piece* capturedEnPassant = nullptr;
                int rookFromCol = -1, rookToCol = -1;

                // DETECT CASTLING
                if (p->getType() == KING && abs(tc - c) == 2) {
                    isCastling = true;
                    rookFromCol = (tc > c) ? 7 : 0;
                    rookToCol   = (tc > c) ? 5 : 3;
                }

                // DETECT EN PASSANT
                if (p->getType() == PAWN && tc != c && temp == nullptr) {
                    isEnPassant = true;
                    int direction = (p->getColor() == WHITE) ? -1 : 1;
                    capturedEnPassant = board->getPiece(tr - direction, tc);
                }

                // PERFORMS THE MOVE
                board->setPiece(tr, tc, p);
                board->removePiece(r, c);

                //SPECIAL MOVES
                if (isCastling) {
                    Piece* rook = board->getPiece(r, rookFromCol);
                    board->setPiece(r, rookToCol, rook);
                    board->removePiece(r, rookFromCol);
                    if (rook) rook->setPosition(Position(r, rookToCol));
                }
                if (isEnPassant && capturedEnPassant) {
                    int direction = (p->getColor() == WHITE) ? -1 : 1;
                    board->removePiece(tr - direction, tc);
                }

                p->setPosition(Position(tr, tc)); // ADDED: UPDATE PIECE POSITION DURING SIMULATION

                bool still_in_check = is_in_check(color);

                //UNDO THE MOVE
                board->setPiece(r, c, p);
                board->setPiece(tr, tc, temp);

                //UNDO SPECIAL MOVES
                if (isCastling) {
                    Piece* rook = board->getPiece(r, rookToCol);
                    board->setPiece(r, rookFromCol, rook);
                    board->removePiece(r, rookToCol);
                    if (rook) rook->setPosition(Position(r, rookFromCol));
                }
                if (isEnPassant && capturedEnPassant) {
                    int direction = (p->getColor() == WHITE) ? -1 : 1;
                    board->setPiece(tr - direction, tc, capturedEnPassant);
                }

                p->setPosition(Position(r, c)); 
                if (!still_in_check) {
                    return false;
                    // at least one legal move exists so no checkmate
                }
            }
            }
        }
    }

    return true;
    // every piece was checked
    // every possible move still left the king in check
}

bool GameConditions::is_stalemate(Color color) {
    if (is_in_check(color)) {
        return false;
    }

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Piece* p = board->getPiece(r, c);
            if (p == NULL) continue;
            if (p->getColor() != color) continue;

            Position moves[28];
            int moveCount = 0;
            Position savedPos = p->getPosition();
            p->setPosition(Position(r, c));
            p->getValidMoves(*board, moves, moveCount);
            p->setPosition(savedPos); // ADDED: RESTORE POSITION AFTER GENERATING MOVES

            for (int i = 0; i < moveCount; i++) { // loop through every possible move this piece has
                int tr = moves[i].row; // tr = target row , where the piece is moved
                int tc = moves[i].col;

                Piece* temp = board->getPiece(tr, tc);

                bool isCastling = false;
                bool isEnPassant = false;
                Piece* capturedEnPassant = nullptr;
                int rookFromCol = -1, rookToCol = -1;

                // DETECT CASTLING
                if (p->getType() == KING && abs(tc - c) == 2) {
                    isCastling = true;
                    rookFromCol = (tc > c) ? 7 : 0;
                    rookToCol   = (tc > c) ? 5 : 3;
                }

                // DETECT EN PASSANT
                if (p->getType() == PAWN && tc != c && temp == nullptr) {
                    isEnPassant = true;
                    int direction = (p->getColor() == WHITE) ? -1 : 1;
                    capturedEnPassant = board->getPiece(tr - direction, tc);
                }

                // PERFORM THE MOVE
                board->setPiece(tr, tc, p);
                board->removePiece(r, c);

                // APPLY SPECIAL MOVES
                if (isCastling) {
                    Piece* rook = board->getPiece(r, rookFromCol);
                    board->setPiece(r, rookToCol, rook);
                    board->removePiece(r, rookFromCol);
                    if (rook) rook->setPosition(Position(r, rookToCol));
                }
                if (isEnPassant && capturedEnPassant) {
                    int direction = (p->getColor() == WHITE) ? -1 : 1;
                    board->removePiece(tr - direction, tc);
                }

                p->setPosition(Position(tr, tc));
                bool still_in_check = is_in_check(color);

                // UNDO THE MOVE
                board->setPiece(r, c, p);
                board->setPiece(tr, tc, temp);

                // UNDO SPECIAL MOVES
                if (isCastling) {
                    Piece* rook = board->getPiece(r, rookToCol);
                    board->setPiece(r, rookFromCol, rook);
                    board->removePiece(r, rookToCol);
                    if (rook) rook->setPosition(Position(r, rookFromCol));
                }
                if (isEnPassant && capturedEnPassant) {
                    int direction = (p->getColor() == WHITE) ? -1 : 1;
                    board->setPiece(tr - direction, tc, capturedEnPassant);
                }

                p->setPosition(Position(r, c));
                if (!still_in_check) {
                    return false;  // at least one legal move exists, not stalemate
                }
            }
        }
    }
    return true;  // no legal moves and not in check, stalemate
}

bool GameConditions::is_draw(Color color) {

    if (is_threefold_repetition()) {
        return true;
    }

    // Stalemate for current player or opponent = draw
    if (is_stalemate(color) || is_stalemate(getOpponentColor(color))) {
        return true;
    }

    // INSUFFICIENT MATERIAL CHECK
    int whiteCount = 0;
    int blackCount = 0;
    bool whiteHasMajor = false;   // Queen, Rook, or Pawn
    bool blackHasMajor = false;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Piece* p = board->getPiece(r, c);
            if (p == nullptr) continue;
            if (p->getType() == KING) continue; // ignore kings

            if (p->getColor() == WHITE) {
                whiteCount++;
                if (p->getType() == QUEEN || p->getType() == ROOK || p->getType() == PAWN) {
                    whiteHasMajor = true;
                }
            } else {
                blackCount++;
                if (p->getType() == QUEEN || p->getType() == ROOK || p->getType() == PAWN) {
                    blackHasMajor = true;
                }
            }
        }
    }

    // King vs King
    if (whiteCount == 0 && blackCount == 0) {
        return true;
    }

    // King + Bishop vs King
    // King + Knight vs King
    if (whiteCount == 1 && blackCount == 0 && !whiteHasMajor) return true;
    if (blackCount == 1 && whiteCount == 0 && !blackHasMajor) return true;

    // King + Bishop vs King + Bishop
    if (whiteCount == 1 && blackCount == 1 && !whiteHasMajor && !blackHasMajor) {
        return true;
    }

    return false;
}

// Helper functions
Color GameConditions::getOpponentColor(Color color) {
    return (color == WHITE) ? BLACK : WHITE;
}

bool GameConditions::is_threefold_repetition() {
    std::string sig = board->getBoardSignature();
    return board->getPositionCount(sig) >= 3;
}