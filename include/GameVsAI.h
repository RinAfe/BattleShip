#include "ConsoleView.h"
#include "CustomPlayer.h"
#include "AIPlayer.h"

class GameVsAI {
public:
    std::unique_ptr<CustomPlayer> CustomPlayer1;
    std::unique_ptr<AIPlayer> AIPlayer1;

public:
    GameVsAI();

    bool gameIsOver();

    void newMove();
};