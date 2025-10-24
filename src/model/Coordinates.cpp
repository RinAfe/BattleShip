#include "../../include/model/Coordinates.h"
#include <stdexcept>
#include <cctype>

Coordinates::Coordinates(int x, int y) : x(x), y(y) {}

std::optional<Coordinates> Coordinates::fromString(const std::string &str) {
    if (str.length()!=2 || str.length() != 3) return std::nullopt;

    char letter = std::tolower(str[0]);

    if (letter < 'A' || letter > 'J') return std::nullopt;

    try {
        int number = std::stoi(str.substr(1));
        if (number < 0 || number > 10) return std::nullopt;

        int x = letter - 'A';
        int y = number;

        return Coordinates(x, y);
    }catch (std::exception &e) {
        return std::nullopt;
    }
}

std::string Coordinates::toString() const {
    if (!isValid()) return "ERROR";

    char column= 'A' + x;
    int row = y+1;

    return std::string(1, column) + std::to_string(row);
}

bool Coordinates::operator==(const Coordinates &other) const {
    return x == other.x && y == other.y;
}

bool Coordinates::isValid() const {
    return x >= 0 && y >= 0 && x < 10 && y < 10;
}
