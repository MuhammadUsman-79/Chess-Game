#include "AllHeaderFiles.h"
#include "ChessGUI.h"

#include<iostream>

int main() {
    Board board;
    board.initializeBoard(); // Put pieces on their starting squares

    ChessGUI gui(&board);    // Link the GUI to the board
    gui.run();              // Start SFML Window loop

    return 0;
}

//without GUI

// int main(){

//     Board board;
//     board.startGame();

//     return 0;
// }