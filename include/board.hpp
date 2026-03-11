#pragma once

#include <piece.hpp>

class Board {
    private:
        Piece pieces[8][8] = {
            {BR, BN, BB, BQ, BK, BB, BN, BR},
            {BP, BP, BP, BP, BP, BP, BP, BP},
            {NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE},
            {NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE},
            {NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE},
            {NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE, NPIECE},
            {WP, WP, WP, WP, WP, WP, WP, WP},
            {WR, WN, WB, WQ, WK, WB, WN, WR}
        };

    public:
        Board();

        Piece get_piece(int row, int col);
};
