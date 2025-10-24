#pragma once

#ifndef BATTLESHIP_COORDINATES_H
#define BATTLESHIP_COORDINATES_H

#endif //BATTLESHIP_COORDINATES_H

#include <string>
#include <optional>

struct Coordinates {
    int x;
    int y;

    Coordinates(int x = 0, int y = 0){}

    static std::optional<Coordinates> fromString(const std::string& str);
    std::string toString() const;

    bool operator==(const Coordinates& other) const;
    bool operator!=(const Coordinates& other) const {return !(*this == other);};
    bool isValid() const;
};

