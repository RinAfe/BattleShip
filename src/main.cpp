#include "BattleShip.h"

ConsoleView GLOBAL_CONSOLE;

int main(){

    GLOBAL_CONSOLE.startConsole();

    int mode;
    std::cin >> mode;

    while (mode < 0 || mode > 2) {
        GLOBAL_CONSOLE.incorrectStartMode();
        std::cin >> mode;
    }


    if (mode == 1) {
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
        std::cout << "Игра окончена!\n";
    }
    else if (mode == 2) {

    }
    return 0;
}