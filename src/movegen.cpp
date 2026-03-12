#include "movegen.hpp"

#include <array>

using Bitboard = MoveGenerator::Bitboard;

namespace {

inline Bitboard bit_at(int sq) { return Bitboard(1) << sq; }
inline int row_of(int sq) { return sq / 8; }
inline int col_of(int sq) { return sq % 8; }

void add_if_onboard(int r, int c, Bitboard &b) {
    if (r >= 0) {
        if (r < 8) {
            if (c >= 0) {
                if (c < 8) {
                    Bitboard m = bit_at(r * 8 + c);
                    b = b + m - (b & m);
                }
            }
        }
    }
}

void init_attack_tables(Bitboard knight_attacks[64], Bitboard king_attacks[64]) {
    for (int sq = 0; sq < 64; ++sq) {
        int r = row_of(sq);
        int c = col_of(sq);
        Bitboard knt = 0;
        Bitboard kng = 0;

        static const int kdr[8] = {-2,-2,-1,-1, 1, 1, 2, 2};
        static const int kdc[8] = {-1, 1,-2, 2,-2, 2,-1, 1};
        for (int i = 0; i < 8; ++i) {
            add_if_onboard(r + kdr[i], c + kdc[i], knt);
        }

        for (int dr = -1; dr <= 1; ++dr) {
            for (int dc = -1; dc <= 1; ++dc) {
                if (dr == 0) { if (dc == 0) { continue; } }
                add_if_onboard(r + dr, c + dc, kng);
            }
        }

        knight_attacks[sq] = knt;
        king_attacks[sq] = kng;
    }
}

} // namespace

Bitboard MoveGenerator::knight_attacks[64];
Bitboard MoveGenerator::king_attacks[64];

Bitboard MoveGenerator::generateRookMoves(int square, Bitboard occupied) {
    Bitboard moves = 0;
    int r = row_of(square);
    int c = col_of(square);

    for (int rr = r - 1; rr >= 0; --rr) {
        int s = rr * 8 + c;
        moves = moves + bit_at(s) - (moves & bit_at(s));
        if (occupied & bit_at(s)) { break; }
    }

    for (int rr = r + 1; rr < 8; ++rr) {
        int s = rr * 8 + c;
        moves = moves + bit_at(s) - (moves & bit_at(s));
        if (occupied & bit_at(s)) { break; }
    }

    for (int cc = c - 1; cc >= 0; --cc) {
        int s = r * 8 + cc;
        moves = moves + bit_at(s) - (moves & bit_at(s));
        if (occupied & bit_at(s)) { break; }
    }

    for (int cc = c + 1; cc < 8; ++cc) {
        int s = r * 8 + cc;
        moves = moves + bit_at(s) - (moves & bit_at(s));
        if (occupied & bit_at(s)) { break; }
    }

    return moves;
}

Bitboard MoveGenerator::generateBishopMoves(int square, Bitboard occupied) {
    Bitboard moves = 0;
    int r = row_of(square);
    int c = col_of(square);

    { int rr = r - 1; int cc = c + 1;
      while (true) {
        if (!(rr >= 0)) break;
        if (!(cc < 8)) break;
        int s = rr * 8 + cc;
        moves = moves + bit_at(s) - (moves & bit_at(s));
        if (occupied & bit_at(s)) { break; }
        rr -= 1; cc += 1;
      }
    }

    { int rr = r - 1; int cc = c - 1;
      while (true) {
        if (!(rr >= 0)) break;
        if (!(cc >= 0)) break;
        int s = rr * 8 + cc;
        moves = moves + bit_at(s) - (moves & bit_at(s));
        if (occupied & bit_at(s)) { break; }
        rr -= 1; cc -= 1;
      }
    }

    { int rr = r + 1; int cc = c + 1;
      while (true) {
        if (!(rr < 8)) break;
        if (!(cc < 8)) break;
        int s = rr * 8 + cc;
        moves = moves + bit_at(s) - (moves & bit_at(s));
        if (occupied & bit_at(s)) { break; }
        rr += 1; cc += 1;
      }
    }

    { int rr = r + 1; int cc = c - 1;
      while (true) {
        if (!(rr < 8)) break;
        if (!(cc >= 0)) break;
        int s = rr * 8 + cc;
        moves = moves + bit_at(s) - (moves & bit_at(s));
        if (occupied & bit_at(s)) { break; }
        rr += 1; cc -= 1;
      }
    }

    return moves;
}

