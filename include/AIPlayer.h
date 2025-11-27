#pragma once

#include "Player.h"

class AIPlayer : public Player {

public:
    std::unique_ptr<Board> AIBoard;
    AIPlayer();
    void makeMove(Board& enemyBoard) override;
};