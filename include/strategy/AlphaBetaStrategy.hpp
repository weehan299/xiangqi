// AlphaBetaStrategy.h
#pragma once
#include "AIEngine.hpp"

// A minimax-based chooser with alpha–beta pruning
class AlphaBetaStrategy : public MoveStrategy {
public:
    explicit AlphaBetaStrategy(int depth);
    std::pair<sf::Vector2i, sf::Vector2i>
    selectMove(const Board& board, Side aiSide) override;

private:
    int maxDepth;

    // Core α–β recursion
    int alphabeta(Board& board,
                  int depth,
                  int alpha,
                  int beta,
                  Side aiSide,
                  bool maximizing) const;
};
