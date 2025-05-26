// AIEngine.h
#pragma once

#include <memory>
#include "Board.h"
#include <SFML/System/Vector2.hpp>

// Strategy interface for move selection
class MoveStrategy {
public:
    virtual ~MoveStrategy() = default;
    // Given the current board and AI side, return a move (src, dst)
    virtual std::pair<sf::Vector2i, sf::Vector2i>
    selectMove(const Board& board, Side aiSide) = 0; // =0 syntax makes this a pure virtual function
};

// AI engine that uses a MoveStrategy
class AIEngine {
public:
    // Construct with side and initial strategy
    AIEngine(Side side, std::unique_ptr<MoveStrategy> strategy);

    // Change the algorithm at runtime
    void setStrategy(std::unique_ptr<MoveStrategy> strategy);

    // Called each frame to make the AI move when it's its turn
    void makeMove(Board& board);

private:
    Side aiSide;
    std::unique_ptr<MoveStrategy> strategy;
};