
// AIEngine.cpp
#include "AIEngine.hpp"
#include <random>

// AIEngine definitions
AIEngine::AIEngine(Side side, std::unique_ptr<MoveStrategy> strat)
    : aiSide(side), strategy(std::move(strat)) {}

void AIEngine::setStrategy(std::unique_ptr<MoveStrategy> strat) {
    strategy = std::move(strat);
}

void AIEngine::makeMove(Board& board) {
    if (board.getCurrentTurn() != aiSide) return;
    auto move = strategy->selectMove(board, aiSide);
    // Validate move to determine if aiengine successfully selected a move
    if (move.first.x < 0) return;
    board.movePiece(
        move.first.x, move.first.y,
        move.second.x, move.second.y
    );
}