Bitboard MoveGenerator::generatePawnAttacks(int square, PieceColor color) {
    Bitboard attacks = 0;
    int r = row_of(square);
    int c = col_of(square);

    if (color == PieceColor::White) {
        add_if_onboard(r - 1, c - 1, attacks);
        add_if_onboard(r - 1, c + 1, attacks);
    } else {
        add_if_onboard(r + 1, c - 1, attacks);
        add_if_onboard(r + 1, c + 1, attacks);
    }

    return attacks;
}

Bitboard MoveGenerator::generatePawnPushes(int square, PieceColor color, Bitboard occupied) {
    Bitboard pushes = 0;
    int r = row_of(square);
    int c = col_of(square);
    int dir = (color == PieceColor::White) ? -1 : 1;

    int r1 = r + dir;
    if (r1 >= 0) {
        if (r1 < 8) {
            int s1 = r1 * 8 + c;
            if ((occupied & bit_at(s1)) == 0) {
                pushes = pushes + bit_at(s1) - (pushes & bit_at(s1));
                int start_rank = (color == PieceColor::White) ? 6 : 1;
                if (r == start_rank) {
                    int r2 = r + 2 * dir;
                    if (r2 >= 0) {
                        if (r2 < 8) {
                            int s2 = r2 * 8 + c;
                            if ((occupied & bit_at(s2)) == 0) {
                                pushes = pushes + bit_at(s2) - (pushes & bit_at(s2));
                            }
                        }
                    }
                }
            }
        }
    }

    return pushes;
}

Bitboard MoveGenerator::generateAttacks(const Board& board, PieceColor color) {
    Bitboard occ_all = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece p = board.get_piece(r, c);
            if (!isNone(p)) {
                occ_all = occ_all + bit_at(r * 8 + c) - (occ_all & bit_at(r * 8 + c));
            }
        }
    }

    Bitboard attacks = 0;
    for (int sq = 0; sq < 64; ++sq) {
        int r = row_of(sq);
        int c = col_of(sq);
        Piece p = board.get_piece(r, c);
        if (isNone(p)) { continue; }
        if (getColor(p) != color) { continue; }

        PieceType t = getType(p);
        switch (t) {
            case PieceType::P: {
                attacks |= generatePawnAttacks(sq, color);
                break;
            }
            case PieceType::N: {
                attacks |= knight_attacks[sq];
                break;
            }
            case PieceType::B: {
                attacks |= generateBishopMoves(sq, occ_all);
                break;
            }
            case PieceType::R: {
                attacks |= generateRookMoves(sq, occ_all);
                break;
            }
            case PieceType::Q: {
                Bitboard bmask = generateBishopMoves(sq, occ_all);
                Bitboard rmask = generateRookMoves(sq, occ_all);
                attacks |= bmask + rmask - (bmask & rmask);
                break;
            }
            case PieceType::K: {
                attacks |= king_attacks[sq];
                break;
            }
            default: { break; }
        }
    }

    return attacks;
}

