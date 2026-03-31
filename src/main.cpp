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

        if (mode == 0) {
            GLOBAL_CONSOLE.exitFromGame();
            break;
        }
        else if (mode == 1) {
            // Новая игра с компьютером
            BattleShip game(1, true);

            int activeGameId = gameSaver.createNewGame(
                playerId,
                0,
                "AI",
                *game.AIGame->CustomPlayer1->customPlayerBoard,
                *game.AIGame->AIPlayer1->AIBoard,
                playerId
            );

            if (activeGameId == -1) {
                GLOBAL_CONSOLE.errorCreateGame();
                continue;
            }

            while (!game.AIGame->gameIsOver()) {

                if (game.AIGame->currentTurn == 1) {
                    GLOBAL_CONSOLE.printBoards(
                        game.AIGame->CustomPlayer1->customPlayerBoard,
                        game.AIGame->AIPlayer1->AIBoard
                    );
                }

                game.AIGame->newMove();

                gameSaver.updateGame(activeGameId,
                    *game.AIGame->CustomPlayer1->customPlayerBoard,
                    *game.AIGame->AIPlayer1->AIBoard,
                    game.AIGame->currentTurn);
            }

            // Игра завершена - определяем победителя
            bool playerWon = (game.AIGame->CustomPlayer1->customPlayerBoard->getCountShipSunk() != 10);
            std::string winner = playerWon ? player1Auth.getSession().name : "Bot";

            // Обновляем статистику игрока
            gameSaver.updatePlayerStats(playerId, playerWon);

            // Завершаем игру
            auto savedGame = gameSaver.loadGameById(activeGameId);
            if (savedGame) {
                gameSaver.finishGame(savedGame->gameRecordId, winner);
            }

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
                GLOBAL_CONSOLE.player2AuthFailed();
                GLOBAL_CONSOLE.pressEnterToContinue();
                continue;
            }

            int player2Id = player2Auth.getSession().playerId;
            GLOBAL_CONSOLE.bothPlayersAuthorized();

            GLOBAL_CONSOLE.showPlayerInfo(
                player1Auth.getSession().name, playerId,
                player2Auth.getSession().name, player2Id
            );

            GLOBAL_CONSOLE.pvpModeInDevelopment();
        }
        else if (mode == 3) {
            // Список активных игр
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
                        // Создаем игру БЕЗ расстановки кораблей
                        BattleShip game(1, false);

                        // Копируем сохраненные доски
                        game.AIGame->CustomPlayer1->customPlayerBoard =
                            std::make_unique<Board>(*savedGame->player1Board);
                        game.AIGame->AIPlayer1->AIBoard =
                            std::make_unique<Board>(*savedGame->player2Board);
                        game.AIGame->currentTurn = (selectedGame.isMyTurn ? 1 : 2);

                        GLOBAL_CONSOLE.messageExistingGame();
                        GLOBAL_CONSOLE.pressEnterToContinue();

                        // Игровой цикл
                        while (!game.AIGame->gameIsOver()) {

                            if (game.AIGame->currentTurn == 1) {
                                GLOBAL_CONSOLE.printBoards(
                                    game.AIGame->CustomPlayer1->customPlayerBoard,
                                    game.AIGame->AIPlayer1->AIBoard
                                );
                            }

                            game.AIGame->newMove();

                            gameSaver.updateGame(selectedGame.gameId,
                                *game.AIGame->CustomPlayer1->customPlayerBoard,
                                *game.AIGame->AIPlayer1->AIBoard,
                                game.AIGame->currentTurn);
                        }

                        // Определяем победителя
                        bool playerWon = (game.AIGame->CustomPlayer1->customPlayerBoard->getCountShipSunk() != 10);
                        std::string winner = playerWon ? player1Auth.getSession().name : "Bot";

                        gameSaver.updatePlayerStats(playerId, playerWon);

                        // Завершаем игру
                        auto savedGame = gameSaver.loadGameById(selectedGame.gameId);
                        if (savedGame) {
                            gameSaver.finishGame(savedGame->gameRecordId, winner);
                        }

                        // Финальная очистка и показ
                        system("clear");
                        GLOBAL_CONSOLE.printBoards(
                            game.AIGame->CustomPlayer1->customPlayerBoard,
                            game.AIGame->AIPlayer1->AIBoard
                        );
                        GLOBAL_CONSOLE.gameIsOver();
                        GLOBAL_CONSOLE.pressEnterToContinue();
                    }
                } else {
                    GLOBAL_CONSOLE.pvpModeInDevelopment();
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