
// MinimaxStrategy.cpp
#include "MinimaxStrategy.h"
#include <limits>
#include <algorithm>
#include "Board.h"
#include "Piece.h"
#include <iostream>

namespace {
    // Piece value heuristic
    int pieceValue(PieceType type) {
        switch (type) {
            case PieceType::General:   return 10000;
            case PieceType::Advisor:   return 20;
            case PieceType::Elephant:  return 20;
            case PieceType::Horse:     return 40;
            case PieceType::Chariot:   return 90;
            case PieceType::Cannon:    return 45;
            case PieceType::Soldier:   return 10;
            default:                   return 0;
        }
    }

    // Evaluate board for aiSide: positive = good for AI, negative = good for opponent
    int evaluateBoard(const Board& board, Side aiSide) {
        int score = 0;
        for (int x = 0; x < COLUMNS; ++x) {
            for (int y = 0; y < ROWS; ++y) {
                const Piece* p = board.pieceAt(x, y);
                if (!p) continue;
                int val = pieceValue(p->getType());
                score += (p->getSide() == aiSide ? val : -val);
            }
        }
        return score;
    }

    // Recursive minimax search
    int minimax(const Board& board, int depth, Side aiSide, bool maximizing) {
        if (depth == 0)
            return evaluateBoard(board, aiSide);

        Side turn = board.getCurrentTurn();
        auto moves = board.getLegalMoves(turn);
        if (moves.empty())
            return evaluateBoard(board, aiSide);

        if (maximizing) {
            int maxEval = std::numeric_limits<int>::min(); // Start with the worst possible value
            for (auto& m : moves) {
                Board child = board;  // assumes Board is copyable
                child.movePiece(m.first.x, m.first.y, m.second.x, m.second.y);
                int eval = minimax(child, depth - 1, aiSide, false);
                maxEval = std::max(maxEval, eval);
            }
            return maxEval;
        } else {
            int minEval = std::numeric_limits<int>::max(); // Start with the best possible value
            for (auto& m : moves) {
                Board child = board;
                child.movePiece(m.first.x, m.first.y, m.second.x, m.second.y);
                int eval = minimax(child, depth - 1, aiSide, true);
                minEval = std::min(minEval, eval);
            }
            return minEval;
        }
    }
} // anonymous namespace

MinimaxStrategy::MinimaxStrategy(int depth)
    : maxDepth(depth) {}

std::pair<sf::Vector2i, sf::Vector2i>
MinimaxStrategy::selectMove(const Board& board, Side aiSide) {
    auto moves = board.getLegalMoves(aiSide);
    if (moves.empty())
        return {{-1, -1}, {-1, -1}};

    int bestValue = std::numeric_limits<int>::min();
    std::pair<sf::Vector2i, sf::Vector2i> bestMove = moves[0];
    for (auto& m : moves) {
        std::cout << "Evaluating move from (" << m.first.x << ", " << m.first.y
                  << ") to (" << m.second.x << ", " << m.second.y << ")\n";
        Board child = board;
        child.movePiece(m.first.x, m.first.y, m.second.x, m.second.y);
        int eval = minimax(child, maxDepth - 1, aiSide, false);
        if (eval > bestValue) {
            bestValue = eval;
            bestMove = m;
        }
    }
    return bestMove;
}
