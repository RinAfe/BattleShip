#pragma once

#include <memory>
#include "Board.h"
#include "ConsoleView.h"

class Player {
public:

    ConsoleView playerConsole;
    virtual void makeMove(Board& enemyBoard) = 0;

};