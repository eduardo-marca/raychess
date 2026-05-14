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
}

std::uint64_t Tester::perft(const Position& position, int depth) {
    if (depth < 0) {
        return 0;
    }
    if (depth == 0) {
        return 1;
    }

    std::vector<Move> moves = position.generateLegalMoves(movegen);
    if (depth == 1) {
        return moves.size();
    }

    std::uint64_t total = 0;
    for (const Move& move : moves) {
        Position next = position;
        next.applyMove(move);
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

    std::vector<Move> moves = rootPosition.generateLegalMoves(movegen);
    breakdown.reserve(moves.size());

    for (const Move& move : moves) {
        Position next = rootPosition;
        next.applyMove(move);
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
