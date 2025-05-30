
// RandomStrategy.h
#pragma once
#include "AIEngine.h"
#include <random>

// A simple random move chooser
class RandomStrategy : public MoveStrategy {
public:
    RandomStrategy();
    std::pair<sf::Vector2i, sf::Vector2i>
    selectMove(const Board& board, Side aiSide) override;

private:
    std::mt19937 rng;
};
