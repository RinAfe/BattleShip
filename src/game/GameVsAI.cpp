#include "GameVsAI.h"

GameVsAI::GameVsAI() {
    CustomPlayer1 = std::make_unique<CustomPlayer>();
    AIPlayer1 = std::make_unique<AIPlayer>();


}

bool GameVsAI::gameIsOver() {
    return(CustomPlayer1->customPlayerBoard->getCountShipSunk() == 10 || AIPlayer1->AIBoard->getCountShipSunk() == 10);
}

void GameVsAI::newMove() {
    CustomPlayer1->makeMove(*(AIPlayer1->AIBoard));
    if (gameIsOver()) return;
    AIPlayer1->makeMove(*(CustomPlayer1->customPlayerBoard));
}