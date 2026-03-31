#pragma once
#include "Board.h"
#include <iostream>
#include <GameSaver.h>
#include <stdio.h>

class ConsoleView {

public:

    void printBoards(std::unique_ptr<Board>& myBoard, std::unique_ptr<Board>& enemyBoard) const;
    void clearBoard() const;
    void chooseMode() const;
    void printRules() const;

    void inputCoordinates() const;
    void incorrectForm() const;
    void alreadyShoot() const;
    void showResult(ShotResult& result) const;

    void greeting() const;
    void incorrectMode() const;
    void incorrectChoseInMenu() const;

    void printBoardForPlacement(Board& board) const;

    void showMainMenu() const;
    void showActiveGamesMenu(const std::vector<ActiveGameInfo>& games) const;
    void noActiveGames() const;
    void pressEnterToContinue() const;
    void incorrectStartMode() const;
    void newMove() const;

    void showShipPlacementInfo(ShipType type, int length, int remaining) const;
    void showPlacementSuccess() const;
    void showPlacementError() const;
    std::string getPlacementCoordinates() const;
    char getPlacementOrientation() const;
    void showAllShipsPlaced() const;

    void greetingMenu() const;
    void incorrectAnswerInMenu() const;
    void authorization() const;
    void registrationName() const;
    void registrationEmail() const;
    void emailAlrdeadyRegistered() const;
    void incorrectAnswerInRegister() const;
    void registrationSuccess() const;
    void registrationFailed() const;
    void authorizationSuccess() const;
    void authorizationFailed() const;
    void askForNewEmail() const;
    void errorToLoginInSystem() const;

    void player1Welcome() const;
    void player2Welcome() const;
    void bothPlayersAuthorized() const;
    void switchingPlayers() const;
    void playerTurn(int playerNumber) const;
    void messageExistingGame() const;

    void invalidEmail() const;
    void nameTooLong() const;

    void errorCreateGame() const;
    void exitFromGame() const;

    void player2AuthFailed() const;
    void showPlayerInfo(const std::string& player1Name, int player1Id,
                       const std::string& player2Name, int player2Id) const;
    void pvpModeInDevelopment() const;

    void gameIsOver() const;
};