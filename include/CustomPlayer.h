#pragma once
#include "Player.h"

class CustomPlayer : public Player {
public:
    std::unique_ptr<Board> customPlayerBoard;

    CustomPlayer();

    void makeMove(Board& enemyBoard) override;
    void manualShipPlacement();

    std::unique_ptr<ConsoleView> consolePlayer = std::make_unique<ConsoleView>();
private:
    std::pair<int, int> parseInput(const std::string& input);
    bool isValidInput(const std::string& input);
};