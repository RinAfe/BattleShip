#include "BattleShip.h"
#include "Auth.h"
#include <iostream>
#include <memory>
#include "DataManager.h"

ConsoleView GLOBAL_CONSOLE;

bool hasActiveGame(int player1Id, int player2Id) {
    // Заглушка - в будущем проверка в БД
    return false;
}

void continueExistingGame(int gameId) {
    // Заглушка - загрузка состояния игры из БД
    GLOBAL_CONSOLE.messageExistingGame();
}

int main() {

    DataManager data_manager;

    data_manager.initDataBase();

    AuthManager player1Auth(GLOBAL_CONSOLE);
    AuthManager player2Auth(GLOBAL_CONSOLE);

    GLOBAL_CONSOLE.player1Welcome();
    if (!player1Auth.authenticate()) {
        GLOBAL_CONSOLE.errorToLoginInSystem();
        return 1;
    }

    GLOBAL_CONSOLE.startConsole();

    int mode;
    std::cin >> mode;

    while (mode < 0 || mode > 2) {
        GLOBAL_CONSOLE.incorrectStartMode();
        std::cin >> mode;
    }

    if (mode == 1) {
        // Режим против компьютера
        BattleShip game(mode);
        while (!game.AIGame->gameIsOver()) {
            system("clear");

            GLOBAL_CONSOLE.printBoards(
                game.AIGame->CustomPlayer1->customPlayerBoard,
                game.AIGame->AIPlayer1->AIBoard
            );

            game.AIGame->newMove();
        }

        system("clear");
        GLOBAL_CONSOLE.printBoards(
            game.AIGame->CustomPlayer1->customPlayerBoard,
            game.AIGame->AIPlayer1->AIBoard
        );
        GLOBAL_CONSOLE.gameIsOver();
    }
    else if (mode == 2) {

        GLOBAL_CONSOLE.player2Welcome();
        if (!player2Auth.authenticate()) {
            std::cout << "Не удалось войти в систему второго игрока. Выход." << std::endl;
            return 1;
        }

        int player1Id = player1Auth.getSession().playerId;
        int player2Id = player2Auth.getSession().playerId;

        if (hasActiveGame(player1Id, player2Id)) {
            std::cout << "\nУ вас есть незавершенная игра!" << std::endl;
            std::cout << "1. Продолжить игру" << std::endl;
            std::cout << "2. Начать новую игру" << std::endl;
            std::cout << "Выберите действие (1/2): ";

            int choice;
            std::cin >> choice;

            if (choice == 1) {
                // int gameId = database.getActiveGameId(player1Id, player2Id);
                // continueExistingGame(gameId);
                std::cout << "Функция продолжения игры в разработке..." << std::endl;
                return 0;
            }
        }

        GLOBAL_CONSOLE.bothPlayersAuthorized();

        std::cout << "\nИгрок 1: " << player1Auth.getSession().name
                  << " (ID: " << player1Auth.getSession().playerId << ")" << std::endl;
        std::cout << "Игрок 2: " << player2Auth.getSession().name
                  << " (ID: " << player2Auth.getSession().playerId << ")" << std::endl;

        // Здесь должна быть логика PvP игры
        std::cout << "\nРежим 'Игрок против Игрока' в разработке..." << std::endl;

        // Временно запускаем игру против компьютера
        BattleShip game(1);
        while (!game.AIGame->gameIsOver()) {
            system("clear");

            GLOBAL_CONSOLE.printBoards(
                game.AIGame->CustomPlayer1->customPlayerBoard,
                game.AIGame->AIPlayer1->AIBoard
            );

            game.AIGame->newMove();
        }

        system("clear");
        GLOBAL_CONSOLE.printBoards(
            game.AIGame->CustomPlayer1->customPlayerBoard,
            game.AIGame->AIPlayer1->AIBoard
        );
        GLOBAL_CONSOLE.gameIsOver();
    }

    return 0;
}