#include "tester.hpp"

#include <array>

namespace {

constexpr std::array<std::uint64_t, 8> kStartingPositionPerft = {
    1ULL,
    20ULL,
    400ULL,
    8902ULL,
    197281ULL,
    4865609ULL,
    119060324ULL,
    3195901860ULL
};

inline int row_of(int square) { return square / 8; }
inline int col_of(int square) { return square % 8; }

} // namespace

Tester::Tester() {
    rootPosition.board = Board();
    rootPosition.sideToMove = PieceColor::White;
    rootPosition.castlingRights = MoveGenerator::CASTLE_WHITE_KING
        | MoveGenerator::CASTLE_WHITE_QUEEN
        | MoveGenerator::CASTLE_BLACK_KING
        | MoveGenerator::CASTLE_BLACK_QUEEN;
    rootPosition.enPassantSquare = -1;
}

std::vector<Move> Tester::generateMoves(const Position& position) {
    return movegen.generateMoves(
        position.board,
        position.sideToMove,
        position.castlingRights,
        position.enPassantSquare
    );
}

void Tester::updateCastlingRights(Position& position, int fromSquare, int toSquare, Piece moving, Piece captured) {
    auto clear = [&](int mask) { position.castlingRights &= ~mask; };

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

void Tester::applyMove(Position& position, const Move& move) {
    int fromRow = row_of(move.from);
    int fromCol = col_of(move.from);
    int toRow = row_of(move.to);
    int toCol = col_of(move.to);

    Piece moving = position.board.get_piece(fromRow, fromCol);
    Piece captured = position.board.get_piece(toRow, toCol);

    updateCastlingRights(position, move.from, move.to, moving, captured);

    int nextEnPassantSquare = -1;
    if (getType(moving) == PieceType::P && (fromRow - toRow == 2 || toRow - fromRow == 2)) {
        nextEnPassantSquare = ((fromRow + toRow) / 2) * 8 + fromCol;
    }

    if (move.isCastling) {
        position.board.move_piece(fromRow, fromCol, toRow, toCol);
        if (position.sideToMove == PieceColor::White) {
            if (move.to == 7 * 8 + 6) {
                position.board.move_piece(7, 7, 7, 5);
            } else {
                position.board.move_piece(7, 0, 7, 3);
            }
        } else {
            if (move.to == 0 * 8 + 6) {
                position.board.move_piece(0, 7, 0, 5);
            } else {
                position.board.move_piece(0, 0, 0, 3);
            }
        }
    } else if (move.isEnPassant) {
        position.board.remove_piece(fromRow, toCol);
        position.board.move_piece(fromRow, fromCol, toRow, toCol);
    } else {
        Piece promotion = move.isPromotion ? move.promotionPiece : Piece::None;
        position.board.move_piece(fromRow, fromCol, toRow, toCol, promotion);
    }

    position.enPassantSquare = nextEnPassantSquare;
    position.sideToMove = (position.sideToMove == PieceColor::White) ? PieceColor::Black : PieceColor::White;
}

std::uint64_t Tester::perft(const Position& position, int depth) {
    if (depth < 0) {
        return 0;
    }
    if (depth == 0) {
        return 1;
    }

    std::vector<Move> moves = generateMoves(position);
    if (depth == 1) {
        return moves.size();
    }

    std::uint64_t total = 0;
    for (const Move& move : moves) {
        Position next = position;
        applyMove(next, move);
        total += perft(next, depth - 1);
    }

    return total;
}

std::string Tester::formatMove(const Move& move) {
    auto appendSquare = [](std::string& out, int square) {
        out.push_back(static_cast<char>('a' + col_of(square)));
        out.push_back(static_cast<char>('8' - row_of(square)));
    };

    std::string text;
    text.reserve(5);
    appendSquare(text, move.from);
    appendSquare(text, move.to);

    if (move.isPromotion) {
        char promo = 'q';
        switch (getType(move.promotionPiece)) {
            case PieceType::R: { promo = 'r'; break; }
            case PieceType::B: { promo = 'b'; break; }
            case PieceType::N: { promo = 'n'; break; }
            default: { promo = 'q'; break; }
        }
        text.push_back(promo);
    }

    return text;
}

std::uint64_t Tester::countGames(int depth) {
    return perft(rootPosition, depth);
}

std::vector<std::pair<std::string, std::uint64_t>> Tester::divide(int depth) {
    std::vector<std::pair<std::string, std::uint64_t>> breakdown;
    if (depth <= 0) {
        return breakdown;
    }

    std::vector<Move> moves = generateMoves(rootPosition);
    breakdown.reserve(moves.size());

    for (const Move& move : moves) {
        Position next = rootPosition;
        applyMove(next, move);
        breakdown.push_back({formatMove(move), perft(next, depth - 1)});
    }

    return breakdown;
}

std::uint64_t Tester::expectedCount(int depth) const {
    if (depth < 0 || depth >= static_cast<int>(kStartingPositionPerft.size())) {
        return 0;
    }

    return kStartingPositionPerft[depth];
}

bool Tester::verifyDepth(int depth) {
    std::uint64_t expected = expectedCount(depth);
    if (expected == 0 && depth != 0) {
        return false;
    }

    return countGames(depth) == expected;
}
