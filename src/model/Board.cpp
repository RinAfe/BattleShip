#include "Board.h"

Board::Board() {
    clear();
}

void Board::clear() {
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            board[i][j] = '.';
        }
    }
}

bool Board::placeShip(int r, int c, int len, Orientation o, char symbol) {
    if (len != static_cast<int>(ShipType::Battleship) &&
        len != static_cast<int>(ShipType::Cruiser) &&
        len != static_cast<int>(ShipType::Destroyer) &&
        len != static_cast<int>(ShipType::Patrol)) {
        return false;
    }

    if (r < 0 || r >= 10 || c < 0 || c >= 10) return false;

    if (o == Orientation::H) {
        if (c + len > 10) return false;
    }
    else {
        if (r + len > 10) return false;
    }

    for (int i = 0; i < len; i++) {
        int checkR = (o == Orientation::H) ? r : r + i;
        int checkC = (o == Orientation::H) ? c + i : c;

        if (board[checkR][checkC] != '.') return false;

        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nr = checkR + dr;
                int nc = checkC + dc;

                if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10) {
                    if (board[nr][nc] != '.') {
                        return false;
                    }
                }
            }
        }
    }

    for (int i = 0; i < len; i++) {
        if (o == Orientation::H) {
            board[r][c + i] = symbol;
        }
        else {
            board[r + i][c] = symbol;
        }
    }

    return true;
}

void Board::findConnectedShip(int r, int c, std::vector<std::pair<int, int>>& shipCells) {

    if (r < 0 || r >= 10 || c < 0 || c >= 10) return;

    char cell = board[r][c];
    if (cell != '#' && cell != 'X') return;

    for (const auto& coord : shipCells) {
        if (coord.first == r && coord.second == c) return;
    }

    
    shipCells.push_back({ r, c });

    findConnectedShip(r - 1, c, shipCells); 
    findConnectedShip(r + 1, c, shipCells);
    findConnectedShip(r, c - 1, shipCells);
    findConnectedShip(r, c + 1, shipCells); 
}

bool Board::isShipSunk(int r, int c) {
    std::vector<std::pair<int, int>> shipCells;
    findConnectedShip(r, c, shipCells);

    for (const auto& [x, y] : shipCells) {
        if (board[x][y] == '#') { 
            return false;
        }
    }

    markAroundSunkShip(shipCells);
    countShipsSunk++;
    return true;
}

ShotResult Board::shoot(int r, int c) {
    if (r < 0 || r >= 10 || c < 0 || c >= 10)
        return ShotResult::Miss;

    if (board[r][c] == '.' || board[r][c] == 'O') {
        board[r][c] = 'O';
        return ShotResult::Miss;
    }

    if (board[r][c] == '#') {
        board[r][c] = 'X';

        if (isShipSunk(r, c)) {
            return ShotResult::Kill;
        }
        else {
            return ShotResult::Hit;
        }
    }

    return ShotResult::Miss;
}

char Board::cellAt(int r, int c) const {
    return board[r][c];
}

int Board::getCountShipSunk() {
    return countShipsSunk;
}

#include <random>

void Board::autoPlacement() {
    clear();

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> orientationDist(0, 1);
    std::uniform_int_distribution<> coordDist(0, 9);

    std::vector<std::pair<int, ShipType>> ships = {
        {1, ShipType::Battleship},
        {2, ShipType::Cruiser},
        {3, ShipType::Destroyer},
        {4, ShipType::Patrol}
    };

    for (const auto& [count, type] : ships) {
        int length = static_cast<int>(type);
        for (int i = 0; i < count; ++i) {
            bool placed = false;

            while (!placed) {
                Orientation o = (orientationDist(generator) == 0) ? Orientation::H : Orientation::V;
                int r, c;

                if (o == Orientation::H) {
                    r = coordDist(generator);
                    c = coordDist(generator) % (11 - length);
                }
                else {
                    r = coordDist(generator) % (11 - length);
                    c = coordDist(generator);
                }

                placed = placeShip(r, c, length, o, '#');
            }
        }
    }
}

void Board::markAroundSunkShip(const std::vector<std::pair<int, int>>& shipCells) {
    for (const auto& [r, c] : shipCells) {
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                int nr = r + dr;
                int nc = c + dc;

                if (nr >= 0 && nr < 10 && nc >= 0 && nc < 10) {
                    if (board[nr][nc] == '.') board[nr][nc] = 'O';

                }
            }
        }
    }
}

std::string Board::serialize() const {
    std::string result;
    result.reserve(100);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            result += board[i][j];
        }
    }
    return result;
}

void Board::deserialize(const std::string& data) {
    if (data.length() != 100) {
        // Если длина не 100, что-то пошло не так
        clear();
        return;
    }

    int index = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            board[i][j] = data[index++];
        }
    }

    // Пересчитываем количество потопленных кораблей
    countShipsSunk = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (board[i][j] == 'X') {
                // Проверяем, потоплен ли корабль
                std::vector<std::pair<int, int>> shipCells;
                findConnectedShip(i, j, shipCells);

                bool isSunk = true;
                for (const auto& [r, c] : shipCells) {
                    if (board[r][c] == '#') {
                        isSunk = false;
                        break;
                    }
                }
                if (isSunk && !shipCells.empty()) {
                    // Убеждаемся, что мы не посчитали один корабль дважды
                    bool alreadyCounted = false;
                    for (const auto& [r, c] : shipCells) {
                        if (r < i || (r == i && c < j)) {
                            alreadyCounted = true;
                            break;
                        }
                    }
                    if (!alreadyCounted) {
                        countShipsSunk++;
                    }
                }
            }
        }
    }
}

std::unique_ptr<Board> Board::clone() const {
    auto newBoard = std::make_unique<Board>();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            newBoard->board[i][j] = board[i][j];
        }
    }
    newBoard->countShipsSunk = countShipsSunk;
    return newBoard;
}