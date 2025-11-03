#include "Board.h"

bool validCage(const std::string& cage) {
    if (cage.size() != 2 && cage.size()!=3) return false;
    if (cage[0] < 'A' || cage[0] > 'J') return false;
    if (cage.size() == 2) {
        return (cage[1] > '0' && cage[1] <= '9');
    }
    else {
        return (cage.substr(1) == "10");
    }
}

void Board::clearBoard() {
    for (size_t i = 0; i < 10; i++) {
        for (size_t j = 0; j < 10; j++) {
            board[i][j] = '.';
        }
    }
}

void Board::printBoard() {
    std::cout << "  ";
    for (char i = 'A' - 1; i < 'A' + 9; i++) {
        std::cout << (char)(i+1) << " ";
    }
    std::cout << std::endl;
    for (size_t i = 0; i < 10; i++) {
        if (i == 9) std::cout << i + 1;
        else std::cout << i + 1 << " ";
        for (size_t j = 0; j < 10; j++) {
            std::cout << board[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void Board::setShip(char typeShip) {
    typeShip = std::tolower(typeShip);
    while (typeShip < 'k' || typeShip > 'n') {
        std::cout << "Неверный тип корабля, введите еще раз(0 - выход): ";
        std::cin >> typeShip;
        if (typeShip == '0') return;
    }
    std::string startPos, endPos;
    while (true) {
        std::cout << "Введите клетку носа корабля: ";
        std::cin >> startPos;
        startPos[0] = std::toupper(startPos[0]);
        if (!validCage(startPos)) {
            std::cout << "Неверная клетка, попробуй еще раз!" << std::endl;
            continue;
        }
        if (typeShip != 'n') {
            std::cout << "Введите конца корабля: ";
            std::cin >> endPos;
            endPos[0] = std::toupper(endPos[0]);
            if (!validCage(endPos)) {
                std::cout << "Неверная клетка, попробуй еще раз!" << std::endl;
                continue;
            }
        }
        if (typeShip == 'k' || typeShip == 'l' || typeShip == 'm') {
            if ((abs(std::stoi(startPos.substr(1)) - std::stoi(endPos.substr(1))) + 1 == 'o' - typeShip) && (startPos[0] == endPos[0])) {
                size_t iS = std::stoi(startPos.substr(1));
                size_t iE = std::stoi(endPos.substr(1));
                if (iS > iE) std::swap(iS, iE);
                for (iS; iS <= iE; iS++) board[iS-1][startPos[0] - 'A'] = '#';
            }
            else if ((abs(startPos[0] - endPos[0]) + 1 == 'o' - typeShip ) && (startPos.substr(1) == endPos.substr(1))) {
                size_t jS = startPos[0] - 'A';
                size_t jE = endPos[0] - 'A';
                if (jS > jE) std::swap(jS, jE);
                for (jS; jS <= jE; jS++) board[std::stoi(startPos.substr(1)) - 1][jS] = '#';
                break;
            }
            else {std::cout << "Длина корабля не равна "<< 'o' - typeShip << " ,введите клетки еще раз!" << std::endl; continue;}
        }
        else board[std::stoi(startPos.substr(1)) - 1][startPos[0] - 'A'] = '#';
        break;
    }
}




