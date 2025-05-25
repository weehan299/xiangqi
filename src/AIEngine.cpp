
#include "AIEngine.h"

AIEngine::AIEngine(Side side)
: aiSide(side)
, rng(std::random_device{}())
{}

void AIEngine::makeMove(Board& board) {
    // only move if it's the AI’s turn
    if (board.getCurrentTurn() != aiSide) return;

    auto moves = board.getLegalMoves(aiSide);
    if (moves.empty()) return;

    std::uniform_int_distribution<size_t> dist(0, moves.size() - 1);
    auto [src, dst] = moves[dist(rng)];
    board.movePiece(src.x, src.y, dst.x, dst.y);
}