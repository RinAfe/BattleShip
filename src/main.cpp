#include "BattleShip.h"
#include "Auth.h"
#include "GameSaver.h"
#include <iostream>
#include <memory>
#include <Databases.h>
#include <pqxx/pqxx>
#include <vector>
#include <limits>

ConsoleView GLOBAL_CONSOLE;

int main() {
    pqxx::connection conn("host=localhost port=5432 dbname=battleship user=postgres password=postgres");
    initDataBases(conn);

    AuthManager player1Auth(GLOBAL_CONSOLE, conn);
    AuthManager player2Auth(GLOBAL_CONSOLE, conn);
    GameSaver gameSaver(conn);

    GLOBAL_CONSOLE.player1Welcome();
    if (!player1Auth.authenticate()) {
        GLOBAL_CONSOLE.errorToLoginInSystem();
        return 1;
    }

    int playerId = player1Auth.getSession().playerId;

    while (true) {
        GLOBAL_CONSOLE.showMainMenu();
        int mode;
        std::cin >> mode;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (mode == 0) {
            std::cout << "Выход из игры." << std::endl;
            break;
        }
        else if (mode == 1) {
            BattleShip game(1, true); // true - нужна расстановка кораблей

            int activeGameId = gameSaver.createNewGame(
                playerId,
                0, // AI
                "AI",
                *game.AIGame->CustomPlayer1->customPlayerBoard,
                *game.AIGame->AIPlayer1->AIBoard,
                playerId // первый ход за игроком
            );

            if (activeGameId == -1) {
                std::cout << "Ошибка создания игры!" << std::endl;
                continue;
            }

            while (!game.AIGame->gameIsOver()) {
                system("clear");

                GLOBAL_CONSOLE.printBoards(
                    game.AIGame->CustomPlayer1->customPlayerBoard,
                    game.AIGame->AIPlayer1->AIBoard
                );

                game.AIGame->newMove();

                gameSaver.updateGame(activeGameId,
                    *game.AIGame->CustomPlayer1->customPlayerBoard,
                    *game.AIGame->AIPlayer1->AIBoard,
                    game.AIGame->currentTurn);
            }

            std::string winner;
            if (game.AIGame->CustomPlayer1->customPlayerBoard->getCountShipSunk() == 10) {
                winner = "Bot"; // Победил компьютер
            } else {
                winner = player1Auth.getSession().name; // Победил игрок
            }

            auto savedGame = gameSaver.loadGameById(activeGameId);
            if (savedGame) {
                gameSaver.finishGame(savedGame->gameRecordId, winner);
            }

            system("clear");
            GLOBAL_CONSOLE.printBoards(
                game.AIGame->CustomPlayer1->customPlayerBoard,
                game.AIGame->AIPlayer1->AIBoard
            );
            GLOBAL_CONSOLE.gameIsOver();
            GLOBAL_CONSOLE.pressEnterToContinue();
        }
        else if (mode == 2) {
            // Игра с человеком (PvP)
            GLOBAL_CONSOLE.player2Welcome();
            if (!player2Auth.authenticate()) {
                std::cout << "Не удалось авторизовать второго игрока." << std::endl;
                GLOBAL_CONSOLE.pressEnterToContinue();
                continue;
            }

            int player2Id = player2Auth.getSession().playerId;
            GLOBAL_CONSOLE.bothPlayersAuthorized();

            std::cout << "\nИгрок 1: " << player1Auth.getSession().name
                      << " (ID: " << playerId << ")" << std::endl;
            std::cout << "Игрок 2: " << player2Auth.getSession().name
                      << " (ID: " << player2Id << ")" << std::endl;

            std::cout << "\nРежим 'Игрок против Игрока' в разработке..." << std::endl;
            GLOBAL_CONSOLE.pressEnterToContinue();
        }
        else if (mode == 3) {
            auto games = gameSaver.getActiveGames(playerId);

            if (games.empty()) {
                GLOBAL_CONSOLE.noActiveGames();
                GLOBAL_CONSOLE.pressEnterToContinue();
                continue;
            }

            GLOBAL_CONSOLE.showActiveGamesMenu(games);

            int choice;
            std::cin >> choice;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (choice == 0) {
                continue;
            }

            if (choice > 0 && choice <= static_cast<int>(games.size())) {
                const auto& selectedGame = games[choice - 1];

                if (selectedGame.gameType == "AI") {
                    auto savedGame = gameSaver.loadGameById(selectedGame.gameId);
                    if (savedGame) {

                        BattleShip game(1, false);

                        game.AIGame->CustomPlayer1->customPlayerBoard = savedGame->player1Board->clone();
                        game.AIGame->AIPlayer1->AIBoard = savedGame->player2Board->clone();
                        game.AIGame->currentTurn = (selectedGame.isMyTurn ? 1 : 2);

                        GLOBAL_CONSOLE.messageExistingGame();
                        GLOBAL_CONSOLE.pressEnterToContinue();

                        while (!game.AIGame->gameIsOver()) {
                            system("clear");

                            GLOBAL_CONSOLE.printBoards(
                                game.AIGame->CustomPlayer1->customPlayerBoard,
                                game.AIGame->AIPlayer1->AIBoard
                            );

                            game.AIGame->newMove();

                            gameSaver.updateGame(selectedGame.gameId,
                                *game.AIGame->CustomPlayer1->customPlayerBoard,
                                *game.AIGame->AIPlayer1->AIBoard,
                                game.AIGame->currentTurn);
                        }

                        // Игра завершена - определяем победителя
                        std::string winner;
                        if (game.AIGame->CustomPlayer1->customPlayerBoard->getCountShipSunk() == 10) {
                            winner = "Bot";
                        } else {
                            winner = player1Auth.getSession().name;
                        }

                        gameSaver.finishGame(savedGame->gameRecordId, winner);

                        system("clear");
                        GLOBAL_CONSOLE.printBoards(
                            game.AIGame->CustomPlayer1->customPlayerBoard,
                            game.AIGame->AIPlayer1->AIBoard
                        );
                        GLOBAL_CONSOLE.gameIsOver();
                        GLOBAL_CONSOLE.pressEnterToContinue();
                    }
                } else {
                    std::cout << "Продолжение PvP игры в разработке..." << std::endl;
                    GLOBAL_CONSOLE.pressEnterToContinue();
                }
            }
        }
        else {
            GLOBAL_CONSOLE.incorrectStartMode();
        }
    }

    return 0;
}