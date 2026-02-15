#pragma once
#include "Board.h"
#include <iostream>
#include <GameSaver.h>

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

    void showMainMenu();
    void showActiveGamesMenu(const std::vector<ActiveGameInfo>& games);
    void noActiveGames();
    void pressEnterToContinue();
    void incorrectStartMode();
    void newMove();


    void showShipPlacementInfo(ShipType type, int length, int remaining);
    void showPlacementSuccess();
    void showPlacementError();
    std::string getPlacementCoordinates();
    char getPlacementOrientation();
    void showAllShipsPlaced();

    void greetingMenu();
    void incorrectAnswerInMenu();
    void authorization();
    void registrationName();
    void registrationEmail();
    void emailAlrdeadyRegistered();
    void incorrectAnswerInRegister();
    void registrationSuccess();
    void registrationFailed();
    void authorizationSuccess();
    void authorizationFailed();
    void askForNewEmail();
    void errorToLoginInSystem();

    void player1Welcome();
    void player2Welcome();
    void bothPlayersAuthorized();
    void switchingPlayers();
    void playerTurn(int playerNumber);
    void messageExistingGame();

    void invalidEmail() const;
    void nameTooLong() const;

    void gameIsOver();
};