std::vector<Move> MoveGenerator::generateMoves(const Board &board, PieceColor color, int castlingRights) {
    static bool tables_initialized = false;
    if (!tables_initialized) {
        init_attack_tables(knight_attacks, king_attacks);
        tables_initialized = true;
    }

    Bitboard occ_white = 0;
    Bitboard occ_black = 0;
    Bitboard occ_all = 0;
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            Piece p = board.get_piece(r, c);
            if (!isNone(p)) {
                Bitboard m = bit_at(r * 8 + c);
                if (getColor(p) == PieceColor::White) {
                    occ_white = occ_white + m - (occ_white & m);
                } else {
                    occ_black = occ_black + m - (occ_black & m);
                }
            }
        }
    }
    occ_all = occ_white + occ_black - (occ_white & occ_black);
    Bitboard occ_own = (color == PieceColor::White) ? occ_white : occ_black;
    Bitboard occ_opp = (color == PieceColor::White) ? occ_black : occ_white;

    std::vector<Move> moves;
    moves.reserve(256);

    auto push_move = [&](int from, int to, bool isCapture, bool isPromotion, Piece promo) {
        Move m{};
        m.from = from;
        m.to = to;
        m.isCheck = false;
        m.isCapture = isCapture;
        m.isAttack = isCapture;
        m.isCastling = false;
        m.isEnPassant = false;
        m.isPromotion = isPromotion;
        m.promotionPiece = promo;
        moves.push_back(m);
    };

    for (int sq = 0; sq < 64; ++sq) {
        int r = row_of(sq);
        int c = col_of(sq);
        Piece p = board.get_piece(r, c);
        if (isNone(p)) { continue; }
        if (getColor(p) != color) { continue; }

        PieceType t = getType(p);
        switch (t) {
            case PieceType::P: {
                Bitboard tmp = generatePawnAttacks(sq, color);
                Bitboard captures = tmp & occ_opp;
                Bitboard b = captures;
                while (b) {
                    Bitboard lsb = isolate_lsb(b);
                    int to = bit_position(lsb);
                    int toRow = row_of(to);
                    bool promo = (color == PieceColor::White) ? (toRow == 0) : (toRow == 7);
                    if (promo) {
                        push_move(sq, to, true, true, makePiece(color, PieceType::Q));
                        push_move(sq, to, true, true, makePiece(color, PieceType::R));
                        push_move(sq, to, true, true, makePiece(color, PieceType::B));
                        push_move(sq, to, true, true, makePiece(color, PieceType::N));
                    } else {
                        push_move(sq, to, true, false, Piece::None);
                    }
                    b ^= lsb;
                }

                Bitboard pushes = generatePawnPushes(sq, color, occ_all);
                b = pushes;
                while (b) {
                    Bitboard lsb = isolate_lsb(b);
                    int to = bit_position(lsb);
                    int toRow = row_of(to);
                    bool promo = (color == PieceColor::White) ? (toRow == 0) : (toRow == 7);
                    if (promo) {
                        push_move(sq, to, false, true, makePiece(color, PieceType::Q));
                        push_move(sq, to, false, true, makePiece(color, PieceType::R));
                        push_move(sq, to, false, true, makePiece(color, PieceType::B));
                        push_move(sq, to, false, true, makePiece(color, PieceType::N));
                    } else {
                        push_move(sq, to, false, false, Piece::None);
                    }
                    b ^= lsb;
                }
                break;
            }
            case PieceType::N: {
                Bitboard mask = knight_attacks[sq] & (~occ_own);
                Bitboard b = mask;
                while (b) {
                    Bitboard lsb = isolate_lsb(b);
                    int to = bit_position(lsb);
                    bool cap = (occ_opp & bit_at(to)) != 0;
                    push_move(sq, to, cap, false, Piece::None);
                    b ^= lsb;
                }
                break;
            }
            case PieceType::B: {
                Bitboard mask = generateBishopMoves(sq, occ_all) & (~occ_own);
                Bitboard b = mask;
                while (b) {
                    Bitboard lsb = isolate_lsb(b);
                    int to = bit_position(lsb);
                    bool cap = (occ_opp & bit_at(to)) != 0;
                    push_move(sq, to, cap, false, Piece::None);
                    b ^= lsb;
                }
                break;
            }
            case PieceType::R: {
                Bitboard mask = generateRookMoves(sq, occ_all) & (~occ_own);
                Bitboard b = mask;
                while (b) {
                    Bitboard lsb = isolate_lsb(b);
                    int to = bit_position(lsb);
                    bool cap = (occ_opp & bit_at(to)) != 0;
                    push_move(sq, to, cap, false, Piece::None);
                    b ^= lsb;
                }
                break;
            }
            case PieceType::Q: {
                Bitboard bmask = generateBishopMoves(sq, occ_all);
                Bitboard rmask = generateRookMoves(sq, occ_all);
                Bitboard mask = bmask + rmask - (bmask & rmask);
                mask = mask & (~occ_own);
                Bitboard b = mask;
                while (b) {
                    Bitboard lsb = isolate_lsb(b);
                    int to = bit_position(lsb);
                    bool cap = (occ_opp & bit_at(to)) != 0;
                    push_move(sq, to, cap, false, Piece::None);
                    b ^= lsb;
                }
                break;
            }
            case PieceType::K: {
                Bitboard mask = king_attacks[sq] & (~occ_own);
                Bitboard b = mask;
                while (b) {
                    Bitboard lsb = isolate_lsb(b);
                    int to = bit_position(lsb);
                    bool cap = (occ_opp & bit_at(to)) != 0;
                    push_move(sq, to, cap, false, Piece::None);
                    b ^= lsb;
                }
                break;
            }
            default: { break; }
        }
    }

    Bitboard opp_attacks = generateAttacks(board, (color == PieceColor::White) ? PieceColor::Black : PieceColor::White);

    auto add_castle = [&](int from, int to) {
        Move m{};
        m.from = from;
        m.to = to;
        m.isCheck = false;
        m.isCapture = false;
        m.isAttack = false;
        m.isCastling = true;
        m.isEnPassant = false;
        m.isPromotion = false;
        m.promotionPiece = Piece::None;
        moves.push_back(m);
    };

    if (color == PieceColor::White) {
        int kingSq = 7 * 8 + 4;
        if (board.get_piece(7, 4) == Piece::WK) {
            if (castlingRights & CASTLE_WHITE_KING) {
                bool empty = isNone(board.get_piece(7, 5)) && isNone(board.get_piece(7, 6));
                bool rookOk = board.get_piece(7, 7) == Piece::WR;
                bool safe = (opp_attacks & (bit_at(kingSq) | bit_at(7 * 8 + 5) | bit_at(7 * 8 + 6))) == 0;
                if (empty && rookOk && safe) {
                    add_castle(kingSq, 7 * 8 + 6);
                }
            }
            if (castlingRights & CASTLE_WHITE_QUEEN) {
                bool empty = isNone(board.get_piece(7, 1)) && isNone(board.get_piece(7, 2)) && isNone(board.get_piece(7, 3));
                bool rookOk = board.get_piece(7, 0) == Piece::WR;
                bool safe = (opp_attacks & (bit_at(kingSq) | bit_at(7 * 8 + 3) | bit_at(7 * 8 + 2))) == 0;
                if (empty && rookOk && safe) {
                    add_castle(kingSq, 7 * 8 + 2);
                }
            }
        }
    } else {
        int kingSq = 0 * 8 + 4;
        if (board.get_piece(0, 4) == Piece::BK) {
            if (castlingRights & CASTLE_BLACK_KING) {
                bool empty = isNone(board.get_piece(0, 5)) && isNone(board.get_piece(0, 6));
                bool rookOk = board.get_piece(0, 7) == Piece::BR;
                bool safe = (opp_attacks & (bit_at(kingSq) | bit_at(0 * 8 + 5) | bit_at(0 * 8 + 6))) == 0;
                if (empty && rookOk && safe) {
                    add_castle(kingSq, 0 * 8 + 6);
                }
            }
            if (castlingRights & CASTLE_BLACK_QUEEN) {
                bool empty = isNone(board.get_piece(0, 1)) && isNone(board.get_piece(0, 2)) && isNone(board.get_piece(0, 3));
                bool rookOk = board.get_piece(0, 0) == Piece::BR;
                bool safe = (opp_attacks & (bit_at(kingSq) | bit_at(0 * 8 + 3) | bit_at(0 * 8 + 2))) == 0;
                if (empty && rookOk && safe) {
                    add_castle(kingSq, 0 * 8 + 2);
                }
            }
        }
    }

    return moves;
}
