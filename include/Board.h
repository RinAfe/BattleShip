#pragma once

#include <cstdlib>
#include <string>
#include <array>
#include <vector>
#include <ctime>
#include <random>
#include <memory>

enum class Orientation { H, V };
enum class ShotResult { Miss, Hit, Kill };
enum class ShipType { Battleship = 4, Cruiser = 3, Destroyer = 2, Patrol = 1 };

class Board {
    std::array<std::array<char, 10>, 10> board;
    int countShipsSunk = 0;

    void markAroundSunkShip(const std::vector<std::pair<int, int>>& shipCells);
public:
    Board();

    int getCountShipSunk();

    void clear();
    bool placeShip(int r, int c, int len, Orientation o, char symbol = '#');
    void autoPlacement();
    void customPlacement();

    ShotResult shoot(int r, int c);
    char cellAt(int r, int c) const;
    bool isShipSunk(int r, int c);
    void findConnectedShip(int r, int c, std::vector<std::pair<int, int>>& shipCages);

};