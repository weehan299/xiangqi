
// ---------- strategy/AlphaBetaStrategy.cpp ----------
#include "strategy/AlphaBetaStrategy.hpp"
#include <limits>
#include <algorithm>
#include "Board.hpp"
#include "Piece.hpp"
#include <iostream>

namespace {
    // Piece value heuristic
    int pieceValue(PieceType type) {
        switch (type) {
            case PieceType::General:   return 10000;
            case PieceType::Advisor:   return   20;
            case PieceType::Elephant:  return   20;
            case PieceType::Horse:     return   40;
            case PieceType::Chariot:   return   90;
            case PieceType::Cannon:    return   45;
            case PieceType::Soldier:   return   10;
            default:                   return    0;
        }
    }

    // Simple material‐count evaluation.  Positive = good for aiSide.
    int evaluateBoard(const Board& board, Side aiSide) {
        int score = 0;
        for (int x = 0; x < COLUMNS; ++x) {
            for (int y = 0; y < ROWS; ++y) {
                if (const Piece* p = board.pieceAt(x, y)) {
                    int v = pieceValue(p->getType());
                    if (p->getSide() == aiSide)      score +=  v;
                    else                              score -=  v;
                }
            }
        }
        return score;
    }
}

AlphaBetaStrategy::AlphaBetaStrategy(int depth)
    : maxDepth(depth)
{ }

// ---------------------------------------------------------------
// The core alphabeta recursion.  We now treat checkmate as ±∞.
// ---------------------------------------------------------------
int AlphaBetaStrategy::alphabeta(Board& board,
                                 int depth,
                                 int alpha,
                                 int beta,
                                 Side aiSide,
                                 bool maximizing) const
{
    Side turn = board.getCurrentTurn();
    auto moves = board.getLegalMoves(turn);


    // If side‐to‐move has no legal moves, Xiangqi rules say that's a loss
    //    (both checkmate and stalemate count as a loss for 'turn').
    if (moves.empty())
    {
        if (turn == aiSide) {
            // AI to move but no moves ⇒ AI loses
            return std::numeric_limits<int>::min() + 1;
        } else {
            // Opponent to move but no moves ⇒ Opponent loses ⇒ AI wins
            return std::numeric_limits<int>::max() - 1;
        }
    }

    // At depth 0 we simply return the material‐score (no special checkmate logic here).
    if (depth == 0) {
        return evaluateBoard(board, aiSide);
    }

    if (maximizing) {
        int best = std::numeric_limits<int>::min();
        for (auto& m : moves) {
            board.movePiece(m.first.x, m.first.y, m.second.x, m.second.y);
            int val = alphabeta(board,
                                depth - 1,
                                alpha,
                                beta,
                                aiSide,
                                false);
            board.undoMove();
            best = std::max(best, val);
            alpha = std::max(alpha, best);
            if (alpha >= beta) {
                // β‐cutoff
                break;
            }
        }
        return best;
    }
    else {
        int best = std::numeric_limits<int>::max();
        for (auto& m : moves) {
            board.movePiece(m.first.x, m.first.y, m.second.x, m.second.y);
            int val = alphabeta(board,
                                depth - 1,
                                alpha,
                                beta,
                                aiSide,
                                true);
            board.undoMove();
            best = std::min(best, val);
            beta = std::min(beta, best);
            if (beta <= alpha) {
                // α‐cutoff
                break;
            }
        }
        return best;
    }
}

// ---------------------------------------------------------------
// Top‐level: pick the single best move for aiSide.  We already
// treat checkmate as ±∞ inside alphabeta, so capturing the general
// becomes the highest‐possible score.  
// ---------------------------------------------------------------
std::pair<sf::Vector2i, sf::Vector2i>
AlphaBetaStrategy::selectMove(const Board& board, Side aiSide)
{
    auto moves = board.getLegalMoves(aiSide);
    if (moves.empty())
        return {{-1, -1}, {-1, -1}};

    // Copy once, so we don’t mutate the real board each trial:
    Board working = board;

    int bestValue = std::numeric_limits<int>::min();
    std::pair<sf::Vector2i, sf::Vector2i> bestMove = moves.front();

    int alpha = std::numeric_limits<int>::min();
    int beta  = std::numeric_limits<int>::max();

    // Try each candidate move
    for (auto& m : moves) {
        std::cout << "Evaluating move: "
                  << m.first.x << "," << m.first.y
                  << " → " 
                  << m.second.x << "," << m.second.y
                  << std::endl;

        working.movePiece(m.first.x, m.first.y, m.second.x, m.second.y);
        int val = alphabeta(working,
                            maxDepth - 1,
                            alpha,
                            beta,
                            aiSide,
                            false);
        working.undoMove();

        if (val > bestValue) {
            bestValue = val;
            bestMove = m;
        }
        alpha = std::max(alpha, bestValue);
    }

    return bestMove;
}
