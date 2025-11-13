#include "AIPlayer.h"

AIPlayer::AIPlayer() {
    AIBoard = std::make_unique<Board>();
    AIBoard->autoPlacement();
}

void AIPlayer::makeMove(Board& enemyBoard) {
    bool hit = true;

    while (hit) {
        int r, c;
        bool validShot = false;

        while (!validShot) {
            r = std::rand() % 10;
            c = std::rand() % 10;

            char cell = enemyBoard.cellAt(r, c);
            if (cell == '.' || cell == '#') {
                validShot = true;
            }
        }

        ShotResult result = enemyBoard.shoot(r, c);
        hit = (result == ShotResult::Hit || result == ShotResult::Kill);
    }
}