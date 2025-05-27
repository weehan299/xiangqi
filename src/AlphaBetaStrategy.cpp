
// AlphaBetaStrategy.cpp
#include "AlphaBetaStrategy.h"
#include <limits>
#include <algorithm>
#include "Board.h"
#include "Piece.h"
#include <iostream>

namespace {
    // Piece value heuristic
    int pieceValue(PieceType type) {
        switch (type) {
            case PieceType::General:   return 100000;
            case PieceType::Advisor:   return 20;
            case PieceType::Elephant:  return 20;
            case PieceType::Horse:     return 40;
            case PieceType::Chariot:   return 90;
            case PieceType::Cannon:    return 45;
            case PieceType::Soldier:   return 10;
            default:                   return 0;
        }
    }

    // Full-board evaluation (positive = good for aiSide)
    int evaluateBoard(const Board& board, Side aiSide) {
        int score = 0;
        for (int x = 0; x < COLUMNS; ++x) {
            for (int y = 0; y < ROWS; ++y) {
                if (const Piece* p = board.pieceAt(x, y)) {
                    int v = pieceValue(p->getType());
                    score += (p->getSide() == aiSide ? +v : -v);
                }
            }
        }
        return score;
    }
}

AlphaBetaStrategy::AlphaBetaStrategy(int depth)
    : maxDepth(depth) {}

int AlphaBetaStrategy::alphabeta(Board& board,
                                 int depth,
                                 int alpha,
                                 int beta,
                                 Side aiSide,
                                 bool maximizing) const
{
    if (depth == 0)
        return evaluateBoard(board, aiSide);

    Side turn = board.getCurrentTurn();
    auto moves = board.getLegalMoves(turn);
    if (moves.empty())
        return evaluateBoard(board, aiSide);

    if (maximizing) {
        int value = std::numeric_limits<int>::min();
        for (auto& m : moves) {
            board.movePiece(m.first.x, m.first.y, m.second.x, m.second.y);
            value = std::max(value,
                             alphabeta(board, depth - 1, alpha, beta, aiSide, false));
            board.undoMove(); // Undo the move for the next iteration
            alpha = std::max(alpha, value);
            if (alpha >= beta) {
                // β-cutoff
                break;
            }
        }
        return value;
    } else {
        int value = std::numeric_limits<int>::max();
        for (auto& m : moves) {
            board.movePiece(m.first.x, m.first.y, m.second.x, m.second.y);
            value = std::min(value,
                             alphabeta(board, depth - 1, alpha, beta, aiSide, true));
            beta = std::min(beta, value);
            board.undoMove(); // Undo the move for the next iteration   
            if (beta <= alpha) {
                // α-cutoff
                break;
            }
        }
        return value;
    }
}

std::pair<sf::Vector2i, sf::Vector2i>
AlphaBetaStrategy::selectMove(const Board& board, Side aiSide)
{
    auto moves = board.getLegalMoves(aiSide);
    if (moves.empty())
        return {{-1, -1}, {-1, -1}};

    // Copy once and work on the copy instead of the original
    // major way to speed up the algorithm where you are not copying the board each time
    Board working_board = board;

    int bestValue = std::numeric_limits<int>::min();
    std::pair<sf::Vector2i, sf::Vector2i> bestMove = moves.front();

    int alpha = std::numeric_limits<int>::min();
    int beta  = std::numeric_limits<int>::max();

    for (auto& m : moves) {
        std::cout << "Evaluating move: " << m.first.x << "," << m.first.y
                  << " to " << m.second.x << "," << m.second.y << std::endl;
        working_board.movePiece(m.first.x, m.first.y, m.second.x, m.second.y);
        int val = alphabeta(working_board,
                            maxDepth - 1,
                            alpha,
                            beta,
                            aiSide,
                            false);
        working_board.undoMove(); // Undo the move for the next iteration
        if (val > bestValue) {
            bestValue = val;
            bestMove = m;
        }
        alpha = std::max(alpha, bestValue);
    }

    return bestMove;
}
