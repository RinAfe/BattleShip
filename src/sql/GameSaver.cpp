#include "GameSaver.h"
#include <iostream>
#include <limits>

GameSaver::GameSaver(pqxx::connection& dbConn) : conn(dbConn) {}

int GameSaver::createNewGame(int player1Id, int player2Id, const std::string& gameType,
                             Board& player1Board, Board& player2Board, int currentTurnId) {
    try {
        pqxx::work work(conn);

        std::string winnerStatus = "In Progress";

        pqxx::result gameRes = work.exec_params(
            "INSERT INTO games (type_game, winner_game, date_start) "
            "VALUES ($1, $2, CURRENT_TIMESTAMP) RETURNING id",
            pqxx::params{gameType, winnerStatus}
        );

        int gameRecordId = gameRes[0][0].as<int>();

        std::string field1 = player1Board.serialize();
        std::string field2 = player2Board.serialize();

        field1 += (currentTurnId == player1Id ? '1' : '2');

        pqxx::result activeRes = work.exec_params(
            "INSERT INTO activeGames (game_id, first_field, last_field, "
            "id_first_player, id_last_player, current_turn) "
            "VALUES ($1, $2, $3, $4, $5, $6) RETURNING id",
            pqxx::params{gameRecordId, field1, field2, player1Id, player2Id, currentTurnId}
        );

        work.commit();

        std::cout << "Новая игра создана: games ID = " << gameRecordId
                  << ", activeGames ID = " << activeRes[0][0].as<int>() << std::endl;

        return activeRes[0][0].as<int>();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка создания игры: " << e.what() << std::endl;
        return -1;
    }
}

bool GameSaver::finishGame(int gameRecordId, const std::string& winner) {
    try {
        pqxx::work work(conn);

        work.exec_params(
            "UPDATE games SET winner_game = $1, date_end = CURRENT_TIMESTAMP "
            "WHERE id = $2",
            pqxx::params{winner, gameRecordId}
        );

        work.exec_params(
            "DELETE FROM activeGames WHERE game_id = $1",
            pqxx::params{gameRecordId}
        );

        work.commit();

        std::cout << "Игра " << gameRecordId << " завершена. Победитель: " << winner << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка завершения игры: " << e.what() << std::endl;
        return false;
    }
}

std::vector<ActiveGameInfo> GameSaver::getActiveGames(int playerId) {
    std::vector<ActiveGameInfo> games;

    try {
        pqxx::work work(conn);

        pqxx::result res = work.exec_params(
            "SELECT ag.id, ag.game_id, ag.id_first_player, ag.id_last_player, "
            "ag.first_field, ag.current_turn, "
            "CASE WHEN ag.id_last_player = 0 THEN 'AI' ELSE 'Player' END as type "
            "FROM activeGames ag "
            "WHERE ag.id_first_player = $1 OR ag.id_last_player = $1",
            pqxx::params{playerId}
        );

        for (const auto& row : res) {
            ActiveGameInfo info;
            info.gameId = row[0].as<int>();
            info.gameRecordId = row[1].as<int>();
            int firstPlayer = row[2].as<int>();
            int lastPlayer = row[3].as<int>();
            std::string field1 = row[4].as<std::string>();
            info.currentTurnId = row[5].as<int>();
            info.gameType = row[6].as<std::string>();

            if (info.gameType == "AI") {
                info.opponentId = 0;
                info.opponentName = "Компьютер";
            } else {
                info.opponentId = (firstPlayer == playerId) ? lastPlayer : firstPlayer;

                pqxx::result nameRes = work.exec_params(
                    "SELECT name FROM players WHERE id = $1",
                    pqxx::params{info.opponentId}
                );

                if (!nameRes.empty()) {
                    info.opponentName = nameRes[0][0].as<std::string>();
                } else {
                    info.opponentName = "Неизвестный";
                }
            }

            info.isMyTurn = (info.currentTurnId == playerId);

            games.push_back(info);
        }

    } catch (const std::exception& e) {
        std::cerr << "Ошибка получения активных игр: " << e.what() << std::endl;
    }

    return games;
}

