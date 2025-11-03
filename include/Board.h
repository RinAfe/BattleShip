#pragma once

#include <iostream>
#include <cstdlib>
#include <string>

class Board{
  char board[10][10];

public:
  Board() {
    for (size_t i = 0; i < 10; i++) {
      for (size_t j = 0; j < 10; j++) {
        board[i][j] = '.';
      }
    }
  }

  void clearBoard();
  void printBoard();
  void setShip(char);

};