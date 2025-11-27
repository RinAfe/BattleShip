#pragma once
#include "Board.h"
#include <iostream>


class ConsoleView {

public:
    void printBoards(std::unique_ptr<Board>& myBoard, std::unique_ptr<Board>& enemyBoard);
    void clearBoard();
    void chooseMode();
    void printRules();

    void inputCoordinates();
    void incorrectForm();
    void alreadyShoot();
    void showResult(ShotResult& result);

    void greeting();
    void incorrectMode();
    void incorrectChoseInMenu();

    void printBoardForPlacement(Board& board) const;

    //GLOBAL_CONSOLE
    void startConsole();
    void incorrectStartMode();
    void newMove();


    void showShipPlacementInfo(ShipType type, int length, int remaining);
    void showPlacementSuccess();
    void showPlacementError();
    std::string getPlacementCoordinates();
    char getPlacementOrientation();
    void showAllShipsPlaced();
};