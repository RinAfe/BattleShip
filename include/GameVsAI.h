#pragma once

#include "CustomPlayer.h"
#include "AIPlayer.h"
#include <memory>

class GameVsAI {
public:
    std::unique_ptr<CustomPlayer> CustomPlayer1;
    std::unique_ptr<AIPlayer> AIPlayer1;
    int currentTurn;

    GameVsAI(bool needPlacement = true);
    bool gameIsOver();
    void newMove();
};