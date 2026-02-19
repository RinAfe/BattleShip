#include "BattleShip.h"

BattleShip::BattleShip(int type, bool needPlacement) {
    if (type == 1) {
        AIGame = std::make_unique<GameVsAI>(needPlacement);
    } else {
        AIGame = nullptr;
    }
}

void BattleShip::updatePlayerStats(pqxx::connection& conn, int playerId, bool playerWon) {
    try {
        pqxx::work work(conn);

        if (playerWon) {
            // Игрок победил
            work.exec_params(
                "UPDATE players SET count_games = count_games + 1, count_wins = count_wins + 1 WHERE id = $1",
                pqxx::params{playerId}
            );
            std::cout << "✅ Победа! Статистика игрока обновлена" << std::endl;
        } else {
            // Игрок проиграл
            work.exec_params(
                "UPDATE players SET count_games = count_games + 1, count_loses = count_loses + 1 WHERE id = $1",
                pqxx::params{playerId}
            );
            std::cout << "❌ Поражение. Статистика игрока обновлена" << std::endl;
        }

        work.commit();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка при обновлении статистики: " << e.what() << std::endl;
    }
}