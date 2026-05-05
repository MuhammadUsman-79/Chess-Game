#include "GameConditions.h"
#include <iostream>
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
                p->getValidMoves(*board, moves, moveCount);

                for (int i = 0; i < moveCount; i++) {
                    int tr = moves[i].row; 

                    int tc = moves[i].col;
                    // store the destination column of this move

                    Piece* temp = board->getPiece(tr, tc);
                    // save whatever is placed on the destination square
                    // could be an opponent piece or nullptr
                    // need this to undo the move later

                    board->setPiece(tr, tc, p);
                    // move the piece to the destination square

                    board->removePiece(r, c);
                    // empty the square our piece just left

                    bool still_in_check = is_in_check(color);
                    // test if the king is still in check after this move

                    board->setPiece(r, c, p);
                    // undo the move, put our piece back where it was

                    board->setPiece(tr, tc, temp);
                    // restore the destination square to what it was before

                    if (!still_in_check) {
                        return false;
                        // at least one legal move exists, not checkmate
                    }
                }
            }
        }
    }

    return true;
    // every piece was checked
    // every possible move still left the king in check
}

// SAME LOGIC AS CHECKMATE BUT PLAYER IS NOT IN CHECK
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
            p->getValidMoves(*board, moves, moveCount);

            for (int i = 0; i < moveCount; i++) { // loop through every possible move this piece has
                int tr = moves[i].row; // tr = target row , where the piece is moved

                int tc = moves[i].col;
                // store the destination column of this move

                Piece* temp = board->getPiece(tr, tc);
                // save whatever is placed on the destination square
                // could be an opponent piece or nullptr
                // need this to undo the move later

                board->setPiece(tr, tc, p);
                // move the piece to the destination square

                board->removePiece(r, c);
                // empty the square our piece just left

                bool still_in_check = is_in_check(color);
                // test if the king is still in check after this move

                board->setPiece(r, c, p);
                // undo the move, put our piece back where it was

                board->setPiece(tr, tc, temp);
                // restore the destination square to what it was before

                if (!still_in_check) {
                    return false;
                    // at least one legal move exists, not stalemate
                }
            }
        }
    }
    return true;
    // no legal moves and not in check, stalemate
}

bool GameConditions::is_draw(Color color) {
    if (is_stalemate(color)) { 
        return true;
    }
    Color opponent;
    if (color == WHITE) {
    opponent = BLACK;
    } else {
    opponent = WHITE;
    }
if (is_stalemate(opponent)) { 
    return true; 
    }

    int whitePieces = 0;
    int blackPieces = 0;
    bool whiteHasMajorPiece = false;
    bool blackHasMajorPiece = false;

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            Piece* p = board->getPiece(r, c);
            if (p == NULL) continue;

            if (p->getColor() == WHITE) {
                whitePieces++;
                // WAITING FOR ALI TO ADD getType() TO piece.h
                // if (p->getType() != BISHOP && p->getType() != KNIGHT) {
                //     whiteHasMajorPiece = true;
                // }
            } else {
                blackPieces++;
                // WAITING FOR ALI TO ADD getType() TO piece.h
                // if (p->getType() != BISHOP && p->getType() != KNIGHT) {
                //     blackHasMajorPiece = true;
                // }
            }
        }
    }

    // king vs king only
    if (whitePieces == 0 && blackPieces == 0) { 
        return true; 
    }

    // king and one minor piece vs lone king
    if (whitePieces <= 1 && blackPieces == 0 && !whiteHasMajorPiece) { 
        return true; 
    }
    if (blackPieces <= 1 && whitePieces == 0 && !blackHasMajorPiece) { 
        return true; 
    }

    return false;
}