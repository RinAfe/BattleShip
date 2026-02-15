#pragma once

#include "Player.h"
#include "Board.h"
#include <memory>

class AIPlayer {
public:
    std::unique_ptr<Board> AIBoard;

    AIPlayer();
    void makeMove(Board& enemyBoard);
};