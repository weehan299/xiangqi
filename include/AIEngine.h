
#ifndef AIENGINE_H
#define AIENGINE_H

#include "Board.h"
#include <random>

class AIEngine {
public:
    AIEngine(Side aiSide);

    // Choose and play one move on the board
    void makeMove(Board& board);

private:
    Side aiSide;
    std::mt19937 rng;
};

#endif // AIENGINE_H