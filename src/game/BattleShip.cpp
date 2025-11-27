#include "BattleShip.h"

BattleShip::BattleShip(int type){
    AIGame = (type == 1 ? std::make_unique<GameVsAI>() : nullptr);
}