std::unique_ptr<SavedGame> GameSaver::loadGameById(int activeGameId) {
    try {
        pqxx::work work(conn);

        pqxx::result res = work.exec_params(
            "SELECT ag.id, ag.game_id, ag.first_field, ag.last_field, "
            "ag.id_first_player, ag.id_last_player, ag.current_turn, "
            "g.type_game "
            "FROM activeGames ag "
            "JOIN games g ON ag.game_id = g.id "
            "WHERE ag.id = $1",
            pqxx::params{activeGameId}
        );

        if (res.empty()) return nullptr;

        auto game = std::make_unique<SavedGame>();
        game->activeGameId = res[0][0].as<int>();
        game->gameRecordId = res[0][1].as<int>();

        std::string field1 = res[0][2].as<std::string>();
        std::string field2 = res[0][3].as<std::string>();
        game->player1Id = res[0][4].as<int>();
        game->player2Id = res[0][5].as<int>();
        game->currentTurn = res[0][6].as<int>();
        game->gameType = res[0][7].as<std::string>();

        if (game->player2Id == 0) {
            game->gameType = "AI";
            game->player2Name = "Компьютер";

            pqxx::result nameRes = work.exec_params(
                "SELECT name FROM players WHERE id = $1",
                pqxx::params{game->player1Id}
            );
            if (!nameRes.empty()) {
                game->player1Name = nameRes[0][0].as<std::string>();
            }
        } else {
            pqxx::result nameRes1 = work.exec_params(
                "SELECT name FROM players WHERE id = $1",
                pqxx::params{game->player1Id}
            );
            pqxx::result nameRes2 = work.exec_params(
                "SELECT name FROM players WHERE id = $1",
                pqxx::params{game->player2Id}
            );

            if (!nameRes1.empty()) game->player1Name = nameRes1[0][0].as<std::string>();
            if (!nameRes2.empty()) game->player2Name = nameRes2[0][0].as<std::string>();
        }

        game->player1Board = std::make_unique<Board>();
        game->player2Board = std::make_unique<Board>();
        game->player1Board->deserialize(field1);
        game->player2Board->deserialize(field2);

        return game;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка загрузки игры: " << e.what() << std::endl;
        return nullptr;
    }
}

bool GameSaver::updateGame(int activeGameId, Board& player1Board, Board& player2Board, int currentTurnId) {
    try {
        pqxx::work work(conn);

        std::string field1 = player1Board.serialize();
        std::string field2 = player2Board.serialize();

        work.exec_params(
            "UPDATE activeGames SET first_field = $1, last_field = $2, current_turn = $3 "
            "WHERE id = $4",
            pqxx::params{field1, field2, currentTurnId, activeGameId}
        );

        work.commit();
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Ошибка обновления игры: " << e.what() << std::endl;
        return false;
    }
}

bool GameSaver::deleteActiveGame(int activeGameId) {
    try {
        pqxx::work work(conn);
        work.exec_params("DELETE FROM activeGames WHERE id = $1", pqxx::params{activeGameId});
        work.commit();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка удаления активной игры: " << e.what() << std::endl;
        return false;
    }
}

bool GameSaver::hasActiveGame(int player1Id, int player2Id) {
    try {
        pqxx::work work(conn);

        pqxx::result res = work.exec_params(
            "SELECT id FROM activeGames WHERE "
            "(id_first_player = $1 AND id_last_player = $2) OR "
            "(id_first_player = $2 AND id_last_player = $1)",
            pqxx::params{player1Id, player2Id}
        );

        return !res.empty();

    } catch (const std::exception& e) {
        std::cerr << "Ошибка проверки игры: " << e.what() << std::endl;
        return false;
    }
}

void GameSaver::updatePlayerStats(int playerId, bool playerWon) {
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