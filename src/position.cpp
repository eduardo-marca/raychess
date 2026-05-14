#include "position.hpp"

namespace {

inline int row_of(int square) { return square / 8; }
inline int col_of(int square) { return square % 8; }

} // namespace

Position::Position() {
    board = Board();
    sideToMove = PieceColor::White;
    castlingRights = MoveGenerator::CASTLE_WHITE_KING
        | MoveGenerator::CASTLE_WHITE_QUEEN
        | MoveGenerator::CASTLE_BLACK_KING
        | MoveGenerator::CASTLE_BLACK_QUEEN;
    enPassantSquare = -1;
}

std::vector<Move> Position::generateLegalMoves(MoveGenerator& movegen) const {
    return movegen.generateMoves(board, sideToMove, castlingRights, enPassantSquare);
}

bool Position::applyMove(const Move& move) {
    int fromRow = row_of(move.from);
    int fromCol = col_of(move.from);
    int toRow = row_of(move.to);
    int toCol = col_of(move.to);

    Piece moving = board.get_piece(fromRow, fromCol);
    if (isNone(moving) || getColor(moving) != sideToMove) {
        return false;
    }

    Piece captured = board.get_piece(toRow, toCol);
    updateCastlingRights(move.from, move.to, moving, captured);

    int nextEnPassantSquare = -1;
    if (getType(moving) == PieceType::P && (fromRow - toRow == 2 || toRow - fromRow == 2)) {
        nextEnPassantSquare = ((fromRow + toRow) / 2) * 8 + fromCol;
    }

    if (move.isCastling) {
        board.move_piece(fromRow, fromCol, toRow, toCol);
        if (sideToMove == PieceColor::White) {
            if (move.to == 7 * 8 + 6) {
                board.move_piece(7, 7, 7, 5);
            } else {
                board.move_piece(7, 0, 7, 3);
            }
        } else {
            if (move.to == 0 * 8 + 6) {
                board.move_piece(0, 7, 0, 5);
            } else {
                board.move_piece(0, 0, 0, 3);
            }
        }
    } else if (move.isEnPassant) {
        board.remove_piece(fromRow, toCol);
        board.move_piece(fromRow, fromCol, toRow, toCol);
    } else {
        Piece promotion = move.isPromotion ? move.promotionPiece : Piece::None;
        board.move_piece(fromRow, fromCol, toRow, toCol, promotion);
    }

    enPassantSquare = nextEnPassantSquare;
    sideToMove = (sideToMove == PieceColor::White) ? PieceColor::Black : PieceColor::White;
    return true;
}

void Position::updateCastlingRights(int fromSquare, int toSquare, Piece moving, Piece captured) {
    auto clear = [&](int mask) { castlingRights &= ~mask; };

    if (getType(moving) == PieceType::K) {
        if (getColor(moving) == PieceColor::White) {
            clear(MoveGenerator::CASTLE_WHITE_KING | MoveGenerator::CASTLE_WHITE_QUEEN);
        } else {
            clear(MoveGenerator::CASTLE_BLACK_KING | MoveGenerator::CASTLE_BLACK_QUEEN);
        }
    }

    if (getType(moving) == PieceType::R) {
        if (getColor(moving) == PieceColor::White) {
            if (fromSquare == 7 * 8 + 7) { clear(MoveGenerator::CASTLE_WHITE_KING); }
            if (fromSquare == 7 * 8 + 0) { clear(MoveGenerator::CASTLE_WHITE_QUEEN); }
        } else {
            if (fromSquare == 0 * 8 + 7) { clear(MoveGenerator::CASTLE_BLACK_KING); }
            if (fromSquare == 0 * 8 + 0) { clear(MoveGenerator::CASTLE_BLACK_QUEEN); }
        }
    }

    if (getType(captured) == PieceType::R) {
        if (getColor(captured) == PieceColor::White) {
            if (toSquare == 7 * 8 + 7) { clear(MoveGenerator::CASTLE_WHITE_KING); }
            if (toSquare == 7 * 8 + 0) { clear(MoveGenerator::CASTLE_WHITE_QUEEN); }
        } else if (getColor(captured) == PieceColor::Black) {
            if (toSquare == 0 * 8 + 7) { clear(MoveGenerator::CASTLE_BLACK_KING); }
            if (toSquare == 0 * 8 + 0) { clear(MoveGenerator::CASTLE_BLACK_QUEEN); }
        }
    }
}
