#include "BattleShip.h"

BattleShip::BattleShip(int type, bool needPlacement) {
    if (type == 1) {
        AIGame = std::make_unique<GameVsAI>(needPlacement);
    } else {
        AIGame = nullptr;
    }
}