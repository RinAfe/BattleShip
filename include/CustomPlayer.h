#pragma once

#include "Board.h"
#include "ConsoleView.h"
#include <memory>

class CustomPlayer {
public:
    std::unique_ptr<Board> customPlayerBoard;
    std::unique_ptr<ConsoleView> consolePlayer;

    CustomPlayer(bool needPlacement = true);
    void makeMove(Board& enemyBoard);
    void manualShipPlacement();

private:
    std::pair<int, int> parseInput(const std::string& input);
    bool isValidInput(const std::string& input);
};