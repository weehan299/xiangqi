// RandomStrategy.cpp
#include "RandomStrategy.h"

RandomStrategy::RandomStrategy()
    : rng(std::random_device{}()) {}

std::pair<sf::Vector2i, sf::Vector2i>
RandomStrategy::selectMove(const Board& board, Side aiSide) {
    std::vector<std::pair<sf::Vector2i, sf::Vector2i>> moves = board.getLegalMoves(aiSide);
    if (moves.empty())
        return {{-1, -1}, {-1, -1}};
    std::uniform_int_distribution<size_t> dist(0, moves.size() - 1);
    return moves[dist(rng)];
}