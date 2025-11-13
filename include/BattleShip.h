#include "GameVsAI.h"
#include "GameVsCustom.h"

class BattleShip {
public:
    std::unique_ptr<GameVsAI> AIGame;
    //std::unique_ptr<GameVsCustom> CustomPlayersGame;

    BattleShip(int type);

};