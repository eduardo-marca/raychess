#include "board.hpp"

Board::Board() {
    
}

Piece Board::get_piece(int row, int col) {
    return pieces[row][col];
}
