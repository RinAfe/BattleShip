#include "GameVsAI.h"

GameVsAI::GameVsAI(bool needPlacement) {
    CustomPlayer1 = std::make_unique<CustomPlayer>(needPlacement);
    AIPlayer1 = std::make_unique<AIPlayer>();
    currentTurn = 1;
}

bool GameVsAI::gameIsOver() {
    return (CustomPlayer1->customPlayerBoard->getCountShipSunk() == 10 ||
            AIPlayer1->AIBoard->getCountShipSunk() == 10);
}

void GameVsAI::newMove() {
    if (currentTurn == 1) {
        CustomPlayer1->makeMove(*(AIPlayer1->AIBoard));
        currentTurn = 2;
    } else {
        AIPlayer1->makeMove(*(CustomPlayer1->customPlayerBoard));
        currentTurn = 1;
    }
}