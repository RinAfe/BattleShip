#include "CustomPlayer.h"

std::pair<int, int> CustomPlayer::parseInput(const std::string& input) {
    if (input.length() < 2) return { -1, -1 };

    char colChar = std::toupper(input[0]); 
    std::string rowStr = input.substr(1);  
    int col = colChar - 'A';

    int row = std::stoi(rowStr) - 1;  

    return { row, col };
}

bool CustomPlayer::isValidInput(const std::string& input) {
    if (input.length() < 2 || input.length() > 3) return false;

    char colChar = std::toupper(input[0]);
    if (colChar < 'A' || colChar > 'J') return false;

    try {
        int row = std::stoi(input.substr(1));
        if (row < 1 || row > 10) return false;
    }
    catch (...) {
        return false;
    }

    return true;
}

void CustomPlayer::makeMove(Board& enemyBoard) {
    bool continueTurn = true;

    while (continueTurn) {
        std::string input;
        bool validMove = false;

        while (!validMove) {
            consolePlayer->inputCoordinates();
            std::cin >> input;

            if (!isValidInput(input)) {
                consolePlayer->incorrectForm();
                continue;
            }

            auto [r, c] = parseInput(input);

            char cell = enemyBoard.cellAt(r, c);
            if (cell == 'X' || cell == 'O') {
                consolePlayer->alreadyShoot();
                continue;
            }

            ShotResult result = enemyBoard.shoot(r, c);
            consolePlayer->showResult(result);
            validMove = true;

            continueTurn = (result == ShotResult::Hit || result == ShotResult::Kill);
        }
    }
}

CustomPlayer::CustomPlayer() {
    customPlayerBoard = std::make_unique<Board>();
    consolePlayer->chooseMode();
    int mode;
    std::cin >> mode;
    if (mode != 1 && mode != 2) {
        consolePlayer->incorrectMode();
        std::cin >> mode;
    }

    if (mode == 1) {
        customPlayerBoard->autoPlacement();
    }
    else {
        // Ручная расстановка
        std::vector<std::pair<int, ShipType>> ships = {
            {1, ShipType::Battleship},
            {2, ShipType::Cruiser},
            {3, ShipType::Destroyer},
            {4, ShipType::Patrol}
        };

        for (const auto& [count, type] : ships) {
            int length = static_cast<int>(type);

            for (int shipNum = 0; shipNum < count; ++shipNum) {
                bool placed = false;

                while (!placed) {
                    consolePlayer->printBoardForPlacement(*customPlayerBoard);
                    consolePlayer->showShipPlacementInfo(type, length, count - shipNum);

                    std::string coord = consolePlayer->getPlacementCoordinates();

                    if (coord == "0") {
                        customPlayerBoard->clear();
                        consolePlayer->clearBoard();
                        continue; 
                    }

                    char orientationChar = consolePlayer->getPlacementOrientation();

                    auto [r, c] = parseInput(coord);
                    Orientation o = (orientationChar == 'H' || orientationChar == 'h') ?
                        Orientation::H : Orientation::V;

                    placed = customPlayerBoard->placeShip(r, c, length, o, '#');

                    if (!placed) {
                        consolePlayer->showPlacementError();
                    }
                    else {
                        consolePlayer->showPlacementSuccess();
                    }
                }
            }
        }

        consolePlayer->showAllShipsPlaced();
        consolePlayer->printBoardForPlacement(*customPlayerBoard);
    }
}

void CustomPlayer::manualShipPlacement() {
    std::vector<std::pair<int, ShipType>> ships = {
        {1, ShipType::Battleship},
        {2, ShipType::Cruiser},
        {3, ShipType::Destroyer},
        {4, ShipType::Patrol}
    };

    for (const auto& [count, type] : ships) {
        int length = static_cast<int>(type);

        for (int shipNum = 0; shipNum < count; ++shipNum) {
            bool placed = false;

            while (!placed) {consolePlayer->printBoardForPlacement(*customPlayerBoard);
                consolePlayer->showShipPlacementInfo(type, length, count - shipNum);

                std::string coord = consolePlayer->getPlacementCoordinates();
                char orientationChar = consolePlayer->getPlacementOrientation();

                auto [r, c] = parseInput(coord);
                Orientation o = (orientationChar == 'H' || orientationChar == 'h') ?
                    Orientation::H : Orientation::V;

                placed = customPlayerBoard->placeShip(r, c, length, o, '#');

                if (!placed) {
                    consolePlayer->showPlacementError();
                }
                else {
                    consolePlayer->showPlacementSuccess();
                }
            }
        }
    }

    consolePlayer->showAllShipsPlaced();
    consolePlayer->printBoardForPlacement(*customPlayerBoard);
}