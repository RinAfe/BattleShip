#pragma once

#include "GameVsAI.h"
#include <memory>

class BattleShip {
public:
    std::unique_ptr<GameVsAI> AIGame;

    BattleShip(int type, bool needPlacement = true);

    static void updatePlayerStats(pqxx::connection& conn, int playerId, bool playerWon);
};