// MinimaxStrategy.h
#pragma once
#include "AIEngine.hpp"

// A placeholder for a minimax-based chooser
class MinimaxStrategy : public MoveStrategy {
public:
    explicit MinimaxStrategy(int depth);
    std::pair<sf::Vector2i, sf::Vector2i>
    selectMove(const Board& board, Side aiSide) override;

private:
    int maxDepth;